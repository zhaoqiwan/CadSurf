//############################################################################

//  Author:	Sharjith Nair
//  Email:	sharjith_ssn@hotmail.com

// Program:	OpenGL, C++ MFC Surface Modeler Demo Application

//							Important Notice:

//	This Idea and the Application is Copyright(c) Sharjith Nair 2002, 2003.
//	You can freely use it as long as you credit me for it.

//	No guarantee/warranty is given on this app and I will not be responsible 
//	for any damage to you, your property or any other person from using it.
//	USE IT ON YOUR OWN RISK.

//	Thankyou
//	Sharjith Nair.

//#############################################################################

#if !defined(AFX_GLVIEW_H__859711F8_BE5E_4EFB_9EAB_84708FB5E343__INCLUDED_)
#define AFX_GLVIEW_H__859711F8_BE5E_4EFB_9EAB_84708FB5E343__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GLView.h : header file
//

#include "GLDisplayContext.h"
#include "GLCamera.h"
#include "GLObject.h"
#include "GLTrihedron.h"
#include "GLGrid.h"
#include "Light.h"

#include "Point3D.h"

#define NUM_COLORS (sizeof(colorv) / sizeof(colorv[0]))

/////////////////////////////////////////////////////////////////////////////
// CGLView view

class CGLDisplayContext;
class CGLView
{
public:
	enum GLProjectionType {GLORTHOGRAPHIC, GLPERSPECTIVE};
public:
	CGLView(CWnd* pWnd, CGLDisplayContext* aContext);
	~CGLView();

	void AddEntity(CGLObject *);
	void RemoveEntity(CGLObject *);

// Attributes
public:
	HGLRC m_hRC;			// Rendering Context
	HDC m_hDC;				// Device Context
	CPalette m_GLPalette;	// Logical Palette
	HPALETTE	m_hPalette,
				m_hPalette2;

private:
	GLdouble nRange;
	GLdouble xRot;
	GLdouble yRot;
	GLdouble zRot;
	GLdouble xTrans;
	GLdouble yTrans;
	GLdouble zTrans;
	GLfloat m_bkRed;
	GLfloat m_bkGreen;
	GLfloat m_bkBlue;
	GLDisplayMode displayMode;
	bool m_antialias;

	CGLTrihedron *myTrihedron;
	CGLGrid* myGrid;

	// Color Indexes
	struct colorIndexState 
	{
		GLfloat amb[3];		/* ambient color / bottom of ramp */
		GLfloat diff[3];	/* diffuse color / middle of ramp */
		GLfloat spec[3];	/* specular color / top of ramp */
		GLfloat ratio;		/* ratio of diffuse to specular in ramp */
		GLint indexes[3];	/* where ramp was placed in palette */
	};

	colorIndexState colorv[3];

	CPoint DownPoint;
	bool ZoomON;
	bool PanON;
	bool RotON;
	//CLight light[10];
	bool gridOn;

// Operations
public:
	CLight light[10];
	void InitGL();
	BOOL EnableRC(HDC m_hDC, HGLRC m_hRC, BOOL bEnable);
	void RenderScene(GLenum mode);
	void InitializePalette(void);

	void ZoomView(CPoint, CPoint);
	void PanView(CPoint, CPoint);
	void RotateView(CPoint, CPoint);

	void StopZooming(){ ZoomON = false; }
	void StopPanning(){ PanON = false; }
	void StopRotation(){ RotON = false; }

	void TopView();
	void BottomView();
	void FrontView();
	void BackView();
	void LeftView();
	void RightView();
	void AxonView();
	void IsometricView(double zRotation);

	GLViewType GetViewType() const { return myViewType; }

	void SetProjectionType(const GLProjectionType& aType);
	GLProjectionType GetProjectionType() const { return myProjType; }


	void FitAll();
	void ZoomWindow(CRect zcRect);
	void ResetView();

	void PointToScreen(const CPoint3D& P, int& x, int& y);
	bool PickReferencePoint(CPoint cursor, CPoint3D& result);
	void DrawReferenceMarker(const CPoint3D& point);
	void DrawReferenceLine(const CPoint3D& start, const CPoint3D& finish);
	void DrawOrientationIndicator() const;
	void ScreenToPoint(const int& x, const int& y, CPoint3D&  P);
	void FitView();
	void Fit3d(CRect zcRect);

	void SetBackgroundColor(GLfloat, GLfloat, GLfloat);
	void GetBackgroundColor(GLfloat&, GLfloat&, GLfloat&);
	void SetDisplayMode(const GLDisplayMode& aMode);

	void SetLight(CLight light1[],int n);

	void SetAntiAliasing(const bool& mode);

	void SetGridOn(const bool& mode);
	void SetGridPara(const CPlane& plane, const double& size, const double& step);

	void Select(const int& x, const int& y);
	void MultiSelect(const int& x, const int& y);
	void SweepSelect(const CRect& swRect);

	int  ProcessSelection(const int& x, const int& y, const int& sensitivity = 5,
	                      bool straightLinesOnly = false);
	int  ProcessSweepSelection(const CRect& rect, unsigned long int* id);


	virtual void Draw(int ntype = 0);
	void ReSize(int cx, int cy);
	void Refresh();

	BOOL QueryNewPalette();
	void PaletteChanged();

	void MakeResource();
	void ClearResource();
	void SwapBuffers();

private:
	void ApplyProjection(int width, int height) const;
	void ApplyViewTransform() const;
	CWnd* ptrWnd;
	CGLDisplayContext* theContext;
	CGLCamera* myCamera;
	GLViewType myViewType;
	GLProjectionType myProjType;
};


#endif // !defined(AFX_GLVIEW_H__859711F8_BE5E_4EFB_9EAB_84708FB5E343__INCLUDED_)
