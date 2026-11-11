// Hyperbola3D.cpp: implementation of the CHyperbola3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Hyperbola3D.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHyperbola3D::CHyperbola3D()
{
	cType = HYPERBOLA;
}

CHyperbola3D::CHyperbola3D(const CAxisSystem& Ax, const double& maj, const double& min) : majorRadius(maj), minorRadius(min)
{
	itsLocation = Ax;
	cType = HYPERBOLA;
}

CHyperbola3D::~CHyperbola3D()
{

}

void CHyperbola3D::Reverse()
{
	cReversed = !cReversed;
}

CHyperbola3D CHyperbola3D::Reversed() const
{
	CHyperbola3D H = (*this);
	H.Reverse();
	return H;
}

double CHyperbola3D::Eccentricity() const
{
	return (sqrt(majorRadius*majorRadius + minorRadius*minorRadius))/majorRadius;
}

CPoint3D CHyperbola3D::PointAtPara(const double& Par)
{
	CVector3D V;
	double par = Par;

	if(cReversed)
		par = -par;

	double ch = cosh(par);
	double sh = sinh(par);
	V = itsLocation.GetOrigin() + itsLocation.GetXDirection()*majorRadius*ch + itsLocation.GetYDirection()*minorRadius*sh;
	return V.Point();
}

double CHyperbola3D::FirstParameter() const
{
	return -2*PI;
}

double CHyperbola3D::LastParameter() const
{
	return 2*PI;
}

bool CHyperbola3D::IsClosed() const
{
	return false;
}

bool CHyperbola3D::operator == (const CHyperbola3D& L) const
{
	bool b = (itsLocation == L.GetLocation() &&	majorRadius == L.GetMajorRadius()
			&& minorRadius == L.GetMinorRadius());
	return b;
}

void CHyperbola3D::operator = (const CHyperbola3D& L)
{
	itsLocation = L.GetLocation();
	majorRadius = L.GetMajorRadius();
	minorRadius = L.GetMinorRadius();
}

void CHyperbola3D::SetMinorRadius(const double& rad)
{
	minorRadius = rad;
}

void CHyperbola3D::SetMajorRadius(const double& rad)
{
	majorRadius = rad;
}

double CHyperbola3D::GetMinorRadius() const
{
	return minorRadius;
}

double CHyperbola3D::GetMajorRadius() const
{
	return majorRadius;
}

CHyperbola3D CHyperbola3D::GetConjugateBranch1() const
{
	CVector3D Dir = itsLocation.GetYDirection();
	CAxisSystem Ax(itsLocation.GetPosition(), itsLocation.GetDirection(), Dir);
	CHyperbola3D H(Ax, minorRadius, majorRadius);
	return H;
}

CHyperbola3D CHyperbola3D::GetConjugateBranch2() const
{
	CVector3D Dir = itsLocation.GetYDirection();
	Dir.Reverse();
	CAxisSystem Ax(itsLocation.GetPosition(), itsLocation.GetDirection(), Dir);
	CHyperbola3D H(Ax, minorRadius, majorRadius);
	return H;
}

CHyperbola3D CHyperbola3D::GetOtherBranch() const
{
	CVector3D Dir = itsLocation.GetXDirection();
	Dir.Reverse();
	CAxisSystem Ax(itsLocation.GetPosition(), itsLocation.GetDirection(), Dir);
	CHyperbola3D H(Ax, majorRadius, minorRadius);
	return H;
}

void CHyperbola3D::SetLocation(const CAxisSystem& ax)
{
	itsLocation = ax;
}

CAxisSystem CHyperbola3D::GetLocation() const
{
	return itsLocation;
}

double CHyperbola3D::FocalLength() const
{
	return 2.0 * sqrt(majorRadius * majorRadius + minorRadius * minorRadius);
}

CCurve* CHyperbola3D::Copy() const
{
	CHyperbola3D* H = new CHyperbola3D(itsLocation, majorRadius, minorRadius);
	return H;
}

void CHyperbola3D::Translate(const COneAxis& Ax, const double& amt)
{
	CGeometry::Translate(Ax, amt);
}

void CHyperbola3D::Translate(double dx, double dy, double dz)
{
	itsLocation.Translate(dx,dy,dz);
}

void CHyperbola3D::Translate(const CVector3D& V)
{
	itsLocation.Translate(V);
}

void CHyperbola3D::Translate(const CPoint3D& P1, const CPoint3D& P2)
{
	itsLocation.Translate(P1, P2);
}

void CHyperbola3D::Rotate(const COneAxis& Ax, double fact)
{
	itsLocation.Rotate(Ax, fact);
}

void CHyperbola3D::Scale(const CPoint3D& P, double fact)
{
	itsLocation.Scale(P, fact);
	majorRadius *= fact;
	minorRadius *= fact;
	if(majorRadius < 0)
		majorRadius = -majorRadius;
	if(minorRadius < 0)
		minorRadius = -minorRadius;
}

void CHyperbola3D::Mirror(const CPoint3D& P)
{
	itsLocation.Mirror(P);
}

void CHyperbola3D::Mirror(const COneAxis& Ax)
{
	itsLocation.Mirror(Ax);
}

void CHyperbola3D::Mirror(const CPlane& Pln)
{
	itsLocation.Mirror(Pln);
}
