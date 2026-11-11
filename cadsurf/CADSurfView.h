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


// CadSurfView.h : interface of the CCadSurfView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CADSURFVIEW_H__B8A99768_7675_4BA6_B486_6BEE3979CA6B__INCLUDED_)
#define AFX_CADSURFVIEW_H__B8A99768_7675_4BA6_B486_6BEE3979CA6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GLView.h"
#include "HDibApi.h"
#include "StlTransform.h"
#include <vector>

class CCadSurfDoc;
class CCommand;

class CCadSurfView : public CView
{
protected: // create from serialization only
	CCadSurfView();
	DECLARE_DYNCREATE(CCadSurfView)

// Attributes
public:
	CCadSurfDoc* GetDocument();
	CGLView* GetView() const { return myView; }

	bool lbutdown;
	bool mbutdown;
	bool rbutdown;

	CPoint lDownPnt;
	CPoint mDownPnt;
	CPoint rDownPnt;

	int     myXmin;
    int     myYmin;  
    int     myXmax;
    int     myYmax;

	bool winZoom;
	bool dShaded;
	bool antialiased;
	bool gridOn;

	CString coord;
	CString date;
	CString time;
	CString view;

	CPalette*	m_palDIB;
	HDIB		hDIB;

public:
	enum LineStyle { Solid, Dot, ShortDash, LongDash };
	CPen*  m_Pen;
	enum CSaveBMPFlag { BMPRECT = 0, BMPWINDOW, BMPCLIENT };
	CSaveBMPFlag m_nSaveBMPFlag;

    virtual void DrawRectangle (const bool  Draw =true,
                                const LineStyle aLineStyle = Solid  );
private:
    std::vector<unsigned long> SelectedStlIds();
    bool StlSelectionCenter(const std::vector<unsigned long>& targets, StlPosition& center);
    void ApplyStlTransform(const std::vector<unsigned long>& targets,
                           const StlTransform& transform, LPCTSTR action);
    void PickStlRotationAxis(CPoint point);
    void UpdateStlRotationAxisHover(CPoint point);
    void ClearStlRotationAxisFeedback();
    void PickStlMovePoint(CPoint point);
    afx_msg void OnStlMove();
    afx_msg void OnStlMoveByPoints();
    afx_msg void OnStlRotate();
    afx_msg void OnStlScale();
    afx_msg void OnUpdateStlTransform(CCmdUI* command);
    afx_msg void OnStlAutoArrange();
    afx_msg void OnUpdateStlAutoArrange(CCmdUI* command);
    afx_msg void OnStlViewX();
    afx_msg void OnStlViewY();
    afx_msg void OnStlViewZ();
    afx_msg void OnStlViewNeIsometric();
    afx_msg void OnStlViewNwIsometric();
    afx_msg void OnStlViewSeIsometric();
    afx_msg void OnStlViewSwIsometric();

    std::vector<unsigned long> stlAxisTargets;
    double stlAxisAngle = 0.0;
    unsigned long stlAxisHoverId = 0;
    CPoint3D stlAxisVisualStart;
    CPoint3D stlAxisVisualFinish;
    bool stlAxisVisualVisible = false;
    std::vector<unsigned long> stlMoveTargets;
    CPoint3D stlMoveStart;
    bool stlMoveHasStart = false;
	CCommand* m_pCmd; 
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCadSurfView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	void PrintDIB(CDC* pDC);
	void FreeDIB();
	void GetDIB();
	virtual ~CCadSurfView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
private:
	CGLView* myView;
	int m_iSketchPlane; //草图的平面：0-关闭，1-XOY, ...
	int m_iSketchType; //1-直线；2-圆；。。。

// Generated message map functions
protected:
	//{{AFX_MSG(CCadSurfView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnZoomwin();
	afx_msg void OnResetview();
	afx_msg void OnDisplaymode();
	afx_msg void OnUpdateDisplaymode(CCmdUI* pCmdUI);
	afx_msg void OnViewDisplayall();
	afx_msg void OnUpdateViewDisplayall(CCmdUI* pCmdUI);
	afx_msg void OnViewEraseall();
	afx_msg void OnUpdateViewEraseall(CCmdUI* pCmdUI);
	afx_msg void OnViewDisplayselected();
	afx_msg void OnUpdateViewDisplayselected(CCmdUI* pCmdUI);
	afx_msg void OnViewEraseselected();
	afx_msg void OnUpdateViewEraseselected(CCmdUI* pCmdUI);
	afx_msg void OnPreferencesDisplaymodeShaded();
	afx_msg void OnUpdatePreferencesDisplaymodeShaded(CCmdUI* pCmdUI);
	afx_msg void OnPreferencesDisplaymodeWireframe();
	afx_msg void OnUpdatePreferencesDisplaymodeWireframe(CCmdUI* pCmdUI);
	afx_msg void OnPreferencesMaterial();
	afx_msg void OnPreferencesLight();
	afx_msg void OnUpdatePreferencesLight(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCoordViewer(CCmdUI* pCmdUI); //Coordinates in Status Bar
	afx_msg void OnUpdatePreferencesMaterial(CCmdUI* pCmdUI);
	afx_msg void OnPreferencesColor();
	afx_msg void OnUpdatePreferencesColor(CCmdUI* pCmdUI);
	afx_msg void OnPreferencesBackgroundcolor();
	afx_msg void OnHlremoved();
	afx_msg void OnAntialias();
	afx_msg void OnPreferencesLinestyleSolid();
	afx_msg void OnPreferencesLinestyleDotted();
	afx_msg void OnPreferencesLinestyleDashed();
	afx_msg void OnPreferencesLinestyleDasheddotted();
	afx_msg void OnPreferencesGridOn();
	afx_msg void OnUpdatePreferencesGridOn(CCmdUI* pCmdUI);
	afx_msg void OnPreferencesGridSettings();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSnapshot();
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnProjectionType();
	afx_msg void OnUpdateProjectionType(CCmdUI* pCmdUI);
	//}}AFX_MSG
public:
	afx_msg void OnTopview();
	afx_msg void OnBottomview();
	afx_msg void OnFrontview();
	afx_msg void OnBackview();
	afx_msg void OnLeftview();
	afx_msg void OnRightview();
	afx_msg void OnAxonview();
	afx_msg void OnFitall();
	DECLARE_MESSAGE_MAP()
	
	afx_msg void OnSkechLine();
	afx_msg void OnUpdateSkechLine(CCmdUI *pCmdUI);
	afx_msg void OnSketchXoy();
	afx_msg void OnUpdateSketchXoy(CCmdUI *pCmdUI);
	afx_msg void OnSketchXoz();
	afx_msg void OnUpdateSketchXoz(CCmdUI *pCmdUI);
	afx_msg void OnSketchYoz();
	afx_msg void OnSketchClose();
	afx_msg void OnUpdateSketchYoz(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSketchClose(CCmdUI *pCmdUI);
	afx_msg void OnSkechCircle();
	afx_msg void OnUpdateSkechCircle(CCmdUI *pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in CadSurfView.cpp
inline CCadSurfDoc* CCadSurfView::GetDocument()
   { return (CCadSurfDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CADSURFVIEW_H__B8A99768_7675_4BA6_B486_6BEE3979CA6B__INCLUDED_)
