//  dibapi.cpp
//
//  Source file for Device-Independent Bitmap (DIB) API.  Provides
//  the following functions:
//
//  PaintDIB()          - Painting routine for a DIB
//  CreateDIBPalette()  - Creates a palette from a DIB
//  FindDIBBits()       - Returns a pointer to the DIB bits
//  DIBWidth()          - Gets the width of the DIB
//  DIBHeight()         - Gets the height of the DIB
//  PaletteSize()       - Gets the size required to store the DIB's palette
//  DIBNumColors()      - Calculates the number of colors
//                        in the DIB's color table
//  CopyHandle()        - Makes a copy of the given global memory block
//

#include "stdafx.h"
//#include <windows.h>
#include <assert.h>
#include "hdibapi.h"


BOOL WINAPI PaintDIB(HDC hDC, LPRECT lpDCRect, HDIB hDIB, 
					 LPRECT lpDIBRect, HPALETTE hPal)
{
	LPSTR    lpDIBHdr;            // Pointer to BITMAPINFOHEADER
	LPSTR    lpDIBBits;           // Pointer to DIB bits
	BOOL     bSuccess=FALSE;      // Success/fail flag
	HPALETTE hOldPal=NULL;        // Previous palette

	/* Check for valid DIB handle */
	if (hDIB == NULL)
		return FALSE;

	/* Lock down the DIB, and get a pointer to the beginning of the bit
	 *  buffer
	 */
	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	lpDIBBits = ::FindDIBBits(lpDIBHdr);

	// Select palette into DC.
	// Select as background since we have
	// already realized in forground if needed
	hOldPal = ::SelectPalette(hDC, hPal, TRUE);

	/* Make sure to use the stretching mode best for color pictures */
	::SetStretchBltMode(hDC, COLORONCOLOR);

	/* Determine whether to call StretchDIBits() or SetDIBitsToDevice() */
	if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDIBRect)) &&
	   (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect)))
		bSuccess = ::SetDIBitsToDevice(hDC,                    // hDC
								   lpDCRect->left,             // DestX
								   lpDCRect->top,              // DestY
								   RECTWIDTH(lpDCRect),        // nDestWidth
								   RECTHEIGHT(lpDCRect),       // nDestHeight
								   lpDIBRect->left,            // SrcX
								   (int)DIBHeight(lpDIBHdr) -
									  lpDIBRect->top -
									  RECTHEIGHT(lpDIBRect),   // SrcY
								   0,                          // nStartScan
								   (WORD)DIBHeight(lpDIBHdr),  // nNumScans
								   lpDIBBits,                  // lpBits
								   (LPBITMAPINFO)lpDIBHdr,     // lpBitsInfo
								   DIB_RGB_COLORS);            // wUsage
   else
	  bSuccess = ::StretchDIBits(hDC,                          // hDC
							   lpDCRect->left,                 // DestX
							   lpDCRect->top,                  // DestY
							   RECTWIDTH(lpDCRect),            // nDestWidth
							   RECTHEIGHT(lpDCRect),           // nDestHeight
							   lpDIBRect->left,                // SrcX
							   lpDIBRect->top,                 // SrcY
							   RECTWIDTH(lpDIBRect),           // wSrcWidth
							   RECTHEIGHT(lpDIBRect),          // wSrcHeight
							   lpDIBBits,                      // lpBits
							   (LPBITMAPINFO)lpDIBHdr,         // lpBitsInfo
							   DIB_RGB_COLORS,                 // wUsage
							   SRCCOPY);                       // dwROP

   ::GlobalUnlock((HGLOBAL) hDIB);

	/* Reselect old palette */
	if (hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}

   return bSuccess;
}

HPALETTE WINAPI CreateDIBPalette(HDIB hDIB)
{
	LPLOGPALETTE lpPal;      // pointer to a logical palette
	HANDLE hLogPal;          // handle to a logical palette
	HPALETTE hPal = NULL;    // handle to a palette
	int i;                   // loop index
	WORD wNumColors;         // number of colors in color table
	LPSTR lpbi;              // pointer to packed-DIB
	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	LPBITMAPCOREINFO lpbmc;  // pointer to BITMAPCOREINFO structure (old)
	BOOL bWinStyleDIB;       // flag which signifies whether this is a Win3.0 DIB
	BOOL bResult = FALSE;

	/* if handle to DIB is invalid, return FALSE */

	if (hDIB == NULL)
	  return NULL;

   lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

   /* get pointer to BITMAPINFO (Win 3.0) */
   lpbmi = (LPBITMAPINFO)lpbi;

   /* get pointer to BITMAPCOREINFO (old 1.x) */
   lpbmc = (LPBITMAPCOREINFO)lpbi;

   /* get the number of colors in the DIB */
   wNumColors = ::DIBNumColors(lpbi);

   if (wNumColors != 0)
   {
		/* allocate memory block for logical palette */
		hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE)
									+ sizeof(PALETTEENTRY)
									* wNumColors);

		/* if not enough memory, clean up and return NULL */
		if (hLogPal == 0)
		{
			::GlobalUnlock((HGLOBAL) hDIB);
			return NULL;
		}

		lpPal = (LPLOGPALETTE) ::GlobalLock((HGLOBAL) hLogPal);

		/* set version and number of palette entries */
		lpPal->palVersion = PALVERSION;
		lpPal->palNumEntries = (WORD)wNumColors;

		/* is this a Win 3.0 DIB? */
		bWinStyleDIB = IS_WIN30_DIB(lpbi);
		for (i = 0; i < (int)wNumColors; i++)
		{
			if (bWinStyleDIB)
			{
				lpPal->palPalEntry[i].peRed = lpbmi->bmiColors[i].rgbRed;
				lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
				lpPal->palPalEntry[i].peBlue = lpbmi->bmiColors[i].rgbBlue;
				lpPal->palPalEntry[i].peFlags = 0;
			}
			else
			{
				lpPal->palPalEntry[i].peRed = lpbmc->bmciColors[i].rgbtRed;
				lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen;
				lpPal->palPalEntry[i].peBlue = lpbmc->bmciColors[i].rgbtBlue;
				lpPal->palPalEntry[i].peFlags = 0;
			}
		}

		/* create the palette and get handle to it */
		hPal = CreatePalette(lpPal);
		::GlobalUnlock((HGLOBAL) hLogPal);
		::GlobalFree((HGLOBAL) hLogPal);
	}

	::GlobalUnlock((HGLOBAL) hDIB);

	return hPal;
}

DWORD WINAPI DIBWidth(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB

	/* point to the header (whether Win 3.0 and old) */

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	/* return the DIB width if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biWidth;
	else  /* it is an other-style DIB, so return its width */
		return (DWORD)lpbmc->bcWidth;
}

DWORD WINAPI DIBHeight(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB

	/* point to the header (whether old or Win 3.0 */

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	/* return the DIB height if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biHeight;
	else  /* it is an other-style DIB, so return its height */
		return (DWORD)lpbmc->bcHeight;
}

WORD WINAPI PaletteSize(LPSTR lpbi)
{
   /* calculate the size required by the palette */
   if (IS_WIN30_DIB (lpbi))
	  return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBQUAD));
   else
	  return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBTRIPLE));
}


WORD WINAPI DIBNumColors(LPSTR lpbi)
{
	WORD wBitCount;  // DIB bit count

	/*  If this is a Windows-style DIB, the number of colors in the
	 *  color table can be less than the number of bits per pixel
	 *  allows for (i.e. lpbi->biClrUsed can be set to some value).
	 *  If this is the case, return the appropriate value.
	 */

	if (IS_WIN30_DIB(lpbi))
	{
		DWORD dwClrUsed;

		dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
		if (dwClrUsed != 0)
			return (WORD)dwClrUsed;
	}

	/*  Calculate the number of colors in the color table based on
	 *  the number of bits per pixel for the DIB.
	 */
	if (IS_WIN30_DIB(lpbi))
		wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	else
		wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;

	/* return number of colors based on bits per pixel */
	switch (wBitCount)
	{
		case 1:
			return 2;

		case 4:
			return 16;

		case 8:
			return 256;

		default:
			return 0;
	}
}

HANDLE WINAPI CopyHandle (HANDLE h)
{
	BYTE  *lpCopy;
	BYTE  *lp;
	HANDLE     hCopy;
	DWORD      dwLen;

	if (h == NULL)
		return NULL;

	dwLen = ::GlobalSize((HGLOBAL) h);

	if ((hCopy = (HANDLE) ::GlobalAlloc (GHND, dwLen)) != NULL)
	{
		lpCopy = (BYTE *) ::GlobalLock((HGLOBAL) hCopy);
		lp     = (BYTE *) ::GlobalLock((HGLOBAL) h);

		while (dwLen--)
			*lpCopy++ = *lp++;

		::GlobalUnlock((HGLOBAL) hCopy);
		::GlobalUnlock((HGLOBAL) h);
	}

	return hCopy;
}

#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

BOOL WINAPI SaveDIB(HDIB hDib, HANDLE file)
{
	BITMAPFILEHEADER bmfHdr; // Header for Bitmap file
	LPBITMAPINFOHEADER lpBI;   // Pointer to DIB info structure
	DWORD dwDIBSize, len;

	if (hDib == NULL)
		return FALSE;

	/*
	 * Get a pointer to the DIB memory, the first of which contains
	 * a BITMAPINFO structure
	 */
	lpBI = (LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL) hDib);
	if (lpBI == NULL)
		return FALSE;

	if (!IS_WIN30_DIB(lpBI))
	{
		::GlobalUnlock((HGLOBAL) hDib);
		return FALSE;       // It's an other-style DIB (save not supported)
	}

	/*
	 * Fill in the fields of the file header
	 */

	/* Fill in file type (first 2 bytes must be "BM" for a bitmap) */
	bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM"

	// Calculating the size of the DIB is a bit tricky (if we want to
	// do it right).  The easiest way to do this is to call GlobalSize()
	// on our global handle, but since the size of our global memory may have
	// been padded a few bytes, we may end up writing out a few too
	// many bytes to the file (which may cause problems with some apps).
	//
	// So, instead let's calculate the size manually (if we can)
	//
	// First, find size of header plus size of color table.  Since the
	// first DWORD in both BITMAPINFOHEADER and BITMAPCOREHEADER conains
	// the size of the structure, let's use this.

	dwDIBSize = *(LPDWORD)lpBI + ::PaletteSize((LPSTR)lpBI);  // Partial Calculation

	// Now calculate the size of the image

	if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4))
	{
		// It's an RLE bitmap, we can't calculate size, so trust the
		// biSizeImage field

		dwDIBSize += lpBI->biSizeImage;
	}
	else
	{
		DWORD dwBmBitsSize;  // Size of Bitmap Bits only

		// It's not RLE, so size is Width (DWORD aligned) * Height

		dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;

		dwDIBSize += dwBmBitsSize;

		// Now, since we have calculated the correct size, why don't we
		// fill in the biSizeImage field (this will fix any .BMP files which
		// have this field incorrect).

		lpBI->biSizeImage = dwBmBitsSize;
	}


	// Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER)

	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;

	/*
	 * Now, calculate the offset the actual bitmap bits will be in
	 * the file -- It's the Bitmap file header plus the DIB header,
	 * plus the size of the color table.
	 */
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize
											  + PaletteSize((LPSTR)lpBI);

	// Write the file header
	::WriteFile(file, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &len, NULL);
	// Write the DIB header and the bits
	::WriteFile(file, lpBI, dwDIBSize, &len, NULL);

	::GlobalUnlock((HGLOBAL) hDib);
	::GlobalFree((HGLOBAL) hDib);
	hDib = NULL;

	return TRUE;
}

HDIB WINAPI ReadDIBFile(HANDLE file)
{
    BITMAPFILEHEADER bmfHeader;
	DWORD dwBitsSize, len;
	HDIB  hDIB;
	LPSTR pDIB;
	BOOL  bl;

	/*
	 * get length of DIB in bytes for use when reading
	 */
                           
	dwBitsSize = ::GetFileSize(file, NULL);

	/*
	 * Go read the DIB file header and check if it's valid.
	 */
	bl = ::ReadFile(file, (LPSTR)&bmfHeader, sizeof(bmfHeader), &len, NULL);
	if (!bl || (sizeof(bmfHeader) != len) || (bmfHeader.bfType != DIB_HEADER_MARKER))
	{
		return NULL;
	}
	/*
	 * Allocate memory for DIB
	 */
	hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);
	if (hDIB == 0)
	{
		return NULL;
	}
	pDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	/*
	 * Go read the bits.
	 */
	bl = ::ReadFile(file, pDIB, dwBitsSize - sizeof(BITMAPFILEHEADER), &len, NULL);
	if (!bl || len != dwBitsSize - sizeof(BITMAPFILEHEADER))
	{
		::GlobalUnlock((HGLOBAL) hDIB);
		::GlobalFree((HGLOBAL) hDIB);
		return NULL;
	}
	::GlobalUnlock((HGLOBAL) hDIB);

	return hDIB;
}

LPSTR WINAPI  FindDIBBits (LPSTR lpbi)
{
   return (lpbi + *(LPDWORD)lpbi + PaletteSize (lpbi));
}

void WINAPI  InitBitmapInfoHeader (LPBITMAPINFOHEADER lpBmInfoHdr,
                                        DWORD dwWidth,
                                        DWORD dwHeight,
                                          int nBPP)
{
   memset (lpBmInfoHdr, 0, sizeof (BITMAPINFOHEADER));

   lpBmInfoHdr->biSize      = sizeof (BITMAPINFOHEADER);
   lpBmInfoHdr->biWidth     = dwWidth;
   lpBmInfoHdr->biHeight    = dwHeight;
   lpBmInfoHdr->biPlanes    = 1;

   if (nBPP <= 1)
      nBPP = 1;
   else if (nBPP <= 4)
      nBPP = 4;
   else if (nBPP <= 8)
      nBPP = 8;
   else
      nBPP = 24;

   lpBmInfoHdr->biBitCount  = nBPP;
   lpBmInfoHdr->biSizeImage = WIDTHBYTES (dwWidth * nBPP) * dwHeight;
}

HANDLE WINAPI  BitmapToDIB (HBITMAP hBitmap, HPALETTE hPal)
{
   BITMAP             Bitmap;
   BITMAPINFOHEADER   bmInfoHdr;
   LPBITMAPINFOHEADER lpbmInfoHdr;
   LPSTR              lpBits;
   HDC                hMemDC;
   HANDLE             hDIB;
   HPALETTE           hOldPal = NULL;

      // Do some setup -- make sure the Bitmap passed in is valid,
      //  get info on the bitmap (like its height, width, etc.),
      //  then setup a BITMAPINFOHEADER.

   if (!hBitmap)
      return NULL;

   if (!GetObject (hBitmap, sizeof (Bitmap), (LPSTR) &Bitmap))
      return NULL;

   InitBitmapInfoHeader (&bmInfoHdr,
                         Bitmap.bmWidth,
                         Bitmap.bmHeight,
                         Bitmap.bmPlanes * Bitmap.bmBitsPixel);


      // Now allocate memory for the DIB.  Then, set the BITMAPINFOHEADER
      //  into this memory, and find out where the bitmap bits go.

   hDIB = GlobalAlloc (GHND, sizeof (BITMAPINFOHEADER) +
             PaletteSize ((LPSTR) &bmInfoHdr) + bmInfoHdr.biSizeImage);

   if (!hDIB)
      return NULL;

   lpbmInfoHdr  = (LPBITMAPINFOHEADER) GlobalLock (hDIB);
   *lpbmInfoHdr = bmInfoHdr;
   lpBits       = FindDIBBits ((LPSTR) lpbmInfoHdr);


      // Now, we need a DC to hold our bitmap.  If the app passed us
      //  a palette, it should be selected into the DC.

   hMemDC       = GetDC (NULL);

   if (hPal)
      {
      hOldPal = SelectPalette (hMemDC, hPal, FALSE);
      RealizePalette (hMemDC);
      }



      // We're finally ready to get the DIB.  Call the driver and let
      //  it party on our bitmap.  It will fill in the color table,
      //  and bitmap bits of our global memory block.

   if (!GetDIBits (hMemDC,
                   hBitmap,
                   0,
                   Bitmap.bmHeight,
                   lpBits,
                   (LPBITMAPINFO) lpbmInfoHdr,
                   DIB_RGB_COLORS))
      {
      GlobalUnlock (hDIB);
      GlobalFree (hDIB);
      hDIB = NULL;
      }
   else
      GlobalUnlock (hDIB);


      // Finally, clean up and return.

   if (hOldPal)
      SelectPalette (hMemDC, hOldPal, FALSE);

   ReleaseDC (NULL, hMemDC);

   return hDIB;
}

int WINAPI  PalEntriesOnDevice (HDC hDC)
{
   int nColors;

      // Find out the number of palette entries on this
      //  defice.

   nColors = GetDeviceCaps (hDC, SIZEPALETTE);


      // For non-palette devices, we'll use the # of system
      //  colors for our palette size.

   if (!nColors)
      nColors = GetDeviceCaps (hDC, NUMCOLORS);

   assert (nColors);

   return nColors;
}

HPALETTE WINAPI  GetSystemPalette (void)
{
   HDC           hDC;
   HPALETTE      hPal = NULL;
   HANDLE        hLogPal;
   LPLOGPALETTE  lpLogPal;
   int           i, nColors;


      // Find out how many palette entries we want.

   hDC = GetDC (NULL);
   if (!hDC)
      {
      DIBError (ERR_GETDC);
      return NULL;
      }

   nColors = PalEntriesOnDevice (hDC);
   ReleaseDC (NULL, hDC);


      // Allocate room for the palette and lock it.

   hLogPal = GlobalAlloc (GHND, sizeof (LOGPALETTE) +
                           nColors * sizeof (PALETTEENTRY));

   if (!hLogPal)
      {
      DIBError (ERR_CREATEPAL);
      return NULL;
      }

   lpLogPal = (LPLOGPALETTE) GlobalLock (hLogPal);

   lpLogPal->palVersion    = PALVERSION;
   lpLogPal->palNumEntries = nColors;

   for (i = 0;  i < nColors;  i++)
      {
      lpLogPal->palPalEntry[i].peBlue  = 0;
      *((LPWORD) (&lpLogPal->palPalEntry[i].peRed)) = i;
      lpLogPal->palPalEntry[i].peFlags = PC_EXPLICIT;
      }



      // Go ahead and create the palette.  Once it's created,
      //  we no longer need the LOGPALETTE, so free it.

   hPal = CreatePalette (lpLogPal);

   GlobalUnlock (hLogPal);
   GlobalFree (hLogPal);

   return hPal;
}

HDIB WINAPI  CopyRectToDIB(HWND hWnd, LPRECT lpRect)
{
	/* check for a valid window handle */
	if (!hWnd)	return NULL;

	RECT rectClient;
	POINT pt1, pt2;

	/* convert client coords to screen coords */
	pt1.x = lpRect->left;
	pt1.y = lpRect->top;
	pt2.x = lpRect->right;
	pt2.y = lpRect->bottom;
	ClientToScreen(hWnd, &pt1);
	ClientToScreen(hWnd, &pt2);
	rectClient.left   = pt1.x;
	rectClient.top    = pt1.y;
	rectClient.right  = pt2.x;
	rectClient.bottom = pt2.y;

	return CopyScreenToDIB(&rectClient);
}

HDIB WINAPI  CopyWindowToDIB(HWND hWnd, WORD fPrintArea)
{
  HDIB hDIB=NULL;  // handle to DIB

  /* check for a valid window handle */
  if (!hWnd)
    return NULL;

  switch (fPrintArea)
  {
    case PW_WINDOW: // copy entire window
    {
      RECT rectWnd;

      /* get the window rectangle */
      GetWindowRect(hWnd, &rectWnd);

      /*  get the DIB of the window by calling
       *  CopyScreenToDIB and passing it the window rect
       */
      hDIB = CopyScreenToDIB(&rectWnd);
    }
    break;

    case PW_CLIENT: // copy client area
    {
      RECT rectClient;
      POINT pt1, pt2;

      /* get the client area dimensions */
      GetClientRect(hWnd, &rectClient);

      /* convert client coords to screen coords */
      pt1.x = rectClient.left;
      pt1.y = rectClient.top;
      pt2.x = rectClient.right;
      pt2.y = rectClient.bottom;
      ClientToScreen(hWnd, &pt1);
      ClientToScreen(hWnd, &pt2);
      rectClient.left   = pt1.x;
      rectClient.top    = pt1.y;
      rectClient.right  = pt2.x;
      rectClient.bottom = pt2.y;

      /*  get the DIB of the client area by calling
       *  CopyScreenToDIB and passing it the client rect
       */
      hDIB = CopyScreenToDIB(&rectClient);
    }
    break;

    default:    // invalid print area
      return NULL;
  }

  /* return the handle to the DIB */
  return hDIB;
}

HDIB WINAPI  CopyScreenToDIB(LPRECT lpRect)
{
  HBITMAP hBitmap;    // handle to device-dependent bitmap
  HPALETTE hPalette;  // handle to palette
  HDIB hDIB=NULL;     // handle to DIB

  /*  get the device-dependent bitmap in lpRect by calling
   *  CopyScreenToBitmap and passing it the rectangle to grab
   */
  hBitmap = CopyScreenToBitmap(lpRect);

  /* check for a valid bitmap handle */
  if (!hBitmap)
    return NULL;

  /* get the current palette */
  hPalette = GetSystemPalette();

  /* convert the bitmap to a DIB */
  hDIB = (HDIB)BitmapToDIB(hBitmap, hPalette);

  /* clean up */
  DeleteObject(hBitmap);
  DeleteObject(hPalette);

  /* return handle to the packed-DIB */
  return hDIB;
}

HBITMAP WINAPI  CopyScreenToBitmap(LPRECT lpRect)
{
  HDC hScrDC, hMemDC;           // screen DC and memory DC
  HBITMAP hBitmap, hOldBitmap;  // handles to deice-dependent bitmaps
  int nX, nY, nX2, nY2;         // coordinates of rectangle to grab
  int nWidth, nHeight;          // DIB width and height
  int xScrn, yScrn;             // screen resolution

  /* check for an empty rectangle */
  if (IsRectEmpty(lpRect))
    return NULL;

  /*  create a DC for the screen and create
   *  a memory DC compatible to screen DC
   */
  hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
  hMemDC = CreateCompatibleDC(hScrDC);

  /* get points of rectangle to grab */
  nX = lpRect->left;
  nY = lpRect->top;
  nX2 = lpRect->right;
  nY2 = lpRect->bottom;

  /* get screen resolution */
  xScrn = GetDeviceCaps(hScrDC, HORZRES);
  yScrn = GetDeviceCaps(hScrDC, VERTRES);

  /* make sure bitmap rectangle is visible */
  if (nX < 0)
    nX = 0;
  if (nY < 0)
    nY = 0;
  if (nX2 > xScrn)
    nX2 = xScrn;
  if (nY2 > yScrn)
    nY2 = yScrn;
  nWidth = nX2 - nX;
  nHeight = nY2 - nY;

  /* create a bitmap compatible with the screen DC */
  hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);

  /* select new bitmap into memory DC */
  hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

  /* bitblt screen DC to memory DC */
  BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY) ;

  /*  select old bitmap back into memory DC and get handle to
   *  bitmap of the screen
   */
  hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);

  /* clean up */
  DeleteDC(hScrDC);
  DeleteDC(hMemDC);

  /* return handle to the bitmap */
  return hBitmap;
}

static char *szErrors[] = {"Not a DIB file!",
                           "Couldn't allocate memory!",
                           "Error reading file!",
                           "Error locking memory!",
                           "Error opening file!",
                           "Error creating palette!",
                           "Error getting a DC!",
                           "Error creating MDI Child!",
                           "Error creating Device Dependent Bitmap",
                           "StretchBlt() failed!",
                           "StretchDIBits() failed!",
                           "Paint requires both DDB and DIB!",
                           "SetDIBitsToDevice() failed!",
                           "Printer: StartDoc failed!",
                           "Printing: GetModuleHandle() couldn't find GDI!",
                           "Printer: SetAbortProc failed!",
                           "Printer: StartPage failed!",
                           "Printer: NEWFRAME failed!",
                           "Printer: EndPage failed!",
                           "Printer: EndDoc failed!",
                           "Only one DIB can be animated at a time!",
                           "No timers available for animation!",
                           "Can't copy to clipboard -- no current DIB!",
                           "Clipboard is busy -- operation aborted!",
                           "Can't paste -- no DIBs or DDBs in clipboard!",
                           "SetDIBits() failed!",
                           "File Not Found!",
                           "Error writing DIB file!"
                          };

void WINAPI  DIBError (int ErrNo)
{
   if ((ErrNo < ERR_MIN) || (ErrNo >= ERR_MAX))
   {
      MessageBox (GetFocus (), "Undefined Error!", NULL, MB_OK | MB_ICONHAND);
   }
   else
   {
      MessageBox (GetFocus (), szErrors[ErrNo], NULL, MB_OK | MB_ICONHAND);
   }
}
