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

#if !defined(AFX_MATERIALDLG_H__6E6E2E86_04DE_43C6_96F5_0CF7CCC72779__INCLUDED_)
#define AFX_MATERIALDLG_H__6E6E2E86_04DE_43C6_96F5_0CF7CCC72779__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MaterialDlg.h : header file
//

#include "GLObject.h"
#include "GLMaterial.h"

/////////////////////////////////////////////////////////////////////////////
// CMaterialDlg dialog

class CMaterialDlg : public CDialog
{
// Construction
public:
	CMaterialDlg(CWnd* pParent = NULL);   // standard constructor
	CGLMaterial m_Matl;

// Dialog Data
	//{{AFX_DATA(CMaterialDlg)
	enum { IDD = IDD_MATLDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaterialDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMaterialDlg)
		// NOTE: the ClassWizard will add member functions here
	afx_msg BOOL OnMaterial(UINT nID);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MATERIALDLG_H__6E6E2E86_04DE_43C6_96F5_0CF7CCC72779__INCLUDED_)
