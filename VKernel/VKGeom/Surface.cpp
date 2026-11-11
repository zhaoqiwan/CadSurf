// Surface.cpp: implementation of the CSurface class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Surface.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSurface::CSurface()
{
	geomType = CSURFACE;
}

CSurface::~CSurface()
{

}

SurfaceType CSurface::Type() const
{
	return sType;
}

CVector3D CSurface::NormalAt(const double uPar, const double vPar)
{
	CPoint3D Po = PointAtPara(uPar, vPar);
	CPoint3D Pu = PointAtPara(uPar+0.000001, vPar);
	CPoint3D Pv = PointAtPara(uPar+0.000001, vPar+0.000001);
	CVector3D Vu(Po, Pu);
	CVector3D Vv(Po, Pv);
	CVector3D N = Vu^Vv;
	N.Normalize();
	return N;
}

bool CSurface::IsOfType(const SurfaceType& type) const
{
	SurfaceType typ = type;
	bool b = (sType == typ);
	return b;
}
