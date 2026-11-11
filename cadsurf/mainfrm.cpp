// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CadSurf.h"

#include "MainFrm.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_INITMENUPOPUP()
	ON_WM_MENUSELECT()
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_DATE, OnUpdateDate)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TIME, OnUpdateTime)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_COORD,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// CG: The following block was inserted by 'Status Bar' component.
	{
		m_nStatusPane1Width = -1;
		m_bMenuSelect = FALSE;
	}

	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_BUTTON, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndToolBar1.CreateEx(this, TBSTYLE_BUTTON, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar1.LoadToolBar(IDR_TB_GEOM))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndToolBar2.CreateEx(this, TBSTYLE_BUTTON, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar2.LoadToolBar(IDR_TB_TRN))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndStatusBar.SetPaneInfo(1,ID_INDICATOR_COORD,SBPS_NORMAL,250);
	

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar1.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar2.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	DockControlBarLeftOf(&m_wndToolBar1,&m_wndToolBar);
	DockControlBarLeftOf(&m_wndToolBar2,&m_wndToolBar1);

	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);
	// CG: The following block was inserted by 'Status Bar' component.
	{
		// Find out the size of the static variable 'indicators' defined
		// by AppWizard and copy it
		int nOrigSize = sizeof(indicators) / sizeof(UINT);

		UINT* pIndicators = new UINT[nOrigSize + 2];
		memcpy(pIndicators, indicators, sizeof(indicators));

		// Call the Status Bar Component's status bar creation function
		if (!InitStatusBar(pIndicators, nOrigSize, 60))
		{
			TRACE0("Failed to initialize Status Bar\n");
			return -1;
		}
		delete[] pIndicators;
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;
	AfxGetApp()->m_nCmdShow = SW_SHOWMAXIMIZED;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf)
{
     CRect rect;
     DWORD dw;
     UINT n;

     // get MFC to adjust the dimensions of all docked ToolBars
     // so that GetWindowRect will be accurate
     RecalcLayout();
     LeftOf->GetWindowRect(&rect);
     rect.OffsetRect(1,0);
     dw=LeftOf->GetBarStyle();
     n = 0;
     n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
     n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
     n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
     n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;

     // When we take the default parameters on rect, DockControlBar will dock
     // each Toolbar on a seperate line.  By calculating a rectangle, we in effect
     // are simulating a Toolbar being dragged to that location and docked.
     DockControlBar(Bar,n,&rect);
}

void CMainFrame::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
  	CMDIFrameWnd::OnMenuSelect(nItemID, nFlags, hSysMenu);
  
	// CG: The following block was inserted by 'Status Bar' component.
	{
		// Restore first pane of the statusbar?
		if (nFlags == 0xFFFF && hSysMenu == 0 && m_nStatusPane1Width != -1)
		{
			m_bMenuSelect = FALSE;
			m_wndStatusBar.SetPaneInfo(0, 
				m_nStatusPane1ID, m_nStatusPane1Style, m_nStatusPane1Width);
			m_nStatusPane1Width = -1;   // Set it to illegal value
		}
		else 
		{
			m_bMenuSelect = TRUE;
		}
	}

}

void CMainFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
  	CMDIFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
  
	// CG: The following block was inserted by 'Status Bar' component.
	{
		// store width of first pane and its style
		if (m_nStatusPane1Width == -1 && m_bMenuSelect)
		{
			m_wndStatusBar.GetPaneInfo(0, m_nStatusPane1ID, 
				m_nStatusPane1Style, m_nStatusPane1Width);
			m_wndStatusBar.SetPaneInfo(0, m_nStatusPane1ID, 
				SBPS_NOBORDERS|SBPS_STRETCH, 16384);
		}
	}

}

void CMainFrame::OnUpdateDate(CCmdUI* pCmdUI)
{
	// CG: This function was inserted by 'Status Bar' component.

	// Get current date and format it
	CTime time = CTime::GetCurrentTime();
	CString strDate = time.Format(_T("%A, %B %d, %y "));

	// BLOCK: compute the width of the date string
	CSize size;
	{
		HGDIOBJ hOldFont = NULL;
		HFONT hFont = (HFONT)m_wndStatusBar.SendMessage(WM_GETFONT);
		CClientDC dc(NULL);
		if (hFont != NULL) 
			hOldFont = dc.SelectObject(hFont);
		size = dc.GetTextExtent(strDate);
		if (hOldFont != NULL) 
			dc.SelectObject(hOldFont);
	}

	// Update the pane to reflect the current date
	UINT nID, nStyle;
	int nWidth;
	m_wndStatusBar.SetPaneInfo(1,ID_INDICATOR_COORD,SBPS_NOBORDERS,250);
	m_wndStatusBar.GetPaneInfo(m_nDatePaneNo, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(m_nDatePaneNo, nID, nStyle, size.cx);
	pCmdUI->SetText(strDate);
	pCmdUI->Enable(TRUE);

}

void CMainFrame::OnUpdateTime(CCmdUI* pCmdUI)
{
	// CG: This function was inserted by 'Status Bar' component.

	// Get current date and format it
	CTime time = CTime::GetCurrentTime();
	CString strTime = time.Format(_T("%X"));

	// BLOCK: compute the width of the date string
	CSize size;
	{
		HGDIOBJ hOldFont = NULL;
		HFONT hFont = (HFONT)m_wndStatusBar.SendMessage(WM_GETFONT);
		CClientDC dc(NULL);
		if (hFont != NULL) 
			hOldFont = dc.SelectObject(hFont);
		size = dc.GetTextExtent(strTime);
		if (hOldFont != NULL) 
			dc.SelectObject(hOldFont);
	}

	// Update the pane to reflect the current time
	UINT nID, nStyle;
	int nWidth;
	m_wndStatusBar.GetPaneInfo(m_nTimePaneNo, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(m_nTimePaneNo, nID, nStyle, size.cx);
	pCmdUI->SetText(strTime);
	pCmdUI->Enable(TRUE);

}

BOOL CMainFrame::InitStatusBar(UINT *pIndicators, int nSize, int nSeconds)
{
	// CG: This function was inserted by 'Status Bar' component.

	// Create an index for the DATE pane
	m_nDatePaneNo = nSize++;
	pIndicators[m_nDatePaneNo] = ID_INDICATOR_DATE;
	// Create an index for the TIME pane
	m_nTimePaneNo = nSize++;
	nSeconds = 1;
	pIndicators[m_nTimePaneNo] = ID_INDICATOR_TIME;

	// TODO: Select an appropriate time interval for updating
	// the status bar when idling.
	m_wndStatusBar.SetTimer(0x1000, nSeconds * 1000, NULL);

	return m_wndStatusBar.SetIndicators(pIndicators, nSize);

}
