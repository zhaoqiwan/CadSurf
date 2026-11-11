#if !defined(AFX_GRIDDLG_H__69C38217_0BBF_40A8_BF4D_A927C0FCA4AF__INCLUDED_)
#define AFX_GRIDDLG_H__69C38217_0BBF_40A8_BF4D_A927C0FCA4AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGridDlg dialog

class CGridDlg : public CDialog
{
// Construction
public:
	CGridDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGridDlg)
	enum { IDD = IDD_GRID };
	CSpinButtonCtrl	m_Spin5;
	CSpinButtonCtrl	m_Spin4;
	CSpinButtonCtrl	m_Spin3;
	CSpinButtonCtrl	m_Spin2;
	CSpinButtonCtrl	m_Spin1;
	double	m_Size;
	double	m_Step;
	double	m_DX;
	double	m_DY;
	double	m_DZ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGridDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDDLG_H__69C38217_0BBF_40A8_BF4D_A927C0FCA4AF__INCLUDED_)
