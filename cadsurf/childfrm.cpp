// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "CadSurf.h"

#include "ChildFrm.h"
#include "CadSurfView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_WM_INITMENUPOPUP()
	ON_WM_MENUSELECT()
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_CREATE()
	//ON_WM_TIMER()
	ON_COMMAND(ID_MULTIFOUR, OnMultifour)
	ON_UPDATE_COMMAND_UI(ID_MULTIFOUR, OnUpdateMultifour)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		| FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	return TRUE;
}

void CChildFrame::ActivateFrame(int nCmdShow)
{
	// TODO: Modify this function to change how the frame is activated.

	nCmdShow = SW_SHOWMAXIMIZED;
	CMDIChildWnd::ActivateFrame(nCmdShow);
}


/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if (!m_wndToolBar2.Create(this) ||

	!m_wndToolBar2.LoadToolBar(IDR_CHILDFRAME))

	{
		TRACE0("Failed to create toolbar\n");

		return -1; // fail to create

	}

	m_wndToolBar2.SetBarStyle(m_wndToolBar2.GetBarStyle() |

		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	m_wndToolBar2.EnableDocking(CBRS_ALIGN_ANY);

		EnableDocking(CBRS_ALIGN_ANY);

	DockControlBar(&m_wndToolBar2);

	return 0;
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// CG: The following block was added by the Splitter Bar component.
	{
		if (!m_wndSplitter.Create(this,
		                          2, 2,          // TODO: adjust the number of rows, columns
		                          CSize(10, 10), // TODO: adjust the minimum pane size
		                          pContext,
								  WS_CHILD | WS_VISIBLE | SPLS_DYNAMIC_SPLIT))
		{
			TRACE0("Failed to create splitter bar ");
			return FALSE;    // failed to create
		}

		return TRUE;
	}
}

void CChildFrame::AutomaticSplit()
{
	// set the timer ( just a trick make an action once the splitting is done )
	KillTimer(1);  // Destroy the old timer
	SetTimer(1, 10 , NULL); // Creates a new timer (index = 1 ; time = 10ms)

	m_wndSplitter.DoAutomaticSplit();	
}

void CChildFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if ( m_wndSplitter.m_bSplittingDone ) // if the splitting is done
	{
		KillTimer(1); // kill the timer ( just a trick make an action once the splitting is done ) 

		//loop on the row & columns
		int nbRow = m_wndSplitter.GetRowCount();
		int nbCol = 
		m_wndSplitter.GetColumnCount(); for  ( int r = 0; r < nbRow; r++ )
		{
			for(int c = 0; c < nbCol; c++ )
			{
				CCadSurfView* theView = (CCadSurfView*)m_wndSplitter.GetPane(r,c);
				if ( r==0 && c== 0)
				{
					//top-left
					theView->OnFrontview();
					theView->OnFitall();
				}
				else if ( r==0 && c== 1)
				{
					//top-right
					theView->OnLeftview();
					theView->OnFitall();
				}
				else if ( r==1 && c== 0)
				{
					//bottom-left
					theView->OnTopview();
					theView->OnFitall();
				}
				else if ( r==1 && c==1)
				{
					//bottom-right
					theView->OnAxonview();
					theView->OnFitall();
				}
			}
		}
	}

	CMDIChildWnd::OnTimer(nIDEvent);
}


void CChildFrame::OnMultifour() 
{
	// TODO: Add your command handler code here
	if( !m_wndSplitter.m_bSplittingDone )
		AutomaticSplit();
	else
	{
		if(m_wndSplitter.GetColumnCount() > 1)
			m_wndSplitter.DeleteColumn(0);
		if(m_wndSplitter.GetRowCount() > 1)
			m_wndSplitter.DeleteRow(0);
	}
	m_wndSplitter.m_bSplittingDone = !m_wndSplitter.m_bSplittingDone;

}

void CChildFrame::OnUpdateMultifour(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}


void CChildFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rect;
   GetClientRect( &rect );
   if( m_wndSplitter.m_bSplittingDone )  // m_bSplitterCreated set in OnCreateClient
   {
      m_wndSplitter.SetColumnInfo(0, rect.Width()/2, 1);
      m_wndSplitter.SetColumnInfo(1, rect.Width()/2, 1);
	  m_wndSplitter.SetRowInfo(0, rect.Height()/2, 1);
      m_wndSplitter.SetRowInfo(1, rect.Height()/2, 1);
      m_wndSplitter.RecalcLayout();
   }

}



void CChildFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
  	CMDIChildWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
  
	// CG: The following block was inserted by 'Status Bar' component.
	{
		GetParentFrame()->PostMessage(WM_INITMENUPOPUP,
			(WPARAM)pPopupMenu->GetSafeHmenu(), MAKELONG(nIndex, bSysMenu));
	}

}

void CChildFrame::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
  	CMDIChildWnd::OnMenuSelect(nItemID, nFlags, hSysMenu);
  
	// CG: The following block was inserted by 'Status Bar' component.
	{
		static BOOL bMenuActive = FALSE;
		if (bMenuActive || hSysMenu != NULL)
		{
			GetParentFrame()->PostMessage(WM_MENUSELECT, 
				MAKELONG(nItemID, nFlags), (LPARAM)hSysMenu);
		}
		bMenuActive = hSysMenu != NULL;
	}

}
