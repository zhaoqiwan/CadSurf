// PipeSurface.cpp: implementation of the CPipeSurface class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "PipeSurface.h"
#include "Point3D.h"
#include "Circle3D.h"
#include "Ellipse3D.h"
#include "OneAxis.h"
#include "AxisSystem.h"
#include "GeomException.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPipeSurface::CPipeSurface()
{
	sType = PIPE;
}

CPipeSurface::CPipeSurface(const CCurve* C, const double& radius) : itsRadius(radius)
{
	sType = PIPE;
	circular = true;
	baseCurve = C->Copy();
}

CPipeSurface::CPipeSurface(const CCurve* C, const double& majrad, const double& minrad) : itsRadius(minrad), 
																						itsMajorRadius(majrad)
{
	sType = PIPE;
	circular = false;
	baseCurve = C->Copy();
}

CPipeSurface::~CPipeSurface()
{
	if(baseCurve)
		delete baseCurve;
}

CPoint3D CPipeSurface::PointAtPara(const double uPar, const double vPar)
{
	CPoint3D P;
	CPoint3D O = baseCurve->PointAtPara(uPar);
	CPoint3D D;
	CVector3D Dir;
	if(uPar == 1)
	{
		D = baseCurve->PointAtPara(uPar-0.001);
		Dir = CVector3D(O, D);
	}
	else
	{
		D = baseCurve->PointAtPara(uPar+0.001);
		Dir = CVector3D(D, O);
	}
	CPoint3D fp = baseCurve->PointAtPara(baseCurve->FirstParameter());
	CPoint3D dfp = baseCurve->PointAtPara(baseCurve->FirstParameter()+0.001);
	CVector3D Z(fp, dfp);
	CAxisSystem Ax(O, Z);
		
	if(circular)
	{
		CCircle3D C(Ax, itsRadius);
		if(uPar == baseCurve->FirstParameter())
			C.Reverse();
		P = C.PointAtPara(vPar);
	}
	else
	{
		CEllipse3D C(Ax, itsMajorRadius, itsRadius);
		if(uPar == baseCurve->FirstParameter())
			C.Reverse();
		P = C.PointAtPara(vPar);
	}

	CVector3D Ref = Dir^Z;
	double ang = Dir.Angle(Z, Ref);

	P.Translate(Ax.GetPosition(), O);
	P.Rotate(COneAxis(O, Ref), -ang);

	return P;
}

CVector3D CPipeSurface::NormalAt(const double uPar, const double vPar)
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
double CPipeSurface::FirstUParameter() const
{
	return baseCurve->FirstParameter();
}

double CPipeSurface::LastUParameter() const
{
	return baseCurve->LastParameter();
}

double CPipeSurface::FirstVParameter() const
{
	return 0;
}

double CPipeSurface::LastVParameter() const
{
	return 2*PI;
}

bool CPipeSurface::IsUClosed() const
{
	return false;
}

bool CPipeSurface::IsVClosed() const
{
	return false;
}

CSurface* CPipeSurface::Copy() const
{
	CPipeSurface* P;
	if(circular)
		P = new CPipeSurface(baseCurve, itsRadius);
	else
		P = new CPipeSurface(baseCurve, itsMajorRadius, itsRadius);
	return P;
}

void CPipeSurface::Translate(const COneAxis& Ax, const double& amt)
{
	CGeometry::Translate(Ax, amt);
}

void CPipeSurface::Translate(double dx, double dy, double dz)
{
	baseCurve->Translate(dx,dy,dz);
}

void CPipeSurface::Translate(const CVector3D& V)
{
	baseCurve->Translate(V);
}

void CPipeSurface::Translate(const CPoint3D& P1, const CPoint3D& P2)
{
	baseCurve->Translate(P1, P2);
}

void CPipeSurface::Rotate(const COneAxis& Ax, double ang)
{
	baseCurve->Rotate(Ax, ang);
}

void CPipeSurface::Scale(const CPoint3D& P, double fact)
{
	baseCurve->Scale(P, fact);
}

void CPipeSurface::Mirror(const CPoint3D& P)
{
	baseCurve->Mirror(P);
}

void CPipeSurface::Mirror(const COneAxis& Ax)
{
	baseCurve->Mirror(Ax);
}

void CPipeSurface::Mirror(const CPlane& Pln)
{
	baseCurve->Mirror(Pln);
}
