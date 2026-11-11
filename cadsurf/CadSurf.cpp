// CadSurf.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CadSurf.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "CadSurfDoc.h"
#include "CadSurfView.h"
#include "Splash.h"
#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCadSurfApp

BEGIN_MESSAGE_MAP(CCadSurfApp, CWinApp)
	//{{AFX_MSG_MAP(CCadSurfApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCadSurfApp construction

CCadSurfApp::CCadSurfApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCadSurfApp object

CCadSurfApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCadSurfApp initialization

BOOL CCadSurfApp::InitInstance()
{
	InitCommonControls();    // initialize common control library
	CWinApp::InitInstance(); // call parent class method

	// CG: The following block was inserted by 'Status Bar' component.
	{
		//Set up date and time defaults so they're the same as system defaults
		setlocale(LC_ALL, "");
	}

	// CG: The following block was added by the Splash Screen component.
\
	{
\
		CCommandLineInfo cmdInfo;
\
		ParseCommandLine(cmdInfo);
\

\
		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
\
	}
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.


	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_CADSURTYPE,
		RUNTIME_CLASS(CCadSurfDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CCadSurfView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str;
	str.Format("##########################\
Author:	Sharjith Nair                  \
Email:	sharjith@gmail.com \
Program:	OpenGL, C++ MFC Surface Modeler Demo Application \
						Important Notice: \
	This Idea and the Application is Copyright(c) Sharjith Nair 2002, 2006. \
	You can freely use it as long as you credit me for it. \
	No guarantee/warantee is given on this app and I will not be responsible \
	for any damage to you, your property or any other person from using it. \
	USE IT ON YOUR OWN RISK. \
	Thankyou - \
	Sharjith Nair. \
############################################################################# \
This Software Is Intended For Demonstrating The Use OF OpenGL \
And Geometry For Cad Type Applications. Almost All The Coding Is Done By me \
And The Respective Names Of The Programmers Of Whom The Codes Are Used Are Mentioned. \
Most Codes Or Ideas That Are Not Programmed By me Are Taken From Sites Like \
www.codeguru.com, www.codeproject.com, www.programmersheaven.com etc. \
I Sincierly Thank All Of Them. Following Are Some Guidlines For Using The Software \
(1)Use Mouse Button 1 For Zooming With Control Key Pressed  \
(2)Use Mouse Button 2 For Panning With Control Key Pressed  \
(3)Use Mouse Button 3 For Rotating With Control Key Pressed  \
(5)Use Mouse Button 1 For Selection Without Any Key Pressed  \
(6)Use Mouse Button 1 For Multi Selection With Shift Key Pressed  \
(7)Press Mouse Button 1 And Drag For Sweep Selection Without Any Key Pressed  \
(8)Actions Like Window Zooming, Best Fit, Different Views, Display Modes In \
Wireframe, Shading And Hidden Line Removed Mode Are Provided In The Child Frame ToolBar \
(9)The ToolBar In The MainFrame Has Buttons For Creating Geometry Implemented Currently \
In Non Interactive Mode. You Can Freely Modify The Code To Make Interactive Applications.");
	CWnd* pWnd = GetDlgItem(IDC_EDIT1);
	pWnd->SetWindowText(str);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CCadSurfApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CCadSurfApp message handlers

BOOL CCadSurfApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}
