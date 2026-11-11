// Conic.cpp: implementation of the CConic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Conic.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConic::CConic()
{

}

CConic::~CConic()
{

}

void CConic::Reverse()
{
	CVector3D V = itsLocation.GetDirection();
	V *= -1;
	itsLocation.SetDirection(V);
}

void CConic::SetLocation(const CAxisSystem& ax)
{
	itsLocation = ax;
}

