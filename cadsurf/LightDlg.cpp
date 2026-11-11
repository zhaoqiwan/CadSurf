// LightDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CadSurf.h"
#include "LightDlg.h"
#include "afxdialogex.h"
#include "GLView.h"


// CLightDlg 对话框

IMPLEMENT_DYNAMIC(CLightDlg, CDialog)

CLightDlg::CLightDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLightDlg::IDD, pParent)
{
	
}

CLightDlg::~CLightDlg()
{
}

void CLightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SLIDERX, m_Z);
	DDX_Control(pDX, IDC_SLIDERY, m_Y);
	DDX_Control(pDX, IDC_SLIDERZ, m_X);
	DDX_Control(pDX, IDC_SLIDERW, m_W);
	DDX_Control(pDX, IDC_SLIDERS, m_S);
	DDX_Control(pDX, IDC_SLIDERE, m_E);
	DDX_Control(pDX, IDC_SLIDERC, m_C);
	m_x=double(m_X.GetPos()/3);
	m_y=double(m_Y.GetPos()/3);
	m_z=double(m_Z.GetPos()/3);
	m_w=double(m_W.GetPos()/3);
	m_s=double(m_S.GetPos()/3);
	m_e=double(m_E.GetPos()/3);
	m_c=double(m_C.GetPos()/3);
	DDX_Control(pDX, IDC_LISTlight, m_listliht);
}


BEGIN_MESSAGE_MAP(CLightDlg, CDialog)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDERX, &CLightDlg::OnReleasedcaptureSliderx)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDERY, &CLightDlg::OnReleasedcaptureSlidery)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDERZ, &CLightDlg::OnReleasedcaptureSliderz)
	ON_BN_CLICKED(IDOK, &CLightDlg::OnBnClickedOk)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDERW, &CLightDlg::OnNMReleasedcaptureSliderw)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDERS, &CLightDlg::OnNMReleasedcaptureSliders)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDERE, &CLightDlg::OnNMReleasedcaptureSlidere)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDERC, &CLightDlg::OnNMReleasedcaptureSliderc)
	ON_BN_CLICKED(IDC_Setcolor, &CLightDlg::OnBnClickedSetcolor)
	ON_BN_CLICKED(IDC_BUTTONnew, &CLightDlg::OnBnClickedButtonnew)
	ON_BN_CLICKED(IDC_BUTTONdelete, &CLightDlg::OnBnClickedButtondelete)
END_MESSAGE_MAP()


BOOL CLightDlg::OnInitDialog()
// CLightDlg 消息处理程序
{
 	CDialog::OnInitDialog();
	close=false;
	m_X.SetRange(-400000,400000);
 	m_Y.SetRange(-400000,400000);
 	m_Z.SetRange(-400000,400000);
	m_W.SetRange(-400000,400000);
	m_S.SetRange(0,4000000);
	m_E.SetRange(0,4000000);
	m_C.SetRange(0,540);
	m_X.SetPos(20);
	m_Y.SetPos(10);
	m_Z.SetPos(10);
	m_W.SetPos(0);
	m_S.SetPos(80);
	m_E.SetPos(0);
	m_C.SetPos(540);
	return true;
}
 

void CLightDlg::OnReleasedcaptureSliderx(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	*pResult = 0;
}
void CLightDlg::OnReleasedcaptureSlidery(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	*pResult = 0;
}
void CLightDlg::OnReleasedcaptureSliderz(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	*pResult = 0;
}

void CLightDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	close=true;
	CDialog::OnOK();
}


void CLightDlg::OnNMReleasedcaptureSliderw(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	*pResult = 0;
}


void CLightDlg::OnNMReleasedcaptureSliders(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	*pResult = 0;
}


void CLightDlg::OnNMReleasedcaptureSlidere(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	*pResult = 0;
}


void CLightDlg::OnNMReleasedcaptureSliderc(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	*pResult = 0;
}


void CLightDlg::OnBnClickedSetcolor()
{
	// TODO: 在此添加控件通知处理程序代码
   CColorDialog dlg;
   //dlg.DoModal();
   dlg.m_cc.Flags |= CC_RGBINIT;
	dlg.m_cc.rgbResult = RGB(0, 0, 0);
	if(dlg.DoModal()==IDOK)
	{
		//COLORREF color = dlg.GetColor();
		//GLfloat m_Red = (GLfloat) GetRValue(color);
		//GLfloat m_Green = (GLfloat) GetGValue(color);
		//GLfloat m_Blue = (GLfloat) GetBValue(color);
	}
}


void CLightDlg::OnBnClickedButtonnew()
{
	// TODO: 在此添加控件通知处理程序代码
	n=m_listliht.GetCount();
	char buff[10];
	itoa(n,buff,10);
	char A[10]="LIGHT";
	strcat(A,buff);
	CString strMsg(A);
    m_listliht.AddString(strMsg);
	light[n].onlight=true;
	light[n].posiMat[0]=m_x;
    light[n].posiMat[1]=m_y;
	light[n].posiMat[2]=m_z;
	light[n].posiMat[3]=m_w;
	light[n].exponent[1]=m_e;
	light[n].cutoff[1]=m_c;
	//return TRUE;
}


void CLightDlg::OnBnClickedButtondelete()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex=m_listliht.GetCurSel();
	m_listliht.DeleteString (nIndex);
	light[n].onlight=false;
}
