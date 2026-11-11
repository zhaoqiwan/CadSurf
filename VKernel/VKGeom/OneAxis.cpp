// OneAxis.cpp: implementation of the COneAxis class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "OneAxis.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//Implementation of class OneAxis 
//(Used for construction of revolutions and as axis of symmetries)
COneAxis::COneAxis()
{
	Position = CPoint3D(0,0,0);
	Direction = CVector3D(0,0,1);
	Direction.Normalize();
	Origin.SetParam(0,0,0);
	geomType = CONEAXIS;
}

COneAxis::COneAxis(const CPoint3D& Pos, const CVector3D& Dir) : Position(Pos), Direction(Dir)
{
	Direction.Normalize();
	Origin.SetParam(Pos.GetX(),Pos.GetY(),Pos.GetZ());
	geomType = CONEAXIS;
}

COneAxis::~COneAxis()
{
}

void COneAxis::SetPosition(const CPoint3D& P)
{
	Position = P;
	Origin = P;
}

void COneAxis::SetPosition(double x, double y, double z)

{
	Position.SetParam(x,y,z);
	Origin.SetParam(x,y,z);
}

void COneAxis::SetDirection(const CVector3D& D)
{
	Direction = D;
	Direction.Normalize();
}

void COneAxis::SetDirection(double dx, double dy, double dz)
{
	Direction.SetParam(dx,dy,dz);
	Direction.Normalize();
}

CPoint3D COneAxis::GetPosition() const
{
	return Position;
}

CVector3D COneAxis::GetOrigin() const
{
	return Origin;
}

CVector3D COneAxis::GetDirection() const
{
	return Direction;
}

bool COneAxis::operator == (const COneAxis& rhAx) const
{
	bool eq = (Position == rhAx.GetPosition() && Direction == rhAx.GetDirection()
				&& Origin == rhAx.GetOrigin());
	return eq;
}

void COneAxis::Translate(const COneAxis& Ax, const double& amt)
{
	CGeometry::Translate(Ax, amt);
}

void COneAxis::Translate(double dx, double dy, double dz)
{
	CPoint3D P = Position;
	P.Translate(dx, dy, dz);
	SetPosition(P);
	Origin.SetParam(P.GetX(), P.GetY(), P.GetZ());
}

void COneAxis::Translate(const CVector3D& V)
{
	CPoint3D P = Position;
	P.Translate(V);
	SetPosition(P);
	Origin.SetParam(P.GetX(), P.GetY(), P.GetZ());
}

void COneAxis::Translate(const CPoint3D& P1, const CPoint3D& P2)
{
	CPoint3D P = Position;
	P.Translate(P1, P2);
	SetPosition(P);
	Origin.SetParam(P.GetX(), P.GetY(), P.GetZ());
}

void COneAxis::Rotate(const COneAxis& Ax, double ang)
{
	CPoint3D P = Position;
	CVector3D D = Direction;
	P.Rotate(Ax, ang);
	D.Rotate(Ax, ang);
	SetPosition(P);
	SetDirection(D);
	Origin.SetParam(P.GetX(), P.GetY(), P.GetZ());
}

void COneAxis::Scale(const CPoint3D& Pnt, double fact)
{
	CPoint3D P = Position;
	CVector3D D = Direction;
	P.Scale(Pnt, fact);
	D.Scale(Pnt, fact);
	SetPosition(P);
	SetDirection(D);
	Origin.SetParam(P.GetX(), P.GetY(), P.GetZ());
}

void COneAxis::Mirror(const CPoint3D& Pnt)
{
	CPoint3D P = Position;
	CVector3D D = Direction;
	P.Mirror(Pnt);
	D.Mirror(Pnt);
	SetPosition(P);
	SetDirection(D);
	Origin.SetParam(P.GetX(), P.GetY(), P.GetZ());
}

void COneAxis::Mirror(const COneAxis& Ax)
{
	CPoint3D P = Position;
	CVector3D D = Direction;
	/*CString str;
	str.Format(" Now Direction X %0.3lf Y %0.3lf Z %0.3lf", D.GetX(),D.GetY(),D.GetZ());
	MessageBox(0, str, "From Axis", MB_OK);*/
	P.Mirror(Ax);
	CVector3D dir = Ax.GetDirection();
	if(dir.IsParallel(D))
		D.Mirror(Ax.GetDirection());
	else
		D.Mirror(Ax);
	/*str.Format(" Now Direction X %0.3lf Y %0.3lf Z %0.3lf", D.GetX(),D.GetY(),D.GetZ());
	MessageBox(0, str, "From Axis", MB_OK);*/
	SetPosition(P);
	SetDirection(D);
	Origin.SetParam(P.GetX(), P.GetY(), P.GetZ());
}

void COneAxis::Mirror(const CPlane& Pln)
{
	CPoint3D P = Position;
	CVector3D D = Direction;
	P.Mirror(Pln);
	D.Mirror(Pln);
	SetPosition(P);
	SetDirection(D);
	Origin.SetParam(P.GetX(), P.GetY(), P.GetZ());
}

COneAxis COneAxis::OX()
{
	COneAxis Ax(CPoint3D(0,0,0), CVector3D(1,0,0));
	return Ax;
}

COneAxis COneAxis::OY()
{
	COneAxis Ax(CPoint3D(0,0,0), CVector3D(0,1,0));
	return Ax;
}

COneAxis COneAxis::OZ()
{
	COneAxis Ax(CPoint3D(0,0,0), CVector3D(0,0,1));
	return Ax;
}

