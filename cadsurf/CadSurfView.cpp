// CadSurfView.cpp : implementation of the CCadSurfView class
//

#include "stdafx.h"
#include "CadSurf.h"

#include "CadSurfDoc.h"
#include "CadSurfView.h"
 #include "ChildFrm.h" 
#include "Light.h"


#include "GLView.h"
#include "COMMAND.H"
#include "CREATECMD.H"
#include "GLStlMesh.h"

#include <algorithm>
#include <utility>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCadSurfView

IMPLEMENT_DYNCREATE(CCadSurfView, CView)

BEGIN_MESSAGE_MAP(CCadSurfView, CView)
	//{{AFX_MSG_MAP(CCadSurfView)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_STL_MOVE, &CCadSurfView::OnStlMove)
	ON_COMMAND(ID_STL_MOVE_BY_POINTS, &CCadSurfView::OnStlMoveByPoints)
	ON_COMMAND(ID_STL_ROTATE, &CCadSurfView::OnStlRotate)
	ON_COMMAND(ID_STL_SCALE, &CCadSurfView::OnStlScale)
	ON_COMMAND(ID_STL_AUTO_ARRANGE, &CCadSurfView::OnStlAutoArrange)
	ON_COMMAND(ID_STL_VIEW_X, &CCadSurfView::OnStlViewX)
	ON_COMMAND(ID_STL_VIEW_Y, &CCadSurfView::OnStlViewY)
	ON_COMMAND(ID_STL_VIEW_Z, &CCadSurfView::OnStlViewZ)
	ON_COMMAND(ID_STL_VIEW_NE_ISO, &CCadSurfView::OnStlViewNeIsometric)
	ON_COMMAND(ID_STL_VIEW_NW_ISO, &CCadSurfView::OnStlViewNwIsometric)
	ON_COMMAND(ID_STL_VIEW_SE_ISO, &CCadSurfView::OnStlViewSeIsometric)
	ON_COMMAND(ID_STL_VIEW_SW_ISO, &CCadSurfView::OnStlViewSwIsometric)
	ON_UPDATE_COMMAND_UI(ID_STL_MOVE, &CCadSurfView::OnUpdateStlTransform)
	ON_UPDATE_COMMAND_UI(ID_STL_MOVE_BY_POINTS, &CCadSurfView::OnUpdateStlTransform)
	ON_UPDATE_COMMAND_UI(ID_STL_ROTATE, &CCadSurfView::OnUpdateStlTransform)
	ON_UPDATE_COMMAND_UI(ID_STL_SCALE, &CCadSurfView::OnUpdateStlTransform)
	ON_UPDATE_COMMAND_UI(ID_STL_AUTO_ARRANGE, &CCadSurfView::OnUpdateStlAutoArrange)
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_ZOOMWIN, OnZoomwin)
	ON_COMMAND(ID_RESETVIEW, OnResetview)
	ON_COMMAND(ID_DISPLAYMODE, OnDisplaymode)
	ON_UPDATE_COMMAND_UI(ID_DISPLAYMODE, OnUpdateDisplaymode)
	ON_COMMAND(ID_VIEW_DISPLAYALL, OnViewDisplayall)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DISPLAYALL, OnUpdateViewDisplayall)
	ON_COMMAND(ID_VIEW_ERASEALL, OnViewEraseall)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ERASEALL, OnUpdateViewEraseall)
	ON_COMMAND(ID_VIEW_DISPLAYSELECTED, OnViewDisplayselected)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DISPLAYSELECTED, OnUpdateViewDisplayselected)
	ON_COMMAND(ID_VIEW_ERASESELECTED, OnViewEraseselected)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ERASESELECTED, OnUpdateViewEraseselected)
	ON_COMMAND(ID_PREFERENCES_DISPLAYMODE_SHADED, OnPreferencesDisplaymodeShaded)
	ON_UPDATE_COMMAND_UI(ID_PREFERENCES_DISPLAYMODE_SHADED, OnUpdatePreferencesDisplaymodeShaded)
	ON_COMMAND(ID_PREFERENCES_DISPLAYMODE_WIREFRAME, OnPreferencesDisplaymodeWireframe)
	ON_UPDATE_COMMAND_UI(ID_PREFERENCES_DISPLAYMODE_WIREFRAME, OnUpdatePreferencesDisplaymodeWireframe)
	ON_COMMAND(ID_PREFERENCES_MATERIAL, OnPreferencesMaterial)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_COORD, OnUpdateCoordViewer)
	ON_UPDATE_COMMAND_UI(ID_PREFERENCES_MATERIAL, OnUpdatePreferencesMaterial)
	ON_COMMAND(ID_PREFERENCES_COLOR, OnPreferencesColor)
	ON_UPDATE_COMMAND_UI(ID_PREFERENCES_COLOR, OnUpdatePreferencesColor)
	ON_COMMAND(ID_PREFERENCES_BACKGROUNDCOLOR, OnPreferencesBackgroundcolor)
	ON_COMMAND(ID_HLREMOVED, OnHlremoved)
	ON_COMMAND(ID_ANTIALIAS, OnAntialias)
	ON_COMMAND(ID_PREFERENCES_LINESTYLE_SOLID, OnPreferencesLinestyleSolid)
	ON_COMMAND(ID_PREFERENCES_LINESTYLE_DOTTED, OnPreferencesLinestyleDotted)
	ON_COMMAND(ID_PREFERENCES_LINESTYLE_DASHED, OnPreferencesLinestyleDashed)
	ON_COMMAND(ID_PREFERENCES_LINESTYLE_DASHEDDOTTED, OnPreferencesLinestyleDasheddotted)
	ON_COMMAND(ID_PREFERENCES_GRID_ON, OnPreferencesGridOn)
	ON_UPDATE_COMMAND_UI(ID_PREFERENCES_GRID_ON, OnUpdatePreferencesGridOn)
	ON_COMMAND(ID_PREFERENCES_GRID_SETTINGS, OnPreferencesGridSettings)
	//ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_SNAPSHOT, OnSnapshot)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_TOPVIEW, OnTopview)
	ON_COMMAND(ID_BOTTOMVIEW, OnBottomview)
	ON_COMMAND(ID_FRONTVIEW, OnFrontview)
	ON_COMMAND(ID_BACKVIEW, OnBackview)
	ON_COMMAND(ID_LEFTVIEW, OnLeftview)
	ON_COMMAND(ID_RIGHTVIEW, OnRightview)
	ON_COMMAND(ID_AXONVIEW, OnAxonview)
	ON_COMMAND(ID_FITALL, OnFitall)
	ON_COMMAND(ID_PROJECTION_TYPE, OnProjectionType)
	ON_UPDATE_COMMAND_UI(ID_PROJECTION_TYPE, OnUpdateProjectionType)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_PREFERENCES_LIGHT, OnPreferencesLight)
	ON_UPDATE_COMMAND_UI(ID_PREFERENCES_LIGHT, OnUpdatePreferencesLight)
	ON_COMMAND(ID_SKECH_LINE, &CCadSurfView::OnSkechLine)
	ON_UPDATE_COMMAND_UI(ID_SKECH_LINE, &CCadSurfView::OnUpdateSkechLine)
	ON_COMMAND(ID_SKETCH_XOY, &CCadSurfView::OnSketchXoy)
	ON_UPDATE_COMMAND_UI(ID_SKETCH_XOY, &CCadSurfView::OnUpdateSketchXoy)
	ON_COMMAND(ID_SKETCH_XOZ, &CCadSurfView::OnSketchXoz)
	ON_UPDATE_COMMAND_UI(ID_SKETCH_XOZ, &CCadSurfView::OnUpdateSketchXoz)
	ON_COMMAND(ID_SKETCH_YOZ, &CCadSurfView::OnSketchYoz)
	ON_COMMAND(ID_SKETCH_CLOSE, &CCadSurfView::OnSketchClose)
	ON_UPDATE_COMMAND_UI(ID_SKETCH_YOZ, &CCadSurfView::OnUpdateSketchYoz)
	ON_UPDATE_COMMAND_UI(ID_SKETCH_CLOSE, &CCadSurfView::OnUpdateSketchClose)
	ON_COMMAND(ID_SKECH_CIRCLE, &CCadSurfView::OnSkechCircle)
	ON_UPDATE_COMMAND_UI(ID_SKECH_CIRCLE, &CCadSurfView::OnUpdateSkechCircle)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCadSurfView construction/destruction

CCadSurfView::CCadSurfView()
{
	m_iSketchPlane = 0;
	m_iSketchType = 0; //什么都不做;

	// TODO: add construction code here
	lbutdown = false;
	mbutdown = false;
	rbutdown = false;
	winZoom = false;
	dShaded = true;
	antialiased = false;
	gridOn = false;
	coord.Format("");
	view.Format(_T("AXO"));
	m_Pen = new CPen(PS_SOLID, 2, RGB(0,0,0));

	hDIB	   = NULL;
	m_palDIB   = new CPalette;
	m_nSaveBMPFlag = BMPCLIENT;

	m_pCmd = NULL;
}

CCadSurfView::~CCadSurfView()
{
	if( m_pCmd )
		delete m_pCmd ;

	delete m_Pen;

	//printing
	if( hDIB != NULL )
    {
        ::GlobalUnlock(hDIB);
	    ::GlobalFree(hDIB);
	    hDIB = NULL;
	}  
	if (m_palDIB != NULL)
	{
		delete m_palDIB;
		m_palDIB = NULL;
	}
}

BOOL CCadSurfView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

int CCadSurfView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	myView = new CGLView(this, GetDocument()->dContext);
	return 0;
}

void CCadSurfView::OnDestroy() 
{
	if (m_pCmd)
	{
		m_pCmd->Cancel();
		delete m_pCmd;
		m_pCmd = NULL;
	}
	delete myView;
	myView = NULL;
	CView::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////
// CCadSurfView drawing

void CCadSurfView::OnDraw(CDC* pDC)
{
	CCadSurfDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	if (pDC->IsPrinting()) 
		PrintDIB(pDC);
	else
	{
		myView->MakeResource();
		::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
		myView->Draw();
		if (!stlMoveTargets.empty() && stlMoveHasStart)
			myView->DrawReferenceMarker(stlMoveStart);
		if (stlAxisVisualVisible)
			myView->DrawReferenceLine(stlAxisVisualStart, stlAxisVisualFinish);
		myView->DrawOrientationIndicator();
		myView->SwapBuffers();
		//GDI
		pDC->SetBkMode(0);
		pDC->SetTextColor(RGB(255,255,0));
		pDC->TextOut(1,1, view);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCadSurfView printing

BOOL CCadSurfView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	pInfo->SetMaxPage(1);
	m_nSaveBMPFlag = BMPCLIENT;
	return DoPreparePrinting(pInfo);
}

void CCadSurfView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCadSurfView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCadSurfView diagnostics

#ifdef _DEBUG
void CCadSurfView::AssertValid() const
{
	CView::AssertValid();
}

void CCadSurfView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCadSurfDoc* CCadSurfView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCadSurfDoc)));
	return (CCadSurfDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCadSurfView message handlers

void CCadSurfView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	SetTimer(1,1000,NULL);
	CRect client;
	GetClientRect(&client);
	if (!GetDocument()->dContext->IsAllErased() && client.Width() > 0 && client.Height() > 0)
	{
		myView->MakeResource();
		myView->ReSize(client.Width(), client.Height());
		myView->FitAll();
	}
}

void CCadSurfView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(myView)
		myView->ReSize(cx, cy);
}

void CCadSurfView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CView::OnTimer(nIDEvent);
}

void CCadSurfView::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (!stlMoveTargets.empty())
    {
        PickStlMovePoint(point);
        return;
    }

    if (!stlAxisTargets.empty())
    {
        PickStlRotationAxis(point);
        return;
    }

    if (stlAxisVisualVisible)
    {
        ClearStlRotationAxisFeedback();
        Invalidate(FALSE);
    }

	// TODO: Add your message handler code here and/or call default

	// ask window to give us events even if outside the view
    SetCapture();

	//如果草图绘制;
	if (m_pCmd)
	{
		CPoint3D pt3d;
		myView->ScreenToPoint(point.x, point.y, pt3d);
// 		//处理一下，根据视图;
// 		if (myView->GetViewType() == GLFRONTVIEW) //FRONT
// 		{
// 			pt3d.SetY(pt3d.GetZ());
// 		}
// 		else if (myView->GetViewType() == GLLEFTVIEW) //LEFT
// 		{
// 			pt3d.SetX(pt3d.GetZ());
// 		}

		m_pCmd->OnLButtonDown(nFlags, pt3d);
		CView::OnLButtonDown(nFlags, point);
		return;
	}

	//  save the current mouse coordinate in min 
	myXmin=point.x;  myYmin=point.y;
	myXmax=point.x;  myYmax=point.y;

	lbutdown = true;
	lDownPnt = point;
	if(!(nFlags & MK_SHIFT) && !(nFlags & MK_CONTROL) && !winZoom)
	{
		myView->Select(point.x, point.y);
		InvalidateRect(NULL, FALSE);
	}
	if ((nFlags & MK_SHIFT) && !(nFlags & MK_CONTROL) && !winZoom)
	{
		myView->MultiSelect(point.x, point.y);
		InvalidateRect(NULL, FALSE);
	}
	CView::OnLButtonDown(nFlags, point);
}

void CCadSurfView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_pCmd)
	{
		ReleaseCapture();

		CView::OnLButtonUp(nFlags, point);
		return;
	}
	// TODO: Add your message handler code here and/or call default
	if (!lbutdown)
	{
		CView::OnLButtonUp(nFlags, point);
		return;
	}
	lbutdown = false;
	myView->StopZooming();

	if(!(nFlags & MK_SHIFT) && !(nFlags & MK_CONTROL) && !winZoom)
	{
		DrawRectangle(false);
		CRect rect(lDownPnt, point);
		rect.NormalizeRect();
		if(rect.Width() && rect.Height())
			myView->SweepSelect(rect);
		InvalidateRect(NULL, FALSE);
	}

	if(winZoom)
	{
		myXmax=point.x;  myYmax=point.y;
		DrawRectangle(false);
		CRect myZoomRect(myXmin, myYmax, myXmax, myYmin);
		if ((abs(myXmin-myXmax)>1) || (abs(myYmin-myYmax)>1))
		// Test if the zoom window size is not null
		{
			myView->ZoomWindow(myZoomRect);
		}
	}

	winZoom = false;
	ReleaseCapture();

	CView::OnLButtonUp(nFlags, point);
}

void CCadSurfView::OnMButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	mbutdown = true;
	mDownPnt = point;
	CView::OnMButtonDown(nFlags, point);
}

void CCadSurfView::OnMButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	mbutdown = false;
	myView->StopPanning();
	CView::OnMButtonUp(nFlags, point);
}

void CCadSurfView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (m_pCmd)
	{
		CPoint3D pt3d;
		myView->ScreenToPoint(point.x, point.y, pt3d);
		m_pCmd->OnRButtonDown(nFlags, pt3d);
		delete m_pCmd;
		m_pCmd = NULL;
		m_iSketchType = 0;
		if (GetCapture() == this)
			ReleaseCapture();
		Invalidate(FALSE);
		::Prompt("Sketch line command ended.");
		return;
	}

	// TODO: Add your message handler code here and/or call default
	rbutdown = true;
	rDownPnt = point;
	CView::OnRButtonDown(nFlags, point);
}

void CCadSurfView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	rbutdown = false;
	myView->StopRotation();
	CView::OnRButtonUp(nFlags, point);
}

void CCadSurfView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!stlAxisTargets.empty())
	{
		UpdateStlRotationAxisHover(point);
		CView::OnMouseMove(nFlags, point);
		return;
	}

	if (m_pCmd)
	{
		CPoint3D pt3d;
		myView->ScreenToPoint(point.x, point.y, pt3d);
// 		//处理一下，根据视图;
// 		if (myView->GetViewType() == GLFRONTVIEW) //FRONT
// 		{
// 			pt3d.SetY(pt3d.GetZ());
// 		}
// 		else if (myView->GetViewType() == GLLEFTVIEW) //LEFT
// 		{
// 			pt3d.SetX(pt3d.GetZ());
// 		}
		m_pCmd->OnMouseMove(nFlags, pt3d);
		CView::OnMouseMove(nFlags, point);
		return;
	}

	// TODO: Add your message handler code here and/or call default
	if(lbutdown && (nFlags & MK_CONTROL))
	{
		myView->ZoomView(lDownPnt, point);
		Invalidate(FALSE);
	}
	if(mbutdown && (nFlags & MK_CONTROL))
	{
		myView->PanView(mDownPnt, point);
		Invalidate(FALSE);
	}
	if(rbutdown && (nFlags & MK_CONTROL))
	{
		myView->RotateView(rDownPnt, point);
		if (myView->GetViewType() == GLUNDEFINEDVIEW)
			view = _T("FREE");
		Invalidate(FALSE);
	}
	if(lbutdown && !(nFlags & MK_CONTROL) && !(nFlags & MK_SHIFT))
	{
		myXmax = point.x; myYmax = point.y;	
		DrawRectangle(true,LongDash);
	}

	CPoint3D C;
	myView->ScreenToPoint(point.x, point.y, C);
	coord.Format("X %0.3lf  Y %0.3lf  Z%0.3lf",C.GetX(), C.GetY(), C.GetZ());

	CView::OnMouseMove(nFlags, point);
}

BOOL CCadSurfView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (!stlAxisTargets.empty() && nHitTest == HTCLIENT)
	{
		::SetCursor(::LoadCursor(NULL, IDC_CROSS));
		return TRUE;
	}
	return CView::OnSetCursor(pWnd, nHitTest, message);
}



void CCadSurfView::OnTopview() 
{
	// TODO: Add your command handler code here
	myView->TopView();
	view.Format(_T("TOP"));
}

void CCadSurfView::OnBottomview() 
{
	// TODO: Add your command handler code here
	myView->BottomView();
	view.Format(_T("BOTTOM"));
}

void CCadSurfView::OnFrontview() 
{
	// TODO: Add your command handler code here
	myView->FrontView();
	view.Format(_T("FRONT"));
}

void CCadSurfView::OnBackview() 
{
	// TODO: Add your command handler code here
	myView->BackView();
	view.Format(_T("BACK"));
}

void CCadSurfView::OnLeftview() 
{
	// TODO: Add your command handler code here
	myView->LeftView();
	view.Format(_T("LEFT"));
}

void CCadSurfView::OnRightview() 
{
	// TODO: Add your command handler code here
	myView->RightView();
	view.Format(_T("RIGHT"));
}

void CCadSurfView::OnAxonview() 
{
	// TODO: Add your command handler code here
	myView->AxonView();
	view.Format(_T("AXO"));
}

void CCadSurfView::OnFitall() 
{
	// TODO: Add your command handler code here
	myView->FitAll();
}

void CCadSurfView::OnZoomwin() 
{
	// TODO: Add your command handler code here
	winZoom = true;
}

void CCadSurfView::OnResetview() 
{
	// TODO: Add your command handler code here
	myView->ResetView();
	view.Format(_T("AXO"));
}


void CCadSurfView::OnDisplaymode() 
{
	// TODO: Add your command handler code here
	dShaded = !dShaded;
	if(dShaded)
	{
		POSITION pos = GetDocument()->GetFirstViewPosition();
		while(pos != NULL)
		{
			CCadSurfView* view = (CCadSurfView*) GetDocument()->GetNextView(pos);
			view->GetView()->SetDisplayMode(GLSHADED);
		}
	}
	else
	{
		POSITION pos = GetDocument()->GetFirstViewPosition();
		while(pos != NULL)
		{
			CCadSurfView* view = (CCadSurfView*) GetDocument()->GetNextView(pos);
			view->GetView()->SetDisplayMode(GLWIREFRAME);
		}
	}
}

void CCadSurfView::OnUpdateDisplaymode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
}

void CCadSurfView::OnHlremoved() 
{
	// TODO: Add your command handler code here
	POSITION pos = GetDocument()->GetFirstViewPosition();
	while(pos != NULL)
	{
		CCadSurfView* view = (CCadSurfView*) GetDocument()->GetNextView(pos);
		view->GetView()->SetDisplayMode(GLHLREMOVED);
	}
}

void CCadSurfView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	POSITION pos = GetDocument()->GetFirstViewPosition();
	while(pos != NULL)
	{
		CCadSurfView* view = (CCadSurfView*) GetDocument()->GetNextView(pos);
		view->GetView()->Refresh();
	}
	CView::OnUpdate(pSender, lHint, pHint);

}


void CCadSurfView::OnViewDisplayall() 
{
	auto* doc = GetDocument();
	if (!doc->dContext->ObjectsOfType(GLSTLMESH, true).empty())
		doc->SetModifiedFlag();
	doc->dContext->DisplayAll();
	doc->UpdateAllViews(nullptr);
}

void CCadSurfView::OnUpdateViewDisplayall(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!GetDocument()->dContext->IsAllDisplayed());
}

void CCadSurfView::OnViewEraseall() 
{
	auto* doc = GetDocument();
	if (!doc->dContext->ObjectsOfType(GLSTLMESH).empty())
		doc->SetModifiedFlag();
	doc->dContext->EraseAll();
	doc->UpdateAllViews(nullptr);
}

void CCadSurfView::OnUpdateViewEraseall(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!GetDocument()->dContext->IsAllErased());
}

void CCadSurfView::OnViewDisplayselected() 
{
	auto* doc = GetDocument();
	if (doc->dContext->HasSelected() && !doc->dContext->ObjectsOfType(GLSTLMESH).empty())
		doc->SetModifiedFlag();
	doc->dContext->DisplaySelected();
	doc->UpdateAllViews(nullptr);
}

void CCadSurfView::OnUpdateViewDisplayselected(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CGLDisplayContext* ctx = GetDocument()->dContext;
	pCmdUI->Enable(ctx->HasSelected());
}

void CCadSurfView::OnViewEraseselected() 
{
	auto* doc = GetDocument();
	if (!SelectedStlIds().empty())
		doc->SetModifiedFlag();
	doc->dContext->EraseSelected();
	doc->UpdateAllViews(nullptr);
}

void CCadSurfView::OnUpdateViewEraseselected(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CGLDisplayContext* ctx = GetDocument()->dContext;
	pCmdUI->Enable(ctx->HasSelected());
}

void CCadSurfView::OnUpdateCoordViewer(CCmdUI* pCmdUI)
{
	if(myView->GetViewType() == GLAXONVIEW || myView->GetViewType() == GLUNDEFINEDVIEW)
		pCmdUI->SetText(_T(""));
	else
		pCmdUI->SetText((LPCTSTR)coord);
}

void CCadSurfView::OnPreferencesDisplaymodeShaded() 
{
	// TODO: Add your command handler code here
	dShaded = true;
	POSITION pos = GetDocument()->GetFirstViewPosition();
	while(pos != NULL)
	{
		CCadSurfView* view = (CCadSurfView*) GetDocument()->GetNextView(pos);
		view->GetView()->SetDisplayMode(GLSHADED);
	}		
}

void CCadSurfView::OnPreferencesDisplaymodeWireframe() 
{
	// TODO: Add your command handler code here
	dShaded = false;
	POSITION pos = GetDocument()->GetFirstViewPosition();
	while(pos != NULL)
	{
		CCadSurfView* view = (CCadSurfView*) GetDocument()->GetNextView(pos);
		view->GetView()->SetDisplayMode(GLWIREFRAME);
	}
}

void CCadSurfView::OnUpdatePreferencesDisplaymodeWireframe(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(!dShaded);
}

void CCadSurfView::OnAntialias() 
{
	// TODO: Add your command handler code here
	antialiased = !antialiased;
	POSITION pos = GetDocument()->GetFirstViewPosition();
	while(pos != NULL)
	{
		CCadSurfView* view = (CCadSurfView*) GetDocument()->GetNextView(pos);
		view->GetView()->SetAntiAliasing(antialiased);
	}
}

#include "MaterialDlg.h"
void CCadSurfView::OnPreferencesMaterial() 
{
	// TODO: Add your command handler code here
	CMaterialDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CGLDisplayContext* ctx = GetDocument()->dContext;
		for(ctx->InitSelected(); ctx->MoreSelected(); ctx->NextSelected())
		{
			CGLObject* O = ctx->CurrentSelected();
			O->SetLight(dlg.m_Matl);
		}
		InvalidateRect(NULL, FALSE);
		GetDocument()->UpdateAllViews(NULL);
	}
}

void CCadSurfView::OnUpdatePreferencesMaterial(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CGLDisplayContext* ctx = GetDocument()->dContext;
	pCmdUI->Enable(ctx->HasSelected() && dShaded);
}

void CCadSurfView::OnPreferencesColor() 
{
	// TODO: Add your command handler code here
	CColorDialog dlg;
	dlg.m_cc.Flags |= CC_RGBINIT;
	dlg.m_cc.rgbResult = RGB(255, 0, 0);
	if(dlg.DoModal()==IDOK)
	{
		COLORREF color = dlg.GetColor();
		GLubyte m_Red = (GLubyte) GetRValue(color);
		GLubyte m_Green = (GLubyte) GetGValue(color);
		GLubyte m_Blue = (GLubyte) GetBValue(color);
		CGLDisplayContext* ctx = GetDocument()->dContext;
		for(ctx->InitSelected(); ctx->MoreSelected(); ctx->NextSelected())
		{
			CGLObject* O = ctx->CurrentSelected();
			if (O)
			{
				O->SetColor(m_Red, m_Green, m_Blue);
				if (O->IsOfType(GLSTLMESH))
					GetDocument()->SetModifiedFlag();
			}
		}
		InvalidateRect(NULL, FALSE);
		GetDocument()->UpdateAllViews(NULL);
	}
}

void CCadSurfView::OnUpdatePreferencesColor(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CGLDisplayContext* ctx = GetDocument()->dContext;
	pCmdUI->Enable(ctx->HasSelected());
}

void CCadSurfView::OnPreferencesLinestyleSolid() 
{
	// TODO: Add your command handler code here
	CGLDisplayContext* ctx = GetDocument()->dContext;
	for(ctx->InitSelected(); ctx->MoreSelected(); ctx->NextSelected())
	{
		CGLObject* O = ctx->CurrentSelected();
		if(O->IsOfType(GLCURVE))
		{
			CGLCurve* crv = static_cast<CGLCurve*>(O);
			crv->SetLineStyle(LINE_SOLID);
		}
	}
	InvalidateRect(NULL, FALSE);
}

void CCadSurfView::OnPreferencesLinestyleDotted() 
{
	// TODO: Add your command handler code here
	CGLDisplayContext* ctx = GetDocument()->dContext;
	for(ctx->InitSelected(); ctx->MoreSelected(); ctx->NextSelected())
	{
		CGLObject* O = ctx->CurrentSelected();
		if(O->IsOfType(GLCURVE))
		{
			CGLCurve* crv = static_cast<CGLCurve*>(O);
			crv->SetLineStyle(LINE_DOTTED);
		}
	}
	InvalidateRect(NULL, FALSE);
}

void CCadSurfView::OnPreferencesLinestyleDashed() 
{
	// TODO: Add your command handler code here
	CGLDisplayContext* ctx = GetDocument()->dContext;
	for(ctx->InitSelected(); ctx->MoreSelected(); ctx->NextSelected())
	{
		CGLObject* O = ctx->CurrentSelected();
		if(O->IsOfType(GLCURVE))
		{
			CGLCurve* crv = static_cast<CGLCurve*>(O);
			crv->SetLineStyle(LINE_DASHED);
		}
	}
	InvalidateRect(NULL, FALSE);
}

void CCadSurfView::OnPreferencesLinestyleDasheddotted() 
{
	// TODO: Add your command handler code here
	CGLDisplayContext* ctx = GetDocument()->dContext;
	for(ctx->InitSelected(); ctx->MoreSelected(); ctx->NextSelected())
	{
		CGLObject* O = ctx->CurrentSelected();
		if(O->IsOfType(GLCURVE))
		{
			CGLCurve* crv = static_cast<CGLCurve*>(O);
			crv->SetLineStyle(LINE_DASHDOTTED);
		}
	}
	InvalidateRect(NULL, FALSE);
}

void CCadSurfView::OnUpdatePreferencesDisplaymodeShaded(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(dShaded);
}

void CCadSurfView::OnPreferencesGridOn() 
{
	// TODO: Add your command handler code here
	gridOn = !gridOn;
	myView->SetGridOn(gridOn);
}

void CCadSurfView::OnUpdatePreferencesGridOn(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(!gridOn)
	{
		pCmdUI->SetText((LPCTSTR)_T("ON"));
	}
	else
	{
		pCmdUI->SetText((LPCTSTR)_T("OFF"));
	}
	
}

#include "GridDlg.h"
void CCadSurfView::OnPreferencesGridSettings() 
{
	// TODO: Add your command handler code here
	CGridDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		//GLfloat x,y,z;
		//x=dlg.m_x;y=dlg.m_y;z=dlg.m_z;

		double dx, dy, dz, size, step;
		//step=dlg.m_x;size=dlg.m_y;dz=dlg.m_z;
		dx = dlg.m_DX;
		dy = dlg.m_DY;
		dz = dlg.m_DZ;
		size = dlg.m_Size;
		step = dlg.m_Step;
		dy=60 ;dx=100;
		if(dx == 0 && dy == 0 && dz == 0)
			dz=1;
		CPlane plane(dx, dy, dz, 0);
		myView->SetGridPara(plane, size, step);
	}
}

void CCadSurfView::OnPreferencesBackgroundcolor() 
{
	// TODO: Add your command handler code here
	CColorDialog dlg;
	dlg.m_cc.Flags |= CC_RGBINIT;
	dlg.m_cc.rgbResult = RGB(0, 0, 0);
	if(dlg.DoModal()==IDOK)
	{
		COLORREF color = dlg.GetColor();
		GLfloat m_Red = (GLfloat) GetRValue(color);
		GLfloat m_Green = (GLfloat) GetGValue(color);
		GLfloat m_Blue = (GLfloat) GetBValue(color);

		POSITION pos = GetDocument()->GetFirstViewPosition();
		while(pos != NULL)
		{
			CCadSurfView* view = (CCadSurfView*) GetDocument()->GetNextView(pos);
			view->GetView()->SetBackgroundColor(m_Red/255, m_Green/255, m_Blue/255);
		}
	}	
}

void CCadSurfView::DrawRectangle(const bool  Draw , 
                                        const LineStyle aLineStyle)
{
    static int m_DrawMode=R2_MERGEPENNOT;
    static		int StoredMinX, StoredMaxX, StoredMinY, StoredMaxY;
    static		bool m_IsVisible = false;
    static      int  StoredLineStyle= aLineStyle;

    CPen* aOldPen;
    CClientDC clientDC(this);
    if (m_Pen) aOldPen = clientDC.SelectObject(m_Pen);
    clientDC.SetROP2(m_DrawMode);

    if ( m_IsVisible ) //  erase at the old position 
    {
     clientDC.MoveTo(StoredMinX,StoredMinY); clientDC.LineTo(StoredMinX,StoredMaxY); 
     clientDC.LineTo(StoredMaxX,StoredMaxY); 
	 clientDC.LineTo(StoredMaxX,StoredMinY); clientDC.LineTo(StoredMinX,StoredMinY);
     m_IsVisible = false;
    }

    StoredMinX = min ( myXmin, myXmax );    StoredMinY = min ( myYmin, myYmax );
    StoredMaxX = max ( myXmin, myXmax );    StoredMaxY = max ( myYmin, myYmax);

    if (Draw){ // move : draw
        if (StoredLineStyle != aLineStyle) {
            delete m_Pen;
            if  (aLineStyle ==Solid )
                {m_Pen = new CPen(PS_SOLID, 2, RGB(0,0,0)); m_DrawMode = R2_MERGEPENNOT;}
            else if (aLineStyle ==Dot )
                {m_Pen = new CPen(PS_DOT, 1, RGB(0,0,0));   m_DrawMode = R2_XORPEN;}
            else if (aLineStyle == ShortDash)
                {m_Pen = new CPen(PS_DASH, 1, RGB(255,0,0));	m_DrawMode = R2_XORPEN;}
            else if (aLineStyle == LongDash)
                {m_Pen = new CPen(PS_DASH, 1, RGB(0,0,0));	m_DrawMode = R2_NOTXORPEN;}
            clientDC.SelectObject(m_Pen);
        }

     clientDC.SetROP2(m_DrawMode);
     clientDC.MoveTo(StoredMinX,StoredMinY); clientDC.LineTo(StoredMinX,StoredMaxY); 
     clientDC.LineTo(StoredMaxX,StoredMaxY); 
	 clientDC.LineTo(StoredMaxX,StoredMinY); clientDC.LineTo(StoredMinX,StoredMinY);
     m_IsVisible = true;
   }

    if (m_Pen) clientDC.SelectObject(aOldPen);
}

void CCadSurfView::OnKillFocus(CWnd* pNewWnd) 
{
	CView::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	((CChildFrame*)GetParentFrame())->m_wndSplitter.RefreshSplitBars();
}

void CCadSurfView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	((CChildFrame*)GetParentFrame())->m_wndSplitter.RefreshSplitBars();
}

void CCadSurfView::OnSnapshot() 
{
	// TODO: Add your command handler code here
	GetDIB();
	if( hDIB == NULL )
	{
		MessageBox("HDIB handle is NULL, save abort !!!");
		return;
	}
	CFileDialog  dilg(FALSE, NULL, "*.bmp", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
					  "Bitmap Files (*.bmp)|*.bmp|All Files (*.*)|*.*||");
												   
	dilg.m_ofn.lpstrTitle="Save Bitmap Dialog";
	if( dilg.DoModal() != IDOK )
	{
		FreeDIB();
		return;
	}
	CString pszPathName = dilg.GetPathName();

	CFile file;
	CFileException fe;

	if (!file.Open(pszPathName, CFile::modeCreate |
	  CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		FreeDIB();
		MessageBox("Cannot save the BMP file <" + pszPathName + "> !!!");
		return;
	}

	BOOL bSuccess = FALSE;
	TRY
	{
		BeginWaitCursor();
		bSuccess = ::SaveDIB(hDIB, (HANDLE)file.m_hFile);
		file.Close();
	}
	CATCH (CException, eSave)
	{
		file.Abort(); // will not throw an exception
		EndWaitCursor();
		FreeDIB();
		MessageBox("Cannot save the BMP file <" + pszPathName + "> !!!");
		return;
	}
	END_CATCH
	EndWaitCursor();
	if (!bSuccess)  
	{
		FreeDIB();
		MessageBox("Cannot save the BMP file <" + pszPathName + "> !!!");
	}

	FreeDIB();
}

void CCadSurfView::OnFilePrint() 
{
	// TODO: Add your command handler code here
	m_nSaveBMPFlag = BMPCLIENT;
	GetDIB();

	CView::OnFilePrint();
}

void CCadSurfView::OnFilePrintPreview() 
{
	// TODO: Add your command handler code here
	m_nSaveBMPFlag = BMPCLIENT;
	GetDIB();

	CView::OnFilePrintPreview();	
}

void CCadSurfView::GetDIB()
{
	if(hDIB != NULL)
	{
		::GlobalUnlock(hDIB);
		::GlobalFree(hDIB);
		hDIB = NULL;
	}
	
	m_nSaveBMPFlag = BMPCLIENT;
	hDIB = ::CopyWindowToDIB(GetSafeHwnd(), m_nSaveBMPFlag);
}

void CCadSurfView::FreeDIB()
{
	::GlobalUnlock(hDIB);
	::GlobalFree(hDIB);
	hDIB = NULL;
}


void CCadSurfView::PrintDIB(CDC *pDC)
{
if(hDIB == NULL || m_palDIB == NULL) return;

	int cxPage = pDC->GetDeviceCaps(HORZRES);
	int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
	int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);
	
	char* pDIB = (char*)::GlobalLock(hDIB);
	int cxDIB  = (int)  ::DIBWidth(pDIB);
	int cyDIB  = (int)  ::DIBHeight(pDIB);
	if( cxDIB <= 0 || cyDIB <= 0 ) return;

	RECT rcDst, rcDIB;
	rcDIB.top    = 0;
	rcDIB.left	 = 0;
	rcDIB.right  = cxDIB;
	rcDIB.bottom = cyDIB;
	rcDst.left   = 0;
	rcDst.top    = 0;
	rcDst.right  = cxPage;
	rcDst.bottom = (int)(1.0 * cyDIB * cxPage * cyInch / cxDIB / cxInch);

	::PaintDIB(pDC->m_hDC, &rcDst, (HDIB)hDIB, &rcDIB, (HPALETTE)m_palDIB->m_hObject);
}


void CCadSurfView::OnProjectionType() 
{
	// TODO: Add your command handler code here
	POSITION pos = GetDocument()->GetFirstViewPosition();
	while(pos != NULL)
	{
		CCadSurfView* view = (CCadSurfView*) GetDocument()->GetNextView(pos);
		CGLView::GLProjectionType aType = view->GetView()->GetProjectionType();
		view->GetView()->SetProjectionType((aType == CGLView::GLORTHOGRAPHIC) ? CGLView::GLPERSPECTIVE: CGLView::GLORTHOGRAPHIC);
	}
}

void CCadSurfView::OnUpdateProjectionType(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	POSITION pos = GetDocument()->GetFirstViewPosition();
	while(pos != NULL)
	{
		CCadSurfView* view = (CCadSurfView*) GetDocument()->GetNextView(pos);
		CGLView::GLProjectionType aType = view->GetView()->GetProjectionType();
		pCmdUI->SetCheck((aType == CGLView::GLPERSPECTIVE));
	}	
}

#include "LightDlg.h"
void CCadSurfView::OnPreferencesLight()
{
	// TODO: 在此添加命令处理程序代码
	CLightDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		GLfloat x,y,z,w,s,e,c;
		x=dlg.m_x;y=dlg.m_y;z=dlg.m_z;w=dlg.m_w;s=dlg.m_s;e=dlg.m_e;c=dlg.m_c;
		CGLDisplayContext* ctx = GetDocument()->dContext;

		for(ctx->InitSelected(); ctx->MoreSelected(); ctx->NextSelected())
		{
			CGLObject* O = ctx->CurrentSelected();
			O->SetMaterial(x,y,z,w,s,e,c);
		}
		InvalidateRect(NULL, FALSE);
		GetDocument()->UpdateAllViews(NULL);
		
	}
}


void CCadSurfView::OnUpdatePreferencesLight(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CGLDisplayContext* ctx = GetDocument()->dContext;
	pCmdUI->Enable(ctx->HasSelected() && dShaded);
}


//草图打开时，应该许多命令不能执行;此逻辑需要梳理;

void CCadSurfView::OnSkechLine()
{
	stlAxisTargets.clear();
	ClearStlRotationAxisFeedback();
	if (!stlMoveTargets.empty())
	{
		stlMoveTargets.clear();
		stlMoveHasStart = false;
		Invalidate(FALSE);
	}
	//绘制草图直线，交互绘制;
	m_iSketchType = 1;

	//command
	if( m_pCmd ){ 
		m_pCmd->Cancel();
		delete m_pCmd ;
		m_pCmd = NULL; 
	}

	// Align the view with the sketch plane before collecting line endpoints.
	const GLViewType viewType = myView->GetViewType();
	if (m_iSketchPlane == 1 && viewType != GLTOPVIEW)
		OnTopview();
	else if (m_iSketchPlane == 2 && viewType != GLFRONTVIEW)
		OnFrontview();
	else if (m_iSketchPlane == 3 && viewType != GLLEFTVIEW)
		OnLeftview();

	m_pCmd = new CCreateLine();
}


void CCadSurfView::OnUpdateSkechLine(CCmdUI *pCmdUI)
{
	//在关闭草图的情况下，此功能关闭;
	pCmdUI->Enable( m_iSketchPlane != 0);
	pCmdUI->SetCheck(m_iSketchType == 1);
}


void CCadSurfView::OnSketchXoy()
{
	//在XOY平面绘制草图;
	m_iSketchPlane = 1;
	OnTopview();
}


void CCadSurfView::OnUpdateSketchXoy(CCmdUI *pCmdUI)
{
	//
	pCmdUI->SetCheck(m_iSketchPlane == 1);
}


void CCadSurfView::OnSketchXoz()
{
	// TODO: 在此添加命令处理程序代码
	m_iSketchPlane = 2;
	OnFrontview();
}


void CCadSurfView::OnUpdateSketchXoz(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码;
	pCmdUI->SetCheck(m_iSketchPlane == 2);
}


void CCadSurfView::OnSketchYoz()
{
	// TODO: 在此添加命令处理程序代码;
	m_iSketchPlane = 3;
	OnLeftview();
}


void CCadSurfView::OnUpdateSketchYoz(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码;
	pCmdUI->SetCheck(m_iSketchPlane == 3);
}

void CCadSurfView::OnSketchClose()
{
	// TODO: 在此添加命令处理程序代码;
	m_iSketchPlane = 0;
	m_iSketchType = 0;
	OnResetview();

	if( m_pCmd ){ 
		m_pCmd->Cancel();
		delete m_pCmd ;
		m_pCmd = NULL; 
	}
	Invalidate(FALSE);
}


void CCadSurfView::OnUpdateSketchClose(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码;
	pCmdUI->SetCheck(m_iSketchPlane ==0);
}


void CCadSurfView::OnSkechCircle()
{
	// TODO: 在此添加命令处理程序代码;
	m_iSketchType = 2;

	//command
	//if (m_pCmd) {
	//	m_pCmd->Cancel();
	//	delete m_pCmd;
	//	m_pCmd = NULL;
	//}

	//m_pCmd = new CCreateCircle();
}


void CCadSurfView::OnUpdateSkechCircle(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码;
	pCmdUI->SetCheck(m_iSketchType == 2);
	pCmdUI->Enable(m_iSketchPlane != 0);
}


namespace
{
class CStlTransformDialog : public CDialog
{
public:
    static constexpr int PickLine = 3;

    CStlTransformDialog(UINT templateId, CWnd* parent)
        : CDialog(templateId, parent), templateId(templateId)
    {
    }

    StlPosition offset = {{0.0, 0.0, 0.0}};
    double angle = 90.0;
    double factor = 1.0;
    int axis = 2;

protected:
    void DoDataExchange(CDataExchange* dx) override
    {
        CDialog::DoDataExchange(dx);
        if (templateId == IDD_STL_MOVE)
        {
            ExchangeFinite(dx, IDC_STL_X, offset[0]);
            ExchangeFinite(dx, IDC_STL_Y, offset[1]);
            ExchangeFinite(dx, IDC_STL_Z, offset[2]);
        }
        else if (templateId == IDD_STL_ROTATE)
        {
            ExchangeFinite(dx, IDC_STL_VALUE, angle);
            DDX_Radio(dx, IDC_STL_AXIS_X, axis);
        }
        else
        {
            ExchangeFinite(dx, IDC_STL_VALUE, factor);
            if (dx->m_bSaveAndValidate && !(factor > 0.0))
            {
                AfxMessageBox(_T("Enter a scale factor greater than zero."), MB_ICONWARNING);
                dx->PrepareEditCtrl(IDC_STL_VALUE);
                dx->Fail();
            }
        }
    }

private:
    static void ExchangeFinite(CDataExchange* dx, int field, double& value)
    {
        DDX_Text(dx, field, value);
        if (dx->m_bSaveAndValidate && !std::isfinite(value))
        {
            AfxMessageBox(_T("Enter a finite number."), MB_ICONWARNING);
            dx->PrepareEditCtrl(field);
            dx->Fail();
        }
    }

    UINT templateId;
};

void SetStlStatus(LPCTSTR text)
{
    if (auto* frame = DYNAMIC_DOWNCAST(CFrameWnd, AfxGetMainWnd()))
        frame->SetMessageText(text);
}
}

void CCadSurfView::OnStlAutoArrange()
{
    const auto objects = GetDocument()->dContext->ObjectsOfType(GLSTLMESH);
    if (objects.empty())
        return;

    std::vector<unsigned long> objectIds;
    objectIds.reserve(objects.size());
    for (const CGLObject* object : objects)
        objectIds.push_back(object->GetObjID());
    auto before = GetDocument()->CaptureStlStates(objectIds);

    double maximumSpan = 0.0;
    double totalWidth = 0.0;
    for (const CGLObject* object : objects)
    {
        const CBoundingBox bounds = object->GetBoundingBox();
        const double width = bounds.XMax() - bounds.XMin();
        const double depth = bounds.YMax() - bounds.YMin();
        maximumSpan = (std::max)(maximumSpan, (std::max)(width, depth));
        totalWidth += width;
    }

    const double gap = (std::max)(1.0, maximumSpan * 0.1);
    totalWidth += gap * static_cast<double>(objects.size() - 1);
    double cursor = -totalWidth * 0.5;

    struct Change
    {
        CGLStlMesh* mesh;
        StlTransform matrix;
        CBoundingBox bounds;
    };
    std::vector<Change> changes;
    changes.reserve(objects.size());

    for (CGLObject* object : objects)
    {
        auto* mesh = dynamic_cast<CGLStlMesh*>(object);
        if (!mesh)
            return;

        const CBoundingBox current = mesh->GetBoundingBox();
        const double width = current.XMax() - current.XMin();
        const StlPosition offset = {{
            cursor - current.XMin(),
            -(current.YMin() + current.YMax()) * 0.5,
            -current.ZMin()
        }};

        Change change;
        change.mesh = mesh;
        if (!mesh->PrepareTransform(StlTransform::Translation(offset),
                                    change.matrix, change.bounds))
        {
            AfxMessageBox(_T("The automatic arrangement is outside the supported coordinate range."),
                          MB_ICONWARNING);
            return;
        }
        changes.push_back(change);
        cursor += width + gap;
    }

    for (const Change& change : changes)
        change.mesh->SetTransform(change.matrix, change.bounds);

    GetDocument()->dContext->RecomputeBounds();
    GetDocument()->RecordStlTransform(
        _T("Auto Arrange"), std::move(before), GetDocument()->CaptureStlStates(objectIds));
    myView->FitAll();
    GetDocument()->UpdateAllViews(nullptr);

    CString message;
    message.Format(_T("Auto-arranged %lu STL part(s) on the XOY plane."),
                   static_cast<unsigned long>(changes.size()));
    SetStlStatus(message);
}

void CCadSurfView::OnUpdateStlAutoArrange(CCmdUI* command)
{
    command->Enable(!GetDocument()->dContext->ObjectsOfType(GLSTLMESH).empty());
}

void CCadSurfView::OnStlViewX()
{
    if (myView->GetViewType() == GLLEFTVIEW)
    {
        myView->RightView();
        view.Format(_T("-X"));
        SetStlStatus(_T("View from the -X direction."));
    }
    else
    {
        myView->LeftView();
        view.Format(_T("+X"));
        SetStlStatus(_T("View from the +X direction."));
    }
}

void CCadSurfView::OnStlViewY()
{
    if (myView->GetViewType() == GLBACKVIEW)
    {
        myView->FrontView();
        view.Format(_T("-Y"));
        SetStlStatus(_T("View from the -Y direction."));
    }
    else
    {
        myView->BackView();
        view.Format(_T("+Y"));
        SetStlStatus(_T("View from the +Y direction."));
    }
}

void CCadSurfView::OnStlViewZ()
{
    if (myView->GetViewType() == GLTOPVIEW)
    {
        myView->BottomView();
        view.Format(_T("-Z"));
        SetStlStatus(_T("View from the -Z direction."));
    }
    else
    {
        myView->TopView();
        view.Format(_T("+Z"));
        SetStlStatus(_T("View from the +Z direction."));
    }
}

void CCadSurfView::OnStlViewNeIsometric()
{
    myView->IsometricView(-135.0);
    view.Format(_T("NE ISO"));
    SetStlStatus(_T("Northeast isometric view."));
}

void CCadSurfView::OnStlViewNwIsometric()
{
    myView->IsometricView(135.0);
    view.Format(_T("NW ISO"));
    SetStlStatus(_T("Northwest isometric view."));
}

void CCadSurfView::OnStlViewSeIsometric()
{
    myView->IsometricView(-45.0);
    view.Format(_T("SE ISO"));
    SetStlStatus(_T("Southeast isometric view."));
}

void CCadSurfView::OnStlViewSwIsometric()
{
    myView->IsometricView(45.0);
    view.Format(_T("SW ISO"));
    SetStlStatus(_T("Southwest isometric view."));
}

void CCadSurfView::ClearStlRotationAxisFeedback()
{
    stlAxisHoverId = 0;
    stlAxisVisualVisible = false;
}

void CCadSurfView::UpdateStlRotationAxisHover(CPoint point)
{
    CPoint3D start, finish;
    const int axisId = myView->ProcessSelection(point.x, point.y, 24, true);
    if (!axisId || !GetDocument()->dContext->GetStraightLine(axisId, start, finish))
    {
        if (stlAxisHoverId || stlAxisVisualVisible)
        {
            ClearStlRotationAxisFeedback();
            Invalidate(FALSE);
        }
        SetStlStatus(_T("Move over an existing straight line, then click it. Esc cancels."));
        return;
    }

    if (stlAxisHoverId != static_cast<unsigned long>(axisId) || !stlAxisVisualVisible)
    {
        stlAxisHoverId = static_cast<unsigned long>(axisId);
        stlAxisVisualStart = start;
        stlAxisVisualFinish = finish;
        stlAxisVisualVisible = true;
        Invalidate(FALSE);
    }
    SetStlStatus(_T("Axis line highlighted. Click to rotate, or press Esc to cancel."));
}

std::vector<unsigned long> CCadSurfView::SelectedStlIds()
{
    std::vector<unsigned long> targets;
    CGLDisplayContext* context = GetDocument()->dContext;
    for (context->InitSelected(); context->MoreSelected(); context->NextSelected())
    {
        const CGLObject* object = context->CurrentSelected();
        if (object && object->IsOfType(GLSTLMESH))
            targets.push_back(object->GetObjID());
    }
    return targets;
}

bool CCadSurfView::StlSelectionCenter(const std::vector<unsigned long>& targets,
                                     StlPosition& center)
{
    if (targets.empty())
        return false;

    CBoundingBox bounds;
    bool first = true;
    CGLDisplayContext* context = GetDocument()->dContext;
    for (unsigned long id : targets)
    {
        const CGLObject* object = context->FindDisplayed(id);
        if (!object || !object->IsOfType(GLSTLMESH))
            return false;
        if (first)
        {
            bounds = object->GetBoundingBox();
            first = false;
        }
        else
            bounds.AddBox(object->GetBoundingBox());
    }

    const CPoint3D pivot = bounds.Center();
    center = {{pivot.GetX(), pivot.GetY(), pivot.GetZ()}};
    return true;
}

void CCadSurfView::ApplyStlTransform(const std::vector<unsigned long>& targets,
                                    const StlTransform& transform, LPCTSTR action)
{
    struct Change
    {
        CGLStlMesh* mesh;
        StlTransform matrix;
        CBoundingBox bounds;
    };

    CGLDisplayContext* context = GetDocument()->dContext;
    auto before = GetDocument()->CaptureStlStates(targets);
    std::vector<Change> changes;
    changes.reserve(targets.size());
    // Validate the entire batch before committing any object's new placement.
    for (unsigned long id : targets)
    {
        auto* mesh = dynamic_cast<CGLStlMesh*>(context->FindDisplayed(id));
        if (!mesh)
        {
            SetStlStatus(_T("The original STL selection is no longer visible."));
            return;
        }
        Change change;
        change.mesh = mesh;
        if (!mesh->PrepareTransform(transform, change.matrix, change.bounds))
        {
            AfxMessageBox(_T("The transform is outside the supported coordinate range."), MB_ICONWARNING);
            return;
        }
        changes.push_back(change);
    }

    for (const Change& change : changes)
        change.mesh->SetTransform(change.matrix, change.bounds);

    context->RecomputeBounds();
    CString historyName;
    if (_tcsncmp(action, _T("Moved"), 5) == 0)
        historyName = _T("Move");
    else if (_tcsncmp(action, _T("Rotated"), 7) == 0)
        historyName = _T("Rotate");
    else if (_tcsncmp(action, _T("Scaled"), 6) == 0)
        historyName = _T("Scale");
    else
        historyName = _T("Transform");
    GetDocument()->RecordStlTransform(
        historyName, std::move(before), GetDocument()->CaptureStlStates(targets));
    GetDocument()->UpdateAllViews(nullptr);
    CString message;
    message.Format(_T("%s %lu STL part(s)."), action, static_cast<unsigned long>(changes.size()));
    SetStlStatus(message);
}

void CCadSurfView::OnStlMove()
{
    ClearStlRotationAxisFeedback();
    Invalidate(FALSE);
    const auto targets = SelectedStlIds();
    CStlTransformDialog dialog(IDD_STL_MOVE, this);
    if (!targets.empty() && dialog.DoModal() == IDOK)
        ApplyStlTransform(targets, StlTransform::Translation(dialog.offset), _T("Moved"));
}

void CCadSurfView::OnStlMoveByPoints()
{
    ClearStlRotationAxisFeedback();
    Invalidate(FALSE);
    stlMoveTargets = SelectedStlIds();
    if (stlMoveTargets.empty())
        return;

    stlMoveHasStart = false;
    SetFocus();
    SetStlStatus(_T("Pick start point A: an existing point or line endpoint. Esc cancels."));
}

void CCadSurfView::PickStlMovePoint(CPoint cursor)
{
    CPoint3D picked;
    if (!myView->PickReferencePoint(cursor, picked))
    {
        SetStlStatus(stlMoveHasStart
            ? _T("No point picked. Click an existing point or line endpoint for B.")
            : _T("No point picked. Click an existing point or line endpoint for A."));
        return;
    }

    if (!stlMoveHasStart)
    {
        stlMoveStart = picked;
        stlMoveHasStart = true;
        Invalidate(FALSE);
        SetStlStatus(_T("Start point A selected. Pick destination point B. Esc cancels."));
        return;
    }

    const StlPosition offset = {{picked.GetX() - stlMoveStart.GetX(),
                                 picked.GetY() - stlMoveStart.GetY(),
                                 picked.GetZ() - stlMoveStart.GetZ()}};
    if (offset == StlPosition{{0.0, 0.0, 0.0}})
    {
        SetStlStatus(_T("Point B must be different from A. Pick another point or press Esc."));
        return;
    }

    auto targets = std::move(stlMoveTargets);
    stlMoveTargets.clear();
    stlMoveHasStart = false;
    Invalidate(FALSE);
    ApplyStlTransform(targets, StlTransform::Translation(offset), _T("Moved"));
}

void CCadSurfView::OnStlRotate()
{
    ClearStlRotationAxisFeedback();
    Invalidate(FALSE);
    auto targets = SelectedStlIds();
    CStlTransformDialog dialog(IDD_STL_ROTATE, this);
    if (targets.empty() || dialog.DoModal() != IDOK)
        return;

    if (dialog.axis == CStlTransformDialog::PickLine)
    {
        stlAxisTargets = std::move(targets);
        stlAxisAngle = dialog.angle;
        SetFocus();
        SetStlStatus(_T("Move over an existing straight line, then click it. Esc cancels."));
        return;
    }

    if (dialog.axis < 0 || dialog.axis > 2)
        return;
    const StlPosition origin = {{0.0, 0.0, 0.0}};
    StlPosition direction = {{0.0, 0.0, 0.0}};
    direction[dialog.axis] = 1.0;
    StlTransform rotation;
    if (StlTransform::Rotation(origin, direction, dialog.angle, rotation))
    {
        static LPCTSTR actions[] = {
            _T("Rotated around world X axis"),
            _T("Rotated around world Y axis"),
            _T("Rotated around world Z axis")
        };
        ApplyStlTransform(targets, rotation, actions[dialog.axis]);
    }
}

void CCadSurfView::OnStlScale()
{
    ClearStlRotationAxisFeedback();
    Invalidate(FALSE);
    const auto targets = SelectedStlIds();
    CStlTransformDialog dialog(IDD_STL_SCALE, this);
    StlPosition center;
    if (!targets.empty() && dialog.DoModal() == IDOK && StlSelectionCenter(targets, center))
        ApplyStlTransform(targets, StlTransform::Scale(center, dialog.factor), _T("Scaled"));
}

void CCadSurfView::OnUpdateStlTransform(CCmdUI* command)
{
    bool enabled = false;
    if (!m_pCmd && stlAxisTargets.empty() && stlMoveTargets.empty())
    {
        CGLDisplayContext* context = GetDocument()->dContext;
        for (context->InitSelected(); context->MoreSelected(); context->NextSelected())
        {
            const CGLObject* object = context->CurrentSelected();
            if (object && object->IsOfType(GLSTLMESH))
            {
                enabled = true;
                break;
            }
        }
    }
    command->Enable(enabled);
}

void CCadSurfView::PickStlRotationAxis(CPoint point)
{
    CPoint3D start, finish;
    const int axisId = myView->ProcessSelection(point.x, point.y, 24, true);
    if (!GetDocument()->dContext->GetStraightLine(axisId, start, finish))
    {
        SetStlStatus(_T("No straight line picked. Click an axis line or press Esc."));
        return;
    }

    const StlPosition pivot = {{start.GetX(), start.GetY(), start.GetZ()}};
    const StlPosition direction = {{finish.GetX() - pivot[0],
                                    finish.GetY() - pivot[1],
                                    finish.GetZ() - pivot[2]}};
    StlTransform rotation;
    if (!StlTransform::Rotation(pivot, direction, stlAxisAngle, rotation))
    {
        SetStlStatus(_T("The axis line must have distinct endpoints. Pick another line or press Esc."));
        return;
    }

    auto targets = std::move(stlAxisTargets);
    stlAxisTargets.clear();
    stlAxisHoverId = 0;
    stlAxisVisualStart = start;
    stlAxisVisualFinish = finish;
    stlAxisVisualVisible = true;
    Invalidate(FALSE);
    ApplyStlTransform(targets, rotation, _T("Rotated around picked line"));
}

void CCadSurfView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    if (nChar == VK_ESCAPE && !stlMoveTargets.empty())
    {
        stlMoveTargets.clear();
        stlMoveHasStart = false;
        Invalidate(FALSE);
        SetStlStatus(_T("STL point-to-point move canceled."));
        return;
    }
    if (nChar == VK_ESCAPE && !stlAxisTargets.empty())
    {
        stlAxisTargets.clear();
        ClearStlRotationAxisFeedback();
        Invalidate(FALSE);
        SetStlStatus(_T("STL rotation canceled."));
        return;
    }
    if (nChar == VK_ESCAPE && m_pCmd)
    {
        m_pCmd->Cancel();
        delete m_pCmd;
        m_pCmd = NULL;
        m_iSketchType = 0;
        if (GetCapture() == this)
            ReleaseCapture();
        Invalidate(FALSE);
        SetStlStatus(_T("Sketch line command ended."));
        return;
    }
    if (nChar == VK_ESCAPE && stlAxisVisualVisible)
    {
        ClearStlRotationAxisFeedback();
        Invalidate(FALSE);
        SetStlStatus(_T("Rotation axis highlight cleared."));
        return;
    }

    if (nChar == VK_DELETE && stlAxisTargets.empty() && stlMoveTargets.empty())
    {
        GetDocument()->OnEditDeleteselected();
        InvalidateRect(NULL, FALSE);
    }
    CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
