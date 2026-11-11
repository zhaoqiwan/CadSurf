// GLView.cpp : implementation file
//

#include "stdafx.h"
#include "CadSurf.h"
#include "CadSurfDoc.h"
#include "GLView.h"
#include "GeomException.h"
#include "ListOfCPoint3D.h"
#include "ListIteratorOfListOfCPoint3D.h"
#include "ListException.h"
#include "GLObject.h"

#include "ChildFrm.h" 

#include <algorithm>
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CGLView * g_pGLView = NULL;
/////////////////////////////////////////////////////////////////////////////
// CGLView

CGLView::CGLView(CWnd* pWnd, CGLDisplayContext* ctx) : ptrWnd(pWnd), theContext(ctx)
{
	xRot = -45.0f;
	zRot = -45.0f;
	yRot = 0;
	xTrans = yTrans = zTrans = 0.0f;

	myCamera = new CGLCamera();

	myViewType = GLAXONVIEW;
	myProjType = GLORTHOGRAPHIC;

	nRange = 500;

	m_bkRed = 0;
	m_bkGreen = 0.27450980392156862745098039215686;
	m_bkBlue = 0.27450980392156862745098039215686;

	displayMode = GLSHADED;

	m_antialias = false;

	//1
	colorv[0].amb[0] =  0.0F;	colorv[0].amb[1] =  0.0F;	colorv[0].amb[2] =  0.0F;
	colorv[0].diff[0] =  0.1F;	colorv[0].diff[1] =  0.6F;	colorv[0].diff[2] =  0.3F;
	colorv[0].spec[0] =  1.0F;	colorv[0].spec[1] =  1.0F;	colorv[0].spec[2] =  1.0F;
	colorv[0].ratio =  0.75F;
	colorv[0].indexes[0] =  0;	colorv[0].indexes[1] =  0;	colorv[0].indexes[2] =  0;

	//2
	colorv[1].amb[0] =  0.0F;	colorv[1].amb[1] =  0.0F;	colorv[1].amb[2] =  0.0F;
	colorv[1].diff[0] =  0.0F;	colorv[1].diff[1] =  0.2F;	colorv[1].diff[2] =  0.5F;
	colorv[1].spec[0] =  1.0F;	colorv[1].spec[1] =  1.0F;	colorv[1].spec[2] =  1.0F;
	colorv[1].ratio =  0.75F;
	colorv[1].indexes[0] =  0;	colorv[1].indexes[1] =  0;	colorv[1].indexes[2] =  0;

	//3
	colorv[2].amb[0] =  0.0F;	colorv[2].amb[1] =  0.05F;	colorv[2].amb[2] =  0.05F;
	colorv[2].diff[0] =  0.6F;	colorv[2].diff[1] =  0.0F;	colorv[2].diff[2] =  0.8F ;
	colorv[2].spec[0] =  1.0F;	colorv[2].spec[1] =  1.0F;	colorv[2].spec[2] =  1.0F ;
	colorv[2].ratio =  0.75F;
	colorv[2].indexes[0] =  0;	colorv[2].indexes[1] =  0;	colorv[2].indexes[2] =  0 ;

	ZoomON = false;
	PanON = false;
	RotON = false;
	for(int i=0;i<=9;i++)
			light[i].onlight=false;
	gridOn = false;



	myTrihedron = new CGLTrihedron;
	myGrid = new CGLGrid;

	InitGL();

	g_pGLView = this;
}

CGLView::~CGLView()
{
	delete myTrihedron;
	delete myGrid;
	delete myCamera;

	::wglDeleteContext(m_hRC);
	ptrWnd->ReleaseDC(ptrWnd->GetDC());
}

void CGLView::AddEntity(CGLObject * pGlObj)
{
	theContext->Append(pGlObj);
}

void CGLView::RemoveEntity(CGLObject * pGlObj)
{
	theContext->Remove(pGlObj);
}
/////////////////////////////////////////////////////////////////////////////
// CGLView drawing

void CGLView::Draw(int /*ntype*/)
{
    CRect rect;
    ptrWnd->GetClientRect(&rect);
    ReSize(rect.Width(), rect.Height());
    RenderScene(GL_RENDER);
}

void CGLView::ReSize(int cx, int cy)
{
    const int width = (std::max)(cx, 1);
    const int height = (std::max)(cy, 1);
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    ApplyProjection(width, height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void CGLView::ApplyProjection(int width, int height) const
{
    const double range = (std::max)(nRange, 0.001);
    const double aspect = static_cast<double>(width) / height;

    // A radius about the origin covers the scene under any view rotation.
    double sceneRadius = 0.0;
    if (!theContext->IsEmpty())
    {
        const CBoundingBox box = theContext->GetBoundingBox();
        const double x = (std::max)(std::abs(box.XMin()), std::abs(box.XMax()));
        const double y = (std::max)(std::abs(box.YMin()), std::abs(box.YMax()));
        const double z = (std::max)(std::abs(box.ZMin()), std::abs(box.ZMax()));
        sceneRadius = std::sqrt(x * x + y * y + z * z);
    }
    const auto camera = myCamera->GetPosition();
    const double cameraOffset =
        std::sqrt(camera.x * camera.x + camera.y * camera.y + camera.z * camera.z);
    const double depthExtent = (std::max)(sceneRadius, 2.0 * range) + cameraOffset + 1.0;

    if (myProjType == GLORTHOGRAPHIC)
    {
        const double halfWidth = range * (std::max)(aspect, 1.0);
        const double halfHeight = range * (std::max)(1.0 / aspect, 1.0);
        const double depth = depthExtent + std::abs(zTrans);
        glOrtho(-halfWidth - xTrans, halfWidth - xTrans,
                -halfHeight - yTrans, halfHeight - yTrans,
                -depth - zTrans, depth - zTrans);
    }
    else
    {
        const double distance = 4.0 * range;
        const double nearPlane = (std::max)(distance / 1000.0, distance - depthExtent);
        gluPerspective(30.0, aspect, nearPlane, distance + depthExtent);
        glTranslated(0.0, 0.0, -distance);
    }
}

void CGLView::Refresh()
{
	ptrWnd->Invalidate(FALSE);
}

void CGLView::InitGL() 
{
	int nPixelFormat;					// Pixel format index
	m_hDC = ptrWnd->GetDC()->m_hDC;			// Get the Device context

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// Size of this structure
		1,								// Version of this structure	
		PFD_DRAW_TO_WINDOW |			// Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |			// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,				// Double buffered mode
		PFD_TYPE_RGBA,					// RGBA Color mode
		24,								// Want 24bit color 
		0,0,0,0,0,0,					// Not used to select mode
		0,0,							// Not used to select mode
		0,0,0,0,0,						// Not used to select mode
		24,								// Size of depth buffer
		0,								// Not used to select mode
		0,								// Not used to select mode
		PFD_MAIN_PLANE,					// Draw in main plane
		0,								// Not used to select mode
		0,0,0 };						// Not used to select mode

	// Choose a pixel format that best matches that described in pfd
	nPixelFormat = ChoosePixelFormat(m_hDC, &pfd);

	// Set the pixel format for the device context
	VERIFY(SetPixelFormat(m_hDC, nPixelFormat, &pfd));

	// Create the rendering context
	m_hRC = wglCreateContext(m_hDC);

	// Make the rendering context current, perform initialization, then
	// deselect it
	VERIFY(wglMakeCurrent(m_hDC,m_hRC));

	glClearColor(m_bkRed, m_bkGreen, m_bkBlue, 1.0);
	
	//wglMakeCurrent(m_hDC, NULL);

	// Create the palette if needed
	InitializePalette();
}


// Initializes the CPalette object
void CGLView::InitializePalette(void)
{

	PIXELFORMATDESCRIPTOR pfd;
    LOGPALETTE* pPal;
    int pixelFormat = GetPixelFormat(m_hDC);
    int paletteSize;

    DescribePixelFormat(m_hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    /*
    ** Determine if a palette is needed and if so what size.
    */
    if (pfd.dwFlags & PFD_NEED_PALETTE) 
	{
		paletteSize = 1 << pfd.cColorBits;
    }else 
	if (pfd.iPixelType == PFD_TYPE_COLORINDEX) 
	{
		paletteSize = 4096;
    }else 
	{
		return;
    }

    pPal = (LOGPALETTE*)
	malloc(sizeof(LOGPALETTE) + paletteSize * sizeof(PALETTEENTRY));
    pPal->palVersion = 0x300;
    pPal->palNumEntries = paletteSize;

    if (pfd.iPixelType == PFD_TYPE_RGBA) 
	{
		/*
		** Fill the logical paletee with RGB color ramps
		*/
		int redMask = (1 << pfd.cRedBits) - 1;
		int greenMask = (1 << pfd.cGreenBits) - 1;
		int blueMask = (1 << pfd.cBlueBits) - 1;
		int i;

		for (i=0; i<paletteSize; ++i) 
		{
			pPal->palPalEntry[i].peRed =
				(((i >> pfd.cRedShift) & redMask) * 255) / redMask;
			pPal->palPalEntry[i].peGreen =
				(((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
			pPal->palPalEntry[i].peBlue =
				(((i >> pfd.cBlueShift) & blueMask) * 255) / blueMask;
			pPal->palPalEntry[i].peFlags = 0;
		}
    }else 
	{
		int numRamps = NUM_COLORS;
		int rampSize = (paletteSize - 20) / numRamps;
		int extra = (paletteSize - 20) - (numRamps * rampSize);
		int i, r;

		GetSystemPaletteEntries(m_hDC, 0, paletteSize, &pPal->palPalEntry[0]);

		for (r=0; r<numRamps; ++r) 
		{
			int rampBase = r * rampSize + 10;
			PALETTEENTRY *pe = &pPal->palPalEntry[rampBase];
			int diffSize = (int) (rampSize * colorv[r].ratio);
			int specSize = rampSize - diffSize;

			for (i=0; i<rampSize; ++i) 
			{
				GLfloat *c0, *c1;
				GLint a;
	
				if (i < diffSize) 
				{
					c0 = colorv[r].amb;
					c1 = colorv[r].diff;
					a = (i * 255) / (diffSize - 1);
				}else 
				{
					c0 = colorv[r].diff;
					c1 = colorv[r].spec;
					a = ((i - diffSize) * 255) / (specSize - 1);
				}

				pe[i].peRed = (BYTE) (a * (c1[0] - c0[0]) + 255 * c0[0]);
				pe[i].peGreen = (BYTE) (a * (c1[1] - c0[1]) + 255 * c0[1]);
				pe[i].peBlue = (BYTE) (a * (c1[2] - c0[2]) + 255 * c0[2]);
				pe[i].peFlags = PC_NOCOLLAPSE;
			}

			colorv[r].indexes[0] = rampBase;
			colorv[r].indexes[1] = rampBase + (diffSize-1);
			colorv[r].indexes[2] = rampBase + (rampSize-1);
		}

		for (i=0; i<extra; ++i) 
		{
			int index = numRamps*rampSize+10+i;
			PALETTEENTRY *pe = &pPal->palPalEntry[index];

			pe->peRed = (BYTE) 0;
			pe->peGreen = (BYTE) 0;
			pe->peBlue = (BYTE) 0;
		    pe->peFlags = PC_NOCOLLAPSE;
		}
    }

   m_hPalette2 = CreatePalette(pPal);
   free(pPal);

    if(m_hPalette2) 
	{
		SelectPalette(m_hDC, m_hPalette2, FALSE);
		RealizePalette(m_hDC);
    }		
	
}

BOOL CGLView::EnableRC(HDC m_hDC, HGLRC m_hRC, BOOL bEnable)
{
	if(bEnable)
	{
		if(!wglMakeCurrent(m_hDC, m_hRC))
		{
			CString err("CGLView::EnableRC - wglMakeCurrent failed\n");
			MessageBox(0, err, "OpenGL ERROR", MB_OK);
			return FALSE;
		}
	}
	else
		wglMakeCurrent(m_hDC, NULL);

	return TRUE;
}



BOOL CGLView::QueryNewPalette() 
{
	int nRet;
	// If the palette was created.
	if((HPALETTE)m_GLPalette)
	{
		// Selects the palette into the current device context
		SelectPalette(m_hDC, (HPALETTE)m_GLPalette, FALSE);

		// Map entries from the currently selected palette to
		// the system palette.  The return value is the number 
		// of palette entries modified.
		nRet = RealizePalette(m_hDC);

		// Repaint, forces remap of palette in current window
		ptrWnd->InvalidateRect(NULL,FALSE);

	}
	return nRet;
}

void CGLView::PaletteChanged() 
{
	
		// Select the palette into the device context
		SelectPalette(m_hDC,(HPALETTE)m_GLPalette,FALSE);

		// Map entries to system palette
		RealizePalette(m_hDC);
				
		// Remap the current colors to the newly realized palette
		UpdateColors(m_hDC);
}

void CGLView::MakeResource()
{
	::wglMakeCurrent(m_hDC,m_hRC);
}

void CGLView::ClearResource()
{
	::wglMakeCurrent(m_hDC,NULL);
}

void CGLView::SwapBuffers()
{
	::SwapBuffers(m_hDC);
}

void CGLView::RenderScene(GLenum mode)
{
	// Clear the window
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if(m_antialias)
	{
		glEnable (GL_LINE_SMOOTH);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
	    glLineWidth (1.125);
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable (GL_LINE_SMOOTH);
		glLineWidth (1);
		glDisable (GL_BLEND);
	}

	/*glEnable(GL_FOG);
   {
      GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};

      static GLint fogMode = GL_EXP;
      glFogi (GL_FOG_MODE, fogMode);
      glFogfv (GL_FOG_COLOR, fogColor);
      glFogf (GL_FOG_DENSITY, 0.0035);
      glHint (GL_FOG_HINT, GL_DONT_CARE);
      glFogf (GL_FOG_START, 1.0);
      glFogf (GL_FOG_END, 5.0);
   }
   glClearColor(0.5, 0.5, 0.5, 1.0);*/


	glPushMatrix();
    
	ApplyViewTransform();

	myTrihedron->SetSize(nRange/2);
	myTrihedron->Display();

	if(gridOn)
		myGrid->Display();

	//Get the Display Context;
	for(int i=0;i<=7;i++)
	{
	if(light[i].onlight)
	{
	//GLfloat posiMat1[4]={1,1,1,0};
	GLfloat ambiMat[4]={0.0, 0.0, 0.0, 1.0};
	GLfloat diffMat[4]={1.0, 1.0, 1.0, 1.0};
	GLfloat specMat[4]={1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT1, GL_POSITION, light[i].posiMat);
	glLightfv(GL_LIGHT1,GL_AMBIENT,ambiMat);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,diffMat);
    glLightfv(GL_LIGHT1,GL_SPECULAR,specMat);
	glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, light[i].exponent);//聚光灯聚光指数 0
	glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, light[i].cutoff);//聚光灯聚光发散半角 180
	glEnable(0x4001+i);}
	else
		glDisable(0x4001+i);
	}


	glTranslatef (0, 0, 0);
	theContext->Render(displayMode, mode == GL_SELECT);
	
	glPopMatrix();

	glFlush();
}

void CGLView::ZoomView(CPoint dPoint, CPoint mPoint)
{
	if(!ZoomON)
		DownPoint = dPoint;
	ZoomON = true;

	if(DownPoint.x > mPoint.x || DownPoint.y < mPoint.y)
		nRange *= 1.05;
	else
		nRange /= 1.05;

	DownPoint = mPoint;

	CRect rect;
	ptrWnd->GetClientRect(&rect);

	int w = rect.Width();
	int h = rect.Height();

	ReSize(w, h);

}

void CGLView::PanView(CPoint dPoint, CPoint mPoint)
{
	if(!PanON)
		DownPoint = dPoint;
	PanON = true;
	CSize pan = DownPoint - mPoint;
	DownPoint = mPoint;	

	myCamera->StrafeRight(0.01*nRange/2*pan.cx);
	myCamera->Move(F3dVector(0.0,-0.01*nRange/2*pan.cy,0.0));

	DownPoint = mPoint;

	CRect rect;
	ptrWnd->GetClientRect(&rect);

	int w = rect.Width();
	int h = rect.Height();

	ReSize(w, h);
}

void CGLView::RotateView(CPoint dPoint, CPoint mPoint)
{
	const CSize delta = (RotON ? DownPoint : dPoint) - mPoint;
	if (delta.cx == 0 && delta.cy == 0)
		return;

	RotON = true;
	myCamera->RotateX(delta.cy * 0.5);
	myCamera->RotateY(delta.cx * 0.5);
	myViewType = GLUNDEFINEDVIEW;
	DownPoint = mPoint;
}


void CGLView::TopView() 
{
	// TODO: Add your command handler code here
	xRot = 0;
	yRot = 0;
	zRot = 0;
	myViewType = GLTOPVIEW;
	myCamera->ResetCamera();
	Refresh();
}

void CGLView::BottomView() 
{
	// TODO: Add your command handler code here
	xRot = -180;
	yRot = 0;
	zRot = 0;
	myViewType = GLBOTTOMVIEW;
	myCamera->ResetCamera();
	Refresh();
}

void CGLView::FrontView() 
{
	// TODO: Add your command handler code here
	xRot = -90;
	yRot = 0;
	zRot = 0;
	myViewType = GLFRONTVIEW;
	myCamera->ResetCamera();
	Refresh();
}

void CGLView::BackView() 
{
	// TODO: Add your command handler code here
	xRot = -90;
	yRot = 0;
	zRot = -180;
	myViewType = GLBACKVIEW;
	myCamera->ResetCamera();
	Refresh();
}

void CGLView::LeftView() 
{
	// TODO: Add your command handler code here
	xRot = -90;
	yRot = 0;
	zRot = -90;
	myViewType = GLLEFTVIEW;
	myCamera->ResetCamera();
	Refresh();
}

void CGLView::RightView() 
{
	// TODO: Add your command handler code here
	xRot = -90;
	yRot = 0;
	zRot = 90;
	myViewType = GLRIGHTVIEW;
	myCamera->ResetCamera();
	Refresh();
}

void CGLView::AxonView() 
{
	// TODO: Add your command handler code here
	xRot = -45;
	yRot = 0;
	zRot = -45;
	myViewType = GLAXONVIEW;
	myCamera->ResetCamera();
	Refresh();
}

void CGLView::IsometricView(double zRotation)
{
	// This tilt gives equal foreshortening of the world X, Y and Z axes.
	// In this legacy view, xRot is measured from the top view.
	xRot = -54.7356103172453;
	yRot = 0;
	zRot = zRotation;
	myViewType = GLAXONVIEW;
	myCamera->ResetCamera();
	Refresh();
}

void CGLView::SetProjectionType(const GLProjectionType& aType)
{
	myProjType = aType;
	CRect rect;
	ptrWnd->GetClientRect(&rect);

	int w = rect.Width();
	int h = rect.Height();

	ReSize(w, h);
	Refresh();
}

void CGLView::FitAll() 
{
	// TODO: Add your command handler code here
	if(theContext->IsEmpty())
		return;
	FitView();
	CRect rect;
	ptrWnd->GetClientRect(&rect);

	int w = rect.Width();
	int h = rect.Height();

	ReSize(w, h);

	Refresh();
}

void CGLView::ZoomWindow(CRect zcRect)
{		
	CRect rcRect;
	ptrWnd->GetClientRect(&rcRect);

	int rcWidth, rcHeight, zcWidth, zcHeight;
	CPoint rcCentre, zcCentre;
	int dx, dy, dw, dh;
	GLdouble wRatio, hRatio;
	GLdouble dRange;

	GLint viewport[4];
	GLdouble mvmatrix[16],projmatrix[16];
	GLint realy;

	GLdouble cx, cy, cz;
	GLdouble mx, my, mz;

	rcWidth = rcRect.Width();
	rcHeight = rcRect.Height();

	zcWidth = zcRect.Width();
	zcHeight = zcRect.Height();

	rcCentre = rcRect.CenterPoint();
	zcCentre = zcRect.CenterPoint();

	//wglMakeCurrent(m_hDC,m_hRC);
	glPushMatrix();
	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

	realy=viewport[3]-(GLint)zcCentre.y-1;
	gluUnProject((GLdouble)zcCentre.x, (GLdouble)realy, 0.5, mvmatrix, projmatrix, viewport,
				&mx, &my, &mz);

	realy=viewport[3]-(GLint)rcCentre.y-1;
	gluUnProject((GLdouble)rcCentre.x, (GLdouble)realy, 0.5, mvmatrix, projmatrix, viewport,
				&cx, &cy, &cz);

	glPopMatrix();

	dx = cx - mx;
	dy = cy - my;

	dw = rcWidth - zcWidth;
	dh = rcHeight - zcHeight;

	wRatio = (GLdouble) zcWidth/rcWidth;
	hRatio = (GLdouble) zcHeight/rcHeight;

	dRange = (wRatio < hRatio) ? hRatio : wRatio;

	nRange *= dRange/0.8;

	//xTrans += dx;
	//yTrans += dy;
	myCamera->StrafeRight(-dx);
	myCamera->MoveForwards(-dy);

	//wglMakeCurrent(NULL,NULL);

	ReSize(rcWidth, rcHeight);

	Refresh();
}

void CGLView::ResetView() 
{
	// TODO: Add your command handler code here
	xRot = -45;
	yRot = 0;
	zRot = -45;
	xTrans = 0;
	yTrans = 0;
	zTrans = 0;
	nRange = 500;
	myCamera->ResetCamera();
	myViewType = GLAXONVIEW;

	CRect rect;
	ptrWnd->GetClientRect(&rect);

	int w = rect.Width();
	int h = rect.Height();

	ReSize(w, h);

	Refresh();
}

void CGLView::SetBackgroundColor(GLfloat r, GLfloat g, GLfloat b)
{
	m_bkRed = r;
	m_bkGreen = g;
	m_bkBlue = b;
	//wglMakeCurrent(m_hDC, m_hRC);
	glClearColor(m_bkRed, m_bkGreen, m_bkBlue, 1.0f);
	//::wglMakeCurrent(m_hDC, NULL);
	ptrWnd->InvalidateRect(NULL, FALSE);
}

void CGLView::SetLight(CLight light1[],int n)
{
	for(int i=0;i<=n;i++)
			light[i]=light1[i];
	//m_bkRed = r;
	//m_bkGreen = g;
	//m_bkBlue = b;
	//wglMakeCurrent(m_hDC, m_hRC);
	//glClearColor(m_bkRed, m_bkGreen, m_bkBlue, 1.0f);
	GLfloat posiMat1[4]={1,1,1,0};
	GLfloat ambiMat[4]={0.0, 0.0, 0.0, 1.0};
	GLfloat diffMat[4]={1.0, 1.0, 1.0, 1.0};
	GLfloat specMat[4]={1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT1, GL_POSITION, posiMat1);
	glLightfv(GL_LIGHT1,GL_AMBIENT,ambiMat);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,diffMat);
    glLightfv(GL_LIGHT1,GL_SPECULAR,specMat);
	glEnable(GL_LIGHT1);
	//glLightfv(GL_LIGHT1, GL_SPOT_EXPONENT, exponent);//聚光灯聚光指数 0
	//glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff);
	//::wglMakeCurrent(m_hDC, NULL);
	ptrWnd->InvalidateRect(NULL, FALSE);
}

void CGLView::GetBackgroundColor(GLfloat& r, GLfloat& g, GLfloat& b)
{
	r = m_bkRed;
	g = m_bkGreen;
	b = m_bkBlue;
}

void CGLView::SetDisplayMode(const GLDisplayMode& aMode)
{
	displayMode = aMode;
	ptrWnd->InvalidateRect(NULL, FALSE);
}

void CGLView::SetAntiAliasing(const bool& mode)
{
	m_antialias = mode;
	ptrWnd->InvalidateRect(NULL, FALSE);
}

void CGLView::SetGridOn(const bool& mode)
{
	gridOn = mode;
	ptrWnd->InvalidateRect(NULL, FALSE);
}

void CGLView::SetGridPara(const CPlane& plane, const double& size, const double& step)
{
	myGrid->SetPlane(plane);
	myGrid->SetSize(size);
	myGrid->SetStep(step);
	ptrWnd->InvalidateRect(NULL, FALSE);
}

void CGLView::ApplyViewTransform() const
{
    myCamera->Render();
    glRotatef(xRot, 1, 0, 0);
    glRotatef(yRot, 0, 1, 0);
    glRotatef(zRot, 0, 0, 1);
}

void CGLView::DrawOrientationIndicator() const
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    const int size = (std::min)({96, viewport[2], viewport[3]});
    if (size < 32)
        return;

    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LINE_BIT | GL_POINT_BIT | GL_TRANSFORM_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_LINE_STIPPLE);
    glDisable(GL_POINT_SMOOTH);
    glLineWidth(1.5f);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, viewport[2], 0, viewport[3], -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    ApplyViewTransform();
    GLdouble worldToView[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, worldToView);

    glLoadIdentity();
    const double scale = size / 96.0;
    glScaled(scale, scale, 1.0);
    glTranslated(48.0, 48.0, 0.0);

    static constexpr GLubyte colors[3][3] = {
        {255, 90, 90}, {107, 233, 120}, {112, 177, 255}
    };
    // Upright X, Y and Z labels drawn in screen space.
    static constexpr int strokes[3][3][4] = {
        {{-3, -4, 3, 4}, {-3, 4, 3, -4}, {}},
        {{-3, 4, 0, 0}, {3, 4, 0, 0}, {0, 0, 0, -4}},
        {{-3, 4, 3, 4}, {3, 4, -3, -4}, {-3, -4, 3, -4}}
    };

    for (int axis = 0; axis < 3; ++axis)
    {
        // Each of the first three columns is a world axis in view coordinates.
        const GLdouble* direction = worldToView + 4 * axis;
        const double length = std::hypot(direction[0], direction[1]);
        double labelX = 9.0;
        double labelY = -9.0;
        glColor3ubv(colors[axis]);

        if (length >= 0.12)
        {
            const double dx = direction[0] / length;
            const double dy = direction[1] / length;
            const double x = 31.0 * direction[0];
            const double y = 31.0 * direction[1];
            const double head = (std::min)(6.0, 15.0 * length);
            glBegin(GL_LINES);
            glVertex2d(0, 0);
            glVertex2d(x, y);
            glVertex2d(x, y);
            glVertex2d(x - head * (dx + 0.5 * dy), y - head * (dy - 0.5 * dx));
            glVertex2d(x, y);
            glVertex2d(x - head * (dx - 0.5 * dy), y - head * (dy + 0.5 * dx));
            glEnd();
            labelX = x + 9.0 * dx;
            labelY = y + 9.0 * dy;
        }
        else if (direction[2] > 0.0)
        {
            // Viewed end-on, a dot points toward the viewer and a cross away.
            glPointSize(5.0f);
            glBegin(GL_POINTS);
            glVertex2d(0, 0);
            glEnd();
        }
        else
        {
            glBegin(GL_LINES);
            glVertex2d(-3, -3);
            glVertex2d(3, 3);
            glVertex2d(-3, 3);
            glVertex2d(3, -3);
            glEnd();
        }

        glBegin(GL_LINES);
        const int strokeCount = axis == 0 ? 2 : 3;
        for (int i = 0; i < strokeCount; ++i)
        {
            const int* line = strokes[axis][i];
            glVertex2d(labelX + line[0], labelY + line[1]);
            glVertex2d(labelX + line[2], labelY + line[3]);
        }
        glEnd();
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
}

bool CGLView::PickReferencePoint(CPoint cursor, CPoint3D& result)
{
    const auto points = theContext->ReferencePoints();
    if (points.empty())
        return false;

    MakeResource();
    CRect client;
    ptrWnd->GetClientRect(&client);
    ReSize(client.Width(), client.Height());

    GLint viewport[4];
    GLdouble modelView[16], projection[16];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glPushMatrix();
    ApplyViewTransform();
    glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
    glPopMatrix();

    constexpr double tolerance = 10.0; // pixels
    double bestDistance = tolerance * tolerance;
    double bestDepth = 2.0;
    bool found = false;
    for (const auto& point : points)
    {
        GLdouble x, y, depth;
        if (!gluProject(point.GetX(), point.GetY(), point.GetZ(),
                        modelView, projection, viewport, &x, &y, &depth))
            continue;
        if (!(depth >= 0.0 && depth <= 1.0 &&
              x >= viewport[0] && x < viewport[0] + viewport[2] &&
              y >= viewport[1] && y < viewport[1] + viewport[3]))
            continue;

        const double dx = x - cursor.x;
        const double dy = viewport[1] + viewport[3] - 1 - y - cursor.y;
        const double distance = dx * dx + dy * dy;
        if (distance < bestDistance || (distance == bestDistance && depth < bestDepth))
        {
            result = point;
            bestDistance = distance;
            bestDepth = depth;
            found = true;
        }
    }
    return found;
}

void CGLView::DrawReferenceMarker(const CPoint3D& point)
{
    glPushMatrix();
    ApplyViewTransform();
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_POINT_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_POINT_SMOOTH);
    glPointSize(9.0f);
    glColor3ub(255, 196, 40);
    glBegin(GL_POINTS);
    glVertex3d(point.GetX(), point.GetY(), point.GetZ());
    glEnd();
    glPopAttrib();
    glPopMatrix();
}

void CGLView::DrawReferenceLine(const CPoint3D& start, const CPoint3D& finish)
{
    glPushMatrix();
    ApplyViewTransform();
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LINE_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glLineWidth(3.0f);
    glColor3ub(255, 196, 40);
    glBegin(GL_LINES);
    glVertex3d(start.GetX(), start.GetY(), start.GetZ());
    glVertex3d(finish.GetX(), finish.GetY(), finish.GetZ());
    glEnd();
    glPopAttrib();
    glPopMatrix();
}

void CGLView::PointToScreen(const CPoint3D& P, int& x, int& y)
{
	CPoint3D p = P;
	GLint viewport[4];
	GLdouble mvmatrix[16],projmatrix[16];
	GLdouble wx, wy, wz;
	//::wglMakeCurrent(m_hDC,m_hRC);
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);
	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);
	gluProject((GLdouble)p.GetX(), p.GetY(), p.GetZ(), mvmatrix, projmatrix, viewport,
				&wx, &wy, &wz);
	wx = sround(wx);
	wy = sround(wy);
	x = (int)wx; y = (int)wy;
	glPopMatrix();
	//::wglMakeCurrent(NULL,NULL);
}

void CGLView::ScreenToPoint(const int& x, const int& y, CPoint3D&  P)
{
	CPoint point(x,y);
	GLint viewport[4];
	GLdouble mvmatrix[16],projmatrix[16];
	GLint realy;
	GLdouble wx, wy, wz;
	//::wglMakeCurrent(m_hDC,m_hRC);
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);
	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);
	realy=viewport[3]-(GLint)point.y-1;
	gluUnProject((GLdouble)point.x, (GLdouble)realy, 0.5, mvmatrix, projmatrix, viewport,
				&wx, &wy, &wz);
	P.SetParam(wx, wy, wz);
	glPopMatrix();
	//::wglMakeCurrent(NULL,NULL);
}

void CGLView::FitView()
{
	GLint viewport[4];
	GLdouble mvmatrix[16],projmatrix[16];
	CRect volRect;
	GLdouble cx, cy, cz;
	GLdouble mx, my, mz;
	GLdouble lx, ly, lz, sx, sy, sz;
	CBoundingBox B = theContext->GetBoundingBox();
	lx = B.XMax(); ly = B.YMax(); lz = B.ZMax();
	sx = B.XMin(); sy = B.YMin(); sz = B.ZMin();

	//wglMakeCurrent(m_hDC, m_hRC);
	glPushMatrix();

	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);

	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

	gluProject(lx, ly, lz, mvmatrix, projmatrix, viewport,
				&mx, &my, &mz);

	gluProject(sx, sy, sz, mvmatrix, projmatrix, viewport,
				&cx, &cy, &cz);

	glPopMatrix();
	//::wglMakeCurrent(m_hDC, NULL);

	volRect.SetRect(cx,cy,mx,my);
	volRect.NormalizeRect();

	Fit3d(volRect);
}

void CGLView::Fit3d(CRect rect)
{	
	CRect zcRect = rect;
	CRect rcRect;
	ptrWnd->GetClientRect(&rcRect);

	int rcWidth, rcHeight, zcWidth, zcHeight;
	CPoint rcCentre, zcCentre;
	int dx, dy, dw, dh;
	GLdouble wRatio, hRatio;
	GLdouble dRange;

	GLint viewport[4];
	GLdouble mvmatrix[16],projmatrix[16];
	GLint realy;

	GLdouble cx, cy, cz;
	GLdouble mx, my, mz;

	rcWidth = rcRect.Width();
	rcHeight = rcRect.Height();

	zcWidth = zcRect.Width();
	zcHeight = zcRect.Height();

	rcCentre = rcRect.CenterPoint();
	zcCentre = zcRect.CenterPoint();

	//::wglMakeCurrent(m_hDC, m_hRC);

	glPushMatrix();

	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

	realy=viewport[3]-(GLint)zcCentre.y-1;
	gluUnProject((GLdouble)zcCentre.x, (GLdouble)realy, 0.5, mvmatrix, projmatrix, viewport,
				&mx, &my, &mz);

	realy=viewport[3]-(GLint)rcCentre.y-1;
	gluUnProject((GLdouble)rcCentre.x, (GLdouble)realy, 0.5, mvmatrix, projmatrix, viewport,
				&cx, &cy, &cz);

	glPopMatrix();

	//::wglMakeCurrent(m_hDC, NULL);

	dx = cx - mx;
	dy = cy - my;

	dw = rcWidth - zcWidth;
	dh = rcHeight - zcHeight;

	wRatio = (GLdouble) zcWidth/rcWidth;
	hRatio = (GLdouble) zcHeight/rcHeight;

	dRange = (wRatio < hRatio) ? hRatio : wRatio;

	nRange *= dRange/(double)0.8;

	myCamera->SetPosition(F3dVector(0,0,0));
	xTrans += dx;
	yTrans -= dy;
}

void CGLView::Select(const int &x, const int &y)
{
	theContext->Select(this, x, y);
}

void CGLView::MultiSelect(const int &x, const int &y)
{
	theContext->MultiSelect(this, x, y);
}

void CGLView::SweepSelect(const CRect& swRect)
{
	theContext->SweepSelect(this, swRect);
}

int CGLView::ProcessSelection(const int& xPos, const int& yPos, const int& sensitivity,
                              bool straightLinesOnly)
{
	MakeResource();

	// Space for selection buffer
	GLuint selectBuff[512];

	// Hit counter and viewport storeage
	GLint hits, viewport[4];

	// Setup selection buffer
	glSelectBuffer(512, selectBuff);
	
	// Get the viewport
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Switch to projection and save the matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	// Change render mode
	glRenderMode(GL_SELECT);

	// Establish new clipping volume to be unit cube around
	// mouse cursor point (xPos, yPos) and extending n pixels
	// in the vertical and horzontal direction
	int n = sensitivity;
	glLoadIdentity();
	gluPickMatrix(xPos, viewport[3] - yPos - 1, n, n, viewport);

	ApplyProjection((std::max)(viewport[2], 1), (std::max)(viewport[3], 1));

	// Draw the scene
	RenderScene(GL_SELECT);

	// Collect the hits
	hits = glRenderMode(GL_RENDER);

	// Restore the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Go back to modelview for normal rendering
	glMatrixMode(GL_MODELVIEW);

    // Each hit contains a name count, minimum and maximum depth, and the names.
    const GLuint* record = selectBuff;
    const GLuint* end = selectBuff + sizeof(selectBuff) / sizeof(selectBuff[0]);
    GLuint closestId = 0;
    GLuint closestDepth = 0;
    for (int i = 0; i < hits && end - record >= 3; ++i)
    {
        const GLuint names = record[0];
        const GLuint depth = record[1];
        record += 3;
        if (names > static_cast<GLuint>(end - record))
            break;
        if (names && (!closestId || depth < closestDepth))
        {
            const GLuint id = record[names - 1];
            CPoint3D start, finish;
            if (id && (!straightLinesOnly || theContext->GetStraightLine(id, start, finish)))
            {
                closestId = id;
                closestDepth = depth;
            }
        }
        record += names;
    }
    return static_cast<int>(closestId);
}

int CGLView::ProcessSweepSelection(const CRect& swRect, unsigned long int* id)
{
	GLint i;
	GLuint j;
	//::wglMakeCurrent(m_hDC,m_hRC);

	// Space for selection buffer
	GLuint selectBuff[512];

	// Hit counter and viewport storeage
	GLint hits = 0, viewport[4];

	// Setup selection buffer
	glSelectBuffer(512, selectBuff);
	
	// Get the viewport
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Switch to projection and save the matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	// Change render mode
	glRenderMode(GL_SELECT);

	// Establish new clipping volume to be cube around
	// center point of sweeping rectangle and extending pixels
	// falling within the height and width of the sweep rectangle
	// in the vertical and horzontal direction
	glLoadIdentity();
	gluPickMatrix(swRect.CenterPoint().x, viewport[3] - swRect.CenterPoint().y, swRect.Width(), swRect.Height(), viewport);

	ApplyProjection((std::max)(viewport[2], 1), (std::max)(viewport[3], 1));

	// Draw the scene
	RenderScene(GL_SELECT);

	// Collect the hits
	hits = glRenderMode(GL_RENDER);

	// Restore the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Go back to modelview for normal rendering
	glMatrixMode(GL_MODELVIEW);

	// If hits occured, display the info.
	hits = (std::max)(0, (std::min)(hits, 100));
	if(hits)
	{
		GLuint *ptr, names;
		ptr = (GLuint*) selectBuff;
		for(i=0; i < hits; i++)
		{
			names = *ptr;
			ptr++; ptr++; ptr++;
			for(j = 0; j < names; j++)
			{
				id[i] = *ptr;
				ptr++;
			}
		}
	}

	//::wglMakeCurrent(m_hDC, NULL);

	return hits;
}

