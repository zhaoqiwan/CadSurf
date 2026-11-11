// ExtrudedSurface.cpp: implementation of the CExtrudedSurface class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "ExtrudedSurface.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExtrudedSurface::CExtrudedSurface(const CCurve* C, const CVector3D& Dir, const double& s, const double& e) : startLimit(s), endLimit(e)
{
	sType = EXTRUDED;
	baseCurve = C->Copy();
	itsDirection = Dir;
}

CExtrudedSurface::~CExtrudedSurface()
{
	if(baseCurve)
		delete baseCurve;
}

CPoint3D CExtrudedSurface::PointAtPara(const double uPar, const double vPar)
{
	CVector3D P;
	CVector3D G(baseCurve->PointAtPara(uPar));
	CVector3D Dir = itsDirection*vPar;
	P = G+Dir;
	return P.Point();
}

double CExtrudedSurface::FirstUParameter() const
{
	return baseCurve->FirstParameter();
}

double CExtrudedSurface::LastUParameter() const
{
	return baseCurve->LastParameter();
}

double CExtrudedSurface::FirstVParameter() const
{
	return startLimit;
}

double CExtrudedSurface::LastVParameter() const
{
	return endLimit;
}

bool CExtrudedSurface::IsUClosed() const
{
	return false;
}

bool CExtrudedSurface::IsVClosed() const
{
	return false;
}

CSurface* CExtrudedSurface::Copy() const
{
	CExtrudedSurface* R = new CExtrudedSurface(baseCurve, itsDirection, startLimit, endLimit);
	return R;
}

void CExtrudedSurface::Translate(const COneAxis& Ax, const double& amt)
{
	CGeometry::Translate(Ax, amt);
}

void CExtrudedSurface::Translate(double dx, double dy, double dz)
{
	baseCurve->Translate(dx,dy,dz);
}

void CExtrudedSurface::Translate(const CVector3D& V)
{
	baseCurve->Translate(V);
}

void CExtrudedSurface::Translate(const CPoint3D& P1, const CPoint3D& P2)
{
	baseCurve->Translate(P1, P2);
}

void CExtrudedSurface::Rotate(const COneAxis& Ax, double ang)
{
	baseCurve->Rotate(Ax, ang);
}

void CExtrudedSurface::Scale(const CPoint3D& P, double fact)
{
	baseCurve->Scale(P, fact);
}

void CExtrudedSurface::Mirror(const CPoint3D& P)
{
	baseCurve->Mirror(P);
}

void CExtrudedSurface::Mirror(const COneAxis& Ax)
{
	baseCurve->Mirror(Ax);
}

void CExtrudedSurface::Mirror(const CPlane& Pln)
{
	baseCurve->Mirror(Pln);
}
