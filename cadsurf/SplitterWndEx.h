#if !defined(AFX_SPLITTERWNDEX_H__CF36A250_6865_48C2_A766_760798A16C6B__INCLUDED_)
#define AFX_SPLITTERWNDEX_H__CF36A250_6865_48C2_A766_760798A16C6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplitterWndEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplitterWndEx window

class CSplitterWndEx : public CSplitterWnd
{
// Construction
public:
	CSplitterWndEx();

// Attributes
public:

public:
	bool m_bSplittingDone;
protected :
	bool m_bIsAutomaticSplit;


// Operations
public:
	void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg);
	void RefreshSplitBars(void);

public:
	BOOL DoAutomaticSplit(); // new method
	BOOL DoKeyboardSplit(); // overload the CSplitterWnd::DoKeyboardSplit();
protected:
	void StartTracking(int ht); // overload the CSplitterWnd::StartTracking
	void StopTracking(BOOL bAccept); // overload the CSplitterWnd::StopTracking



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitterWndEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSplitterWndEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSplitterWndEx)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLITTERWNDEX_H__CF36A250_6865_48C2_A766_760798A16C6B__INCLUDED_)
