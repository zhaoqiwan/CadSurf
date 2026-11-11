// hdibapi.h


#ifndef _INC_DIBAPI
#define _INC_DIBAPI

/* Handle to a DIB */
DECLARE_HANDLE(HDIB);

/* DIB constants */
#define PALVERSION   0x300
#define PW_WINDOW        1
#define PW_CLIENT        2

/* DIB Macros*/

#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))
#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)

// WIDTHBYTES performs DWORD-aligning of DIB scanlines.  The "bits"
// parameter is the bit count for the scanline (biWidth * biBitCount),
// and this macro returns the number of DWORD-aligned bytes needed
// to hold those bits.

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

/* Function prototypes */
BOOL      WINAPI  PaintDIB (HDC, LPRECT, HDIB, LPRECT, HPALETTE hPal);
HPALETTE  WINAPI  CreateDIBPalette(HDIB hDIB);
LPSTR     WINAPI  FindDIBBits (LPSTR lpbi);
DWORD     WINAPI  DIBWidth (LPSTR lpDIB);
DWORD     WINAPI  DIBHeight (LPSTR lpDIB);
WORD      WINAPI  PaletteSize (LPSTR lpbi);
WORD      WINAPI  DIBNumColors (LPSTR lpbi);
HANDLE    WINAPI  CopyHandle (HANDLE h);
BOOL      WINAPI  SaveDIB (HDIB hDib, HANDLE file);
HDIB      WINAPI  ReadDIBFile(HANDLE file);
void      WINAPI  InitBitmapInfoHeader (LPBITMAPINFOHEADER, DWORD, DWORD, int);
HANDLE    WINAPI  BitmapToDIB (HBITMAP hBitmap, HPALETTE hPal);
int       WINAPI  PalEntriesOnDevice (HDC hDC);
HPALETTE  WINAPI  GetSystemPalette (void);
HDIB	  WINAPI  CopyRectToDIB(HWND hWnd, LPRECT lpRect);
HDIB      WINAPI  CopyWindowToDIB(HWND hWnd, WORD fPrintArea);
HDIB      WINAPI  CopyScreenToDIB(LPRECT lpRect);
HBITMAP   WINAPI  CopyScreenToBitmap(LPRECT lpRect);                                          

enum {
      ERR_MIN = 0,                     // All error #s >= this value
      ERR_NOT_DIB = 0,                 // Tried to load a file, NOT a DIB!
      ERR_MEMORY,                      // Not enough memory!
      ERR_READ,                        // Error reading file!
      ERR_LOCK,                        // Error on a GlobalLock()!
      ERR_OPEN,                        // Error opening a file!
      ERR_CREATEPAL,                   // Error creating palette.
      ERR_GETDC,                       // Couldn't get a DC.
      ERR_CREATECHILD,                 // Error creating MDI child.
      ERR_CREATEDDB,                   // Error create a DDB.
      ERR_STRETCHBLT,                  // StretchBlt() returned failure.
      ERR_STRETCHDIBITS,               // StretchDIBits() returned failure.
      ERR_NODIBORDDB,                  // Error painting -- need BOTH DDB & DIB.
      ERR_SETDIBITSTODEVICE,           // SetDIBitsToDevice() failed.
      ERR_STARTDOC,                    // Error calling StartDoc().
      ERR_NOGDIMODULE,                 // Couldn't find GDI module in memory.
      ERR_SETABORTPROC,                // Error calling SetAbortProc().
      ERR_STARTPAGE,                   // Error calling StartPage().
      ERR_NEWFRAME,                    // Error calling NEWFRAME escape.
      ERR_ENDPAGE,                     // Error calling EndPage().
      ERR_ENDDOC,                      // Error calling EndDoc().
      ERR_ANIMATE,                     // Only one DIB animated @ a time.
      ERR_NOTIMERS,                    // No timers avail for pal animation.
      ERR_NOCLIPWINDOW,                // Now current window for clipboard.
      ERR_CLIPBUSY,                    // Clipboard is busy.
      ERR_NOCLIPFORMATS,               // During paste can't find DIB or DDB.
      ERR_SETDIBITS,                   // Error calling SetDIBits().
      ERR_FILENOTFOUND,                // Error opening file in GetDib()
      ERR_WRITEDIB,                    // Error writing DIB file.
      ERR_MAX                          // All error #s < this value
     };

void WINAPI  DIBError (int ErrNo);

#endif //!_INC_DIBAPI
