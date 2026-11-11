// MaterialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CadSurf.h"
#include "MaterialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaterialDlg dialog


CMaterialDlg::CMaterialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMaterialDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMaterialDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMaterialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaterialDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMaterialDlg, CDialog)
	//{{AFX_MSG_MAP(CMaterialDlg)
		// NOTE: the ClassWizard will add message map macros here
		ON_COMMAND_EX_RANGE(IDC_ALUM,IDC_BRUBBER, OnMaterial)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaterialDlg message handlers

BOOL CMaterialDlg::OnMaterial(UINT nID)
{
	switch(nID)
	{
	case IDC_ALUM :
		m_Matl = ALUMINIUM;
		break;
	case IDC_BRASS :
		m_Matl = BRASS;
		break;
	case IDC_PBRONZE :
		m_Matl = P_BRONZE;
		break;
	case IDC_CHROME :
		m_Matl = CHROME;
		break;
	case IDC_PCOPPER :
		m_Matl = P_COPPER;
		break;
	case IDC_GOLD :
		m_Matl = GOLD;
		break;
	case IDC_PEWTER :
		m_Matl = PEWTER;
		break;
	case IDC_SILVER :
		m_Matl = SILVER;
		break;
	case IDC_STEEL :
		m_Matl = STEEL;
		break;
	case IDC_EMERALD :
		m_Matl = EMERALD;
		break;
	case IDC_OBSIDIAN :
		m_Matl = OBSIDIAN;
		break;
	case IDC_PEARL :
		m_Matl = PEARL;
		break;
	case IDC_TURQUOISE :
		m_Matl = TURQUOISE;
		break;
	case IDC_BPLASTIC :
		m_Matl = PLASTIC;
		break;
	case IDC_BRONZE :
		m_Matl = BRONZE;
		break;
	case IDC_COPPER :
		m_Matl = COPPER;
		break;
	case IDC_PGOLD :
		m_Matl = P_GOLD;
		break;
	case IDC_PSILVER :
		m_Matl = P_SILVER;
		break;
	case IDC_JADE :
		m_Matl = JADE;
		break;
	case IDC_RUBY :
		m_Matl = RUBY;
		break;
	case IDC_BRUBBER :
		m_Matl = RUBBER;
		break;
	default:
		m_Matl = JADE;
		break;
	}
	return TRUE;
}
