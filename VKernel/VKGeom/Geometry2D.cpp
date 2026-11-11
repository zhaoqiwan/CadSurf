// Geometry.cpp: implementation of the CGeometry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Geometry2D.h"
#include "Point2D.h"
#include "Vector2D.h"
#include "OneAxis2D.h"
//#include "Line2D.h"
//#include "Plane.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CGeometry2D::refCount = 0;

CGeometry2D::CGeometry2D()
{
	refCount++;
}

CGeometry2D::~CGeometry2D()
{
	refCount--;
}

void CGeometry2D::Translate(const COneAxis2D& Ax, const double& amt)
{
	CVector2D D = Ax.GetDirection();
	double dx, dy;
	dx = D.GetX(); dy = D.GetY();
	dx *= amt; dy *= amt;
	Translate(dx,dy);
}

