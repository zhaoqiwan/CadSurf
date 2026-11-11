// Plane.cpp: implementation of the CPlane class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "Plane.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "OneAxis.h"
#include "GeomException.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlane::CPlane(const CPoint3D& P1, const CPoint3D& P2, const CPoint3D& P3)
{
	CVector3D V1(P1, P2);
	CVector3D V2(P1, P3);
	CVector3D Dir = V1^V2;

	CAxisSystem ax(P1, Dir, V1);
	itsLocation = ax;

	UpdateEquation();
	
	geomType = CPLANE;
}


CPlane::CPlane(const CPoint3D& P1, const CVector3D& V1, const CVector3D& V2)
{
	CVector3D v1 = V1, v2 = V2;
	CVector3D Dir = v1^v2;

	CAxisSystem ax(P1, Dir, v1);
	itsLocation = ax;

	UpdateEquation();
	
	geomType = CPLANE;
}

CPlane::CPlane(const double& A, const double& B, const double& C, const double& D)
{
	Equation[0] = A;
	Equation[1] = B;
	Equation[2] = C;
	Equation[3] = D;

	CPoint3D Pos(A, B, C);
	CVector3D Dir(A, B, C);

	
	CAxisSystem location(Pos, Dir);

	double d = location.GetDirection().Magnitude();
	if ( d > 0.0 )
	{
		d = 1.0/d;
		CVector3D O = location.GetDirection()*(-d)*Equation[3];
		location.SetPosition(O.Point());
	}

	itsLocation = location;
	
	geomType = CPLANE;
}

CPlane::CPlane(const COneAxis& Ax)
{
	COneAxis ax = Ax;

	CPoint3D P = Ax.GetPosition();
	CVector3D V = Ax.GetDirection();

	CAxisSystem location(P, V);

	itsLocation = location;

	UpdateEquation();
	geomType = CPLANE;
}

CPlane::~CPlane()
{
}

CPoint3D CPlane::PointAtPara(const double uPar, const double vPar)
{
	CVector3D V;
	V = itsLocation.GetOrigin() + itsLocation.GetXDirection() * uPar + itsLocation.GetYDirection() * vPar;
	/*CString str;
	str.Format("YDirection %0.3lf, %0.3lf, %0.3lf",itsLocation.GetYDirection().GetX(),itsLocation.GetYDirection().GetY(),itsLocation.GetYDirection().GetZ());
	MessageBox(0, str, "From Plane", MB_OK);*/
	return V.Point();
}

CPoint3D CPlane::GetPosition() const
{
	return itsLocation.GetPosition();
}

CVector3D CPlane::GetDirection() const
{
	return itsLocation.GetDirection();
}

void CPlane::EquationAt(const CPoint3D& P, double* e)
{
	double x = P.GetX();
	double y = P.GetY();
	double z = P.GetZ();
	e[0] = Equation[0]*x;
	e[1] = Equation[1]*y;
	e[2] = Equation[2]*z;
	e[3] = Equation[3];
}

double CPlane::GetConstant() const
{
	return Equation[3];
}

void CPlane::Translate(const COneAxis& Ax, const double& amt)
{
	CGeometry::Translate(Ax, amt);
}

void CPlane::Translate(double dx, double dy, double dz)
{
	itsLocation.Translate(dx, dy, dz);
}

void CPlane::Translate(const CVector3D& V)
{
	itsLocation.Translate(V);
}

void CPlane::Translate(const CPoint3D& P1, const CPoint3D& P2)
{
	itsLocation.Translate(P1, P2);
}

void CPlane::Rotate(const COneAxis& Ax, double ang)
{
	itsLocation.Rotate(Ax, ang);
}

void CPlane::Scale(const CPoint3D& P, double fact)
{
	itsLocation.Scale(P, fact);
}

void CPlane::Mirror(const CPoint3D& P)
{
	itsLocation.Mirror(P);
}

void CPlane::Mirror(const COneAxis& Ax)
{
	itsLocation.Mirror(Ax);
}

void CPlane::Mirror(const CPlane& Pln)
{
	itsLocation.Mirror(Pln);
}

void CPlane::UpdateEquation()
{
	CVector3D Z = itsLocation.GetDirection();
	Equation[0] = Z.GetX();
	Equation[1] = Z.GetY();
	Equation[2] = Z.GetZ();
	Equation[3] = -(itsLocation.GetOrigin().Dot(Z));
}

CPlane CPlane::XOY()
{
	CPlane P = CPlane(COneAxis(CPoint3D(0,0,0), CVector3D(0,0,1)));
	return P;
}

CPlane CPlane::YOZ()
{
	CPlane P = CPlane(COneAxis(CPoint3D(0,0,0), CVector3D(1,0,0)));
	return P;
}

CPlane CPlane::ZOX()
{
	CPlane P = CPlane(COneAxis(CPoint3D(0,0,0), CVector3D(0,1,0)));
	return P;
}
