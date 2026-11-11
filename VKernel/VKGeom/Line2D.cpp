// Line2D.cpp: implementation of the CLine2D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Line2D.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLine2D::CLine2D()
{
	cType = LINE2D;
}

CLine2D::CLine2D(const CPoint2D& sP, const CPoint2D& eP) : startPoint(sP), endPoint(eP)
{
	COneAxis2D Ax(sP, CVector2D(sP, eP));
	itsLocation = Ax;
	cType = LINE2D;
}

CLine2D::CLine2D(const CPoint2D& P, const CVector2D& V)
{
	CVector2D dir = V;
	CVector2D sv = dir.Unit() * -INFINITY;
	CVector2D ev = dir.Unit() * INFINITY;
	sv.SetParam(sv.GetX()+P.GetX(), sv.GetY()+P.GetY());
	ev.SetParam(ev.GetX()+P.GetX(), ev.GetY()+P.GetY());

	startPoint.SetParam(sv.GetX(), sv.GetY());
	endPoint.SetParam(ev.GetX(), ev.GetY());

	COneAxis2D Ax(startPoint, CVector2D(sv, ev));
	itsLocation = Ax;

	cType = LINE2D;
}

CLine2D::~CLine2D()
{
}

void CLine2D::Reverse()
{
	CPoint2D tPnt = startPoint;
	startPoint = endPoint;
	endPoint = tPnt;
}

CPoint2D CLine2D::PointAtPara(const double& par)
{
	double u;
	if (par < 0)
		u = 0;
	else if(par > 1)
		u = 1;
	else
		u = par;

	CPoint2D aPnt;
	aPnt = startPoint + (endPoint - startPoint) * u;
	return aPnt;
}

bool CLine2D::IsClosed() const
{
	return false;
}

bool CLine2D::operator == (const CLine2D& L) const
{
	bool eq = (startPoint == L.StartPoint() && endPoint == L.EndPoint());
	return eq;
}

void CLine2D::operator = (const CLine2D& L)
{
	itsLocation = L.GetLocation();
	startPoint = L.StartPoint();
	endPoint = L.EndPoint();
}

CVector2D CLine2D::Direction() const
{
	CVector2D sv(startPoint);
	CVector2D ev(endPoint);
	CVector2D dir(sv, ev);
	return dir.Unit();
}

CVector2D CLine2D::NormalThrough(const CPoint2D& P)
{
	CPoint2D P3 = P;
	CPoint2D P1 = startPoint;
	double D = Distance(P3);
	D = 1/D;
	CVector2D N1 = Direction();
	CVector2D PV = CVector2D(P3 - P1);
	double dt = PV.Dot(N1);
	CVector2D N = (CVector2D(P3 - P1) - N1*(dt)) * D;
	return N;
}

double CLine2D::Distance(const CPoint2D& P)
{
	CPoint2D P3 = P;
	CPoint2D P1 = startPoint;
	CPoint2D P2 = endPoint;
	double x1, y1, x2, y2, x3, y3;
	x1 = P1.GetX(); y1 = P1.GetY();
	x2 = P2.GetX(); y2 = P2.GetY();
	x3 = P3.GetX(); y3 = P3.GetY();
	double L = P1.Distance(P2);
	L = 1/L;
	double D = L * (sqrt(pow((x3-x1)*(y2-y1)-(y3-y1)*(x2-x1),2)));
	return D;
}

void CLine2D::Translate(const COneAxis2D& Ax, const double& amt)
{
	CGeometry2D::Translate(Ax, amt);
}

void CLine2D::Translate(double dx, double dy)
{
	startPoint.Translate(dx,dy);
	endPoint.Translate(dx,dy);
}

void CLine2D::Translate(const CVector2D& V)
{
	startPoint.Translate(V);
	endPoint.Translate(V);
}

CCurve2D* CLine2D::Copy() const
{
	CLine2D* L = new CLine2D(startPoint, endPoint);
	return L;
}

void CLine2D::Translate(const CPoint2D& P1, const CPoint2D& P2)
{
	startPoint.Translate(P1, P2);
	endPoint.Translate(P1, P2);
}

void CLine2D::Rotate(const CPoint2D& Ax, double ang)
{
	startPoint.Rotate(Ax, ang);
	endPoint.Rotate(Ax, ang);
}

void CLine2D::Scale(const CPoint2D& P, double fact)
{
	startPoint.Scale(P, fact);
	endPoint.Scale(P, fact);
}

void CLine2D::Mirror(const CPoint2D& P)
{
	startPoint.Mirror(P);
	endPoint.Mirror(P);
}

void CLine2D::Mirror(const COneAxis2D& Ax)
{
	startPoint.Mirror(Ax);
	endPoint.Mirror(Ax);
}

