// SplitterWndEx.cpp : implementation file
//

#include "stdafx.h"
#include "CadSurf.h"
#include "SplitterWndEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplitterWndEx

CSplitterWndEx::CSplitterWndEx()
{
	m_bIsAutomaticSplit = false;
	m_bSplittingDone = false;
}

CSplitterWndEx::~CSplitterWndEx()
{
}


BEGIN_MESSAGE_MAP(CSplitterWndEx, CSplitterWnd)
	//{{AFX_MSG_MAP(CSplitterWndEx)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSplitterWndEx message handlers
void CSplitterWndEx::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg)
{
	int x_ActivePane, y_ActivePane; 
	COLORREF hilightcolor = 
	 RGB(255,255,0);
	
	GetActivePane(&x_ActivePane, &y_ActivePane);
	if( ((GetRowCount()>1) ||(GetColumnCount()>1)) && (nType == splitBorder))
	{
		int pRow = 0;
		int pCol = 0;
		if(rectArg.top)
		{
			pRow = 1;	
		}
		if(rectArg.left)
		{
			pCol = 1;
		}
		if((pCol == y_ActivePane) && (pRow == x_ActivePane)) 
		{
			if (pDC == NULL)
			{
				RedrawWindow(rectArg, NULL, RDW_INVALIDATE|RDW_NOCHILDREN);
				return; 		
			}
			ASSERT_VALID(pDC);	
			CRect rect = 
			rectArg;pDC->Draw3dRect(rect, hilightcolor, hilightcolor);
			int dx = -GetSystemMetrics(SM_CXBORDER);
			int dy = 
			-GetSystemMetrics(SM_CYBORDER);
			rect.InflateRect(dx,dy);pDC->Draw3dRect(rect, hilightcolor, hilightcolor);
			return;
		}
	}
	CSplitterWnd::OnDrawSplitter(pDC,nType,rectArg);
}

//This Method draw an extra Rectangle around the ActivePane with the color <hilighcolor>. 

//Note: you can set 2 colors (see help for Draw3dRect). 

void CSplitterWndEx::RefreshSplitBars(void)
{
	CRect rectInside;
	GetInsideRect(rectInside);
	DrawAllSplitBars(NULL, rectInside.right, rectInside.bottom);
}

void CSplitterWndEx::StartTracking(int ht)
{
	//save the current cursor ...
	HCURSOR theCurrentCursor = GetCursor();

	CSplitterWnd::StartTracking(ht); 

	if ( m_bIsAutomaticSplit )
	{
		//...and restore it immediatelly if in AutomaticSplit mode
		SetCursor(theCurrentCursor);
	}
}

void CSplitterWndEx::StopTracking(BOOL bAccept)
{
	CSplitterWnd::StopTracking(bAccept);

	//now the is splitting done
	m_bIsAutomaticSplit = false;
	m_bSplittingDone = true;
}

BOOL CSplitterWndEx::DoAutomaticSplit()
{
	//save the current mouse position
	POINT theInitialMousePosition;
	GetCursorPos(&theInitialMousePosition);

	//set the splitting done to false ( of course )
	m_bSplittingDone = false;

	//and automaitic to true 
	m_bIsAutomaticSplit = true;
	
	//do the split
	BOOL RetVal = CSplitterWnd::DoKeyboardSplit();

	//restore immediatelly the old mouse poition
	SetCursorPos(theInitialMousePosition.x,theInitialMousePosition.y);
	return RetVal;
}

BOOL CSplitterWndEx::DoKeyboardSplit()
{
	m_bSplittingDone = false;
	m_bIsAutomaticSplit = false;
	return CSplitterWnd::DoKeyboardSplit();
}

void CSplitterWndEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	if ( m_bIsAutomaticSplit )
	{
		//if AutomaticSplit mode : We are not able to choose the position of the splitters
		// -> Exiting to next step
		StopTracking(TRUE);
		return;
	}
	CSplitterWnd::OnMouseMove(nFlags, point);
}

