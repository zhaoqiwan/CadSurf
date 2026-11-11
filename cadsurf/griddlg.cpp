// GridDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CadSurf.h"
#include "GridDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridDlg dialog


CGridDlg::CGridDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGridDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGridDlg)
	m_Size = 1000.0;
	m_Step = 50.0;
	m_DX = 0.0;
	m_DY = 0.0;
	m_DZ = 1.0;
	//}}AFX_DATA_INIT
}


void CGridDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGridDlg)
	DDX_Control(pDX, IDC_SPIN5, m_Spin5);
	DDX_Control(pDX, IDC_SPIN4, m_Spin4);
	DDX_Control(pDX, IDC_SPIN3, m_Spin3);
	DDX_Control(pDX, IDC_SPIN2, m_Spin2);
	DDX_Control(pDX, IDC_SPIN1, m_Spin1);
	DDX_Text(pDX, IDC_EDIT1, m_Size);
	DDV_MinMaxDouble(pDX, m_Size, 100., 1000000.);
	DDX_Text(pDX, IDC_EDIT2, m_Step);
	DDV_MinMaxDouble(pDX, m_Step, 1., 1000.);
	DDX_Text(pDX, IDC_EDIT3, m_DX);
	DDV_MinMaxDouble(pDX, m_DX, -100000., 100000.);
	DDX_Text(pDX, IDC_EDIT4, m_DY);
	DDV_MinMaxDouble(pDX, m_DY, -100000., 100000.);
	DDX_Text(pDX, IDC_EDIT5, m_DZ);
	DDV_MinMaxDouble(pDX, m_DZ, -100000., 100000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGridDlg, CDialog)
	//{{AFX_MSG_MAP(CGridDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridDlg message handlers

BOOL CGridDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Spin1.SetRange32(100, 1000000);
	m_Spin2.SetRange32(1, 1000);
	m_Spin3.SetRange32(-100000, 100000);
	m_Spin4.SetRange32(-100000, 100000);
	m_Spin5.SetRange32(-100000, 100000);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
