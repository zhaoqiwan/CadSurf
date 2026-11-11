#include "stdafx.h"
#include "math.h"

#include "Point3D.h"
#include "Command.h"

#include "CreateCmd.h"
#include "GLCurve.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "GLView.h"

extern CGLView * g_pGLView;

CCreateLine::CCreateLine()
	: m_begin(0,0,0), m_end(0,0,0)
{
	m_nStep = 0; // 初始化操作步为 0

	m_pTmpLine = NULL;
}

CCreateLine::~CCreateLine()
{

}

void CCreateLine::RemovePreview()
{
	if (m_pTmpLine)
	{
		g_pGLView->RemoveEntity(m_pTmpLine);
		m_pTmpLine = NULL;
	}
}

int CCreateLine::GetType()
{
	return ctCreateLine; 
}

int	CCreateLine::OnLButtonDown(UINT nFlags, const CPoint3D& pos) 
{
	m_nStep ++; // 每次单击鼠标左键时操作步加 1 


	switch(m_nStep) // 根据操作步执行相应的操作
	{
		case 1: 
		{
			m_begin = m_end = pos;

			::Prompt("请输入直线的末点：");
			break;
		}
		default:
		{
			m_end = pos;
			CLine3D ln(m_begin, m_end);

			CGLCurve* lin1 = new CGLCurve(&ln);
			//lin1->Display();
			RemovePreview();
			g_pGLView->AddEntity(lin1);
			//delete lin1;

			m_nStep = 0;
			::Prompt("请输入直线的起点：");
			break;
		}	
		
	}


	return 0;
}

//鼠标左键双击创建多线
int CCreateLine::OnLButtonDblClk(UINT nFlags, const CPoint3D& pos)
{
	
	return 0;
}

int	CCreateLine::OnMouseMove(UINT nFlags, const CPoint3D& pos)
{
	//::SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW_LINE));

	CCommand::OnMouseMove(nFlags,  pos);

	switch(m_nStep)
	{
		case 0:
			::Prompt("请输入直线的起点：");
			break;
		default:
		{
			//g_pGLView->MakeResource();
			::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  //此函数实现擦画功能

 			CLine3D ln(m_begin, pos);
			CGLCurve* lin1 = new CGLCurve(&ln);

			//::glFinish();
			RemovePreview();
			g_pGLView->AddEntity(lin1);

			g_pGLView->Draw();

			g_pGLView->SwapBuffers();
			//g_pGLView->ClearResource();

			m_pTmpLine = lin1;
			m_end = pos; // 将当前位置设置为直线终点，以备下一次鼠标移动时用
			break;
		}
	}

	return 0;
}

// 单击鼠标右键取消当前的操作
int	CCreateLine::OnRButtonDown(UINT nFlags, const CPoint3D& pos) 
{
	RemovePreview();
	m_nStep = 0; // 将操作步重置为 0 
	::Prompt("就绪");
	return 0;
}

// 调用Cancel 函数取消本次操作
int CCreateLine::Cancel()
{
	RemovePreview();
	m_nStep = 0; // 将操作步重置为 0 
	::Prompt("就绪"); // 等待提示新类型的命令操作
	return 0 ;
}

