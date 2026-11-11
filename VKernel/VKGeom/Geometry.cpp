// Geometry.cpp: implementation of the CGeometry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Geometry.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "Line3D.h"
#include "Plane.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CGeometry::refCount = 0;

CGeometry::CGeometry()
{
	refCount++;
}

CGeometry::~CGeometry()
{
	refCount--;
}

void CGeometry::Translate(const COneAxis& Ax, const double& amt)
{
	CVector3D D = Ax.GetDirection();
	double dx, dy, dz;
	dx = D.GetX(); dy = D.GetY(); dz = D.GetZ();
	dx *= amt; dy *= amt; dz *= amt;
	Translate(dx,dy,dz);
}
