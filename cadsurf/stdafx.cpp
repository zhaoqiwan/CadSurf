// stdafx.cpp : source file that includes just the standard includes
//	CadSurf.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// 功能：在状态栏上给出操作过程的提示信息
void Prompt(char* pmt)
{
	CStatusBar* pStatus=(CStatusBar*)
		AfxGetApp()->m_pMainWnd->GetDescendantWindow(ID_VIEW_STATUS_BAR);

	ASSERT(pStatus) ;
	if(pStatus)
		pStatus->SetPaneText(0, pmt, TRUE);
}