// RevolvedSurface.cpp: implementation of the CRevolvedSurface class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "RevolvedSurface.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "Line3D.h"
#include "AxisSystem.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRevolvedSurface::CRevolvedSurface(const CCurve* C, const COneAxis& Ax, const double& st, const double& en) : itsAxisOfRevol(Ax), startAngle(st), endAngle(en)
{
	sType = REVOLVED;
	baseCurve = C->Copy();
}

CRevolvedSurface::~CRevolvedSurface()
{
	if(baseCurve)
		delete baseCurve;
}

CPoint3D CRevolvedSurface::PointAtPara(const double uPar, const double vPar)
{
	CPoint3D P;
	CPoint3D CP = baseCurve->PointAtPara(uPar);
	COneAxis ax = itsAxisOfRevol;
	CP.Rotate(ax, vPar);
	P = CP;
	return P;
}

CVector3D CRevolvedSurface::NormalAt(const double uPar, const double vPar)
{
	CPoint3D Po, Pu, Pv;
	CVector3D N;

	if(uPar == LastUParameter() && vPar == LastVParameter())
	{
		Po = PointAtPara(uPar, vPar);
		Pu = PointAtPara(uPar-0.001, vPar);
		Pv = PointAtPara(uPar, vPar-0.001);
		CVector3D Vu(Po, Pu), Vv(Po, Pv);
		N = Vu^Vv;
		return N;
	}
	else if(uPar == LastUParameter() && vPar != LastVParameter())
	{
		Po = PointAtPara(uPar, vPar);
		Pu = PointAtPara(uPar-0.001, vPar);
		Pv = PointAtPara(uPar, vPar+0.001);
		CVector3D Vu(Po, Pu), Vv(Po, Pv);
		N = Vv^Vu;
		return N;
	}
	else if(uPar != LastUParameter() && vPar == LastVParameter())
	{
		Po = PointAtPara(uPar, vPar);
		Pu = PointAtPara(uPar+0.001, vPar);
		Pv = PointAtPara(uPar, vPar-0.001);
		CVector3D Vu(Po, Pu), Vv(Po, Pv);
		N = Vv^Vu;
		return N;
	}
	else
	{
		Po = PointAtPara(uPar, vPar);
		Pu = PointAtPara(uPar+0.001, vPar);
		Pv = PointAtPara(uPar, vPar+0.001);
		CVector3D Vu(Po, Pu), Vv(Po, Pv);
		N = Vu^Vv;
		return N;
	}
}

double CRevolvedSurface::FirstUParameter() const
{
	return baseCurve->FirstParameter();
}

double CRevolvedSurface::LastUParameter() const
{
	return baseCurve->LastParameter();
}

double CRevolvedSurface::FirstVParameter() const
{
	return startAngle;
}

double CRevolvedSurface::LastVParameter() const
{
	return endAngle;
}

bool CRevolvedSurface::IsUClosed() const
{
	return false;
}

bool CRevolvedSurface::IsVClosed() const
{
	bool b = ((endAngle - startAngle) - (2 * PI) == 0.0);
	return b;
}

CSurface* CRevolvedSurface::Copy() const
{
	CRevolvedSurface* R = new CRevolvedSurface(baseCurve, itsAxisOfRevol, startAngle, endAngle);
	return R;
}

void CRevolvedSurface::Translate(const COneAxis& Ax, const double& amt)
{
	CGeometry::Translate(Ax, amt);
}

void CRevolvedSurface::Translate(double dx, double dy, double dz)
{
	baseCurve->Translate(dx,dy,dz);
	itsAxisOfRevol.Translate(dx,dy,dz);
}

void CRevolvedSurface::Translate(const CVector3D& V)
{
	baseCurve->Translate(V);
	itsAxisOfRevol.Translate(V);
}

void CRevolvedSurface::Translate(const CPoint3D& P1, const CPoint3D& P2)
{
	baseCurve->Translate(P1, P2);
	itsAxisOfRevol.Translate(P1, P2);
}

void CRevolvedSurface::Rotate(const COneAxis& Ax, double ang)
{
	baseCurve->Rotate(Ax, ang);
	itsAxisOfRevol.Rotate(Ax, ang);
}

void CRevolvedSurface::Scale(const CPoint3D& P, double fact)
{
	baseCurve->Scale(P, fact);
	itsAxisOfRevol.Scale(P, fact);
}

void CRevolvedSurface::Mirror(const CPoint3D& P)
{
	baseCurve->Mirror(P);
	itsAxisOfRevol.Mirror(P);
}

void CRevolvedSurface::Mirror(const COneAxis& Ax)
{
	baseCurve->Mirror(Ax);
	itsAxisOfRevol.Mirror(Ax);
}

void CRevolvedSurface::Mirror(const CPlane& Pln)
{
	baseCurve->Mirror(Pln);
	itsAxisOfRevol.Mirror(Pln);
}
