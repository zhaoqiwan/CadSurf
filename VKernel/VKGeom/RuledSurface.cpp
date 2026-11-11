// RuledSurface.cpp: implementation of the CRuledSurface class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "RuledSurface.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRuledSurface::CRuledSurface(const CCurve* C1, const CCurve* C2)
{
	sType = RULED;
	baseCurve = C1->Copy();
	secondRail = C2->Copy();
}

CRuledSurface::~CRuledSurface()
{
	if(baseCurve)
		delete baseCurve;
	if(secondRail)
		delete secondRail;
}

CCurve* CRuledSurface::RailCurve1() const
{
	return baseCurve;
}

CCurve* CRuledSurface::RailCurve2() const
{
	return secondRail;
}

#include "Line3D.h"
CPoint3D CRuledSurface::PointAtPara(const double uPar, const double vPar)
{
	double uParR1 = baseCurve->FirstParameter() + 
		((baseCurve->LastParameter() - baseCurve->FirstParameter())*uPar);
	double uParR2 = secondRail->FirstParameter() +
		((secondRail->LastParameter() - secondRail->FirstParameter())*uPar);

	CPoint3D P1 = baseCurve->PointAtPara(uParR1);
	CPoint3D P2 = secondRail->PointAtPara(uParR2);
	CLine3D L(P1, P2);
	CPoint3D P = L.PointAtPara(vPar);
	return P;
}

CVector3D CRuledSurface::NormalAt(const double uPar, const double vPar)
{
	CPoint3D Po, Pu, Pv;
	CVector3D N;

	if(uPar == 1 && vPar == 1)
	{
		Po = PointAtPara(uPar, vPar);
		Pu = PointAtPara(uPar-0.001, vPar);
		Pv = PointAtPara(uPar, vPar-0.001);
		CVector3D Vu(Po, Pu), Vv(Po, Pv);
		N = Vu^Vv;
		return N;
	}
	else if(uPar == 1 && vPar != 1)
	{
		Po = PointAtPara(uPar, vPar);
		Pu = PointAtPara(uPar-0.001, vPar);
		Pv = PointAtPara(uPar, vPar+0.001);
		CVector3D Vu(Po, Pu), Vv(Po, Pv);
		N = Vv^Vu;
		return N;
	}
	else if(uPar != 1 && vPar == 1)
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

double CRuledSurface::FirstUParameter() const
{
	return 0;
}

double CRuledSurface::LastUParameter() const
{
	return 1;
}

double CRuledSurface::FirstVParameter() const
{
	return 0;
}

double CRuledSurface::LastVParameter() const
{
	return 1;
}

bool CRuledSurface::IsUClosed() const
{
	return false;
}

bool CRuledSurface::IsVClosed() const
{
	return false;
}

CSurface* CRuledSurface::Copy() const
{
	CRuledSurface* R = new CRuledSurface(baseCurve, secondRail);
	return R;
}

void CRuledSurface::Translate(const COneAxis& Ax, const double& amt)
{
	CGeometry::Translate(Ax, amt);
}

void CRuledSurface::Translate(double dx, double dy, double dz)
{
	baseCurve->Translate(dx,dy,dz);
	secondRail->Translate(dx,dy,dz);
}

void CRuledSurface::Translate(const CVector3D& V)
{
	baseCurve->Translate(V);
	secondRail->Translate(V);
}

void CRuledSurface::Translate(const CPoint3D& P1, const CPoint3D& P2)
{
	baseCurve->Translate(P1, P2);
	secondRail->Translate(P1, P2);
}

void CRuledSurface::Rotate(const COneAxis& Ax, double ang)
{
	baseCurve->Rotate(Ax, ang);
	secondRail->Rotate(Ax, ang);
}

void CRuledSurface::Scale(const CPoint3D& P, double fact)
{
	baseCurve->Scale(P, fact);
	secondRail->Scale(P, fact);
}

void CRuledSurface::Mirror(const CPoint3D& P)
{
	baseCurve->Mirror(P);
	secondRail->Mirror(P);
}

void CRuledSurface::Mirror(const COneAxis& Ax)
{
	baseCurve->Mirror(Ax);
	secondRail->Mirror(Ax);
}

void CRuledSurface::Mirror(const CPlane& Pln)
{
	baseCurve->Mirror(Pln);
	secondRail->Mirror(Pln);
}

