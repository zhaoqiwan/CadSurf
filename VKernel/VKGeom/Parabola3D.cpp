// Parabola3D.cpp: implementation of the CParabola3D class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "Parabola3D.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParabola3D::CParabola3D()

{
	cType = PARABOLA;
}

CParabola3D::CParabola3D(const CAxisSystem& Ax, const double fLen)
{

	itsLocation = Ax;
	focalLength = fLen;
	cType = PARABOLA;
	rangeSet = false;
}

CParabola3D::~CParabola3D()
{

}

void CParabola3D::Reverse()
{
	cReversed = !cReversed;
}

CParabola3D CParabola3D::Reversed() const
{
	CParabola3D P = (*this);
	P.Reverse();
	return P;
}

double CParabola3D::Eccentricity() const
{
	return 1;
}

CPoint3D CParabola3D::PointAtPara(const double& Par)
{

	CVector3D V;
	double par = Par;

	if(cReversed)
		par = -par;

	V = itsLocation.GetOrigin() + itsLocation.GetXDirection()*par*par/(4.*focalLength) + itsLocation.GetYDirection()*par;
	return V.Point();
}

double CParabola3D::FirstParameter() const
{
	if(!rangeSet)
	{
		return -INFINITY;
	}
	else
		return lowerRange/2*focalLength;
}

double CParabola3D::LastParameter() const
{
	if(!rangeSet)
	{
		return INFINITY;
	}
	else
		return upperRange/2*focalLength;
}

bool CParabola3D::IsClosed() const
{
	return false;
}

bool CParabola3D::operator == (const CParabola3D& L) const
{
	bool b = (itsLocation == L.GetLocation() && focalLength == L.GetFocalLength() 
		&& lowerRange == L.GetLowerRange() && upperRange == L.GetUpperRange());
	return b;
}

void CParabola3D::operator = (const CParabola3D& L)
{
	itsLocation = L.GetLocation();
	focalLength = L.GetFocalLength();
	lowerRange = L.GetLowerRange();
	upperRange = L.GetUpperRange();
}

void CParabola3D::SetLocation(const CAxisSystem& Ax)
{
	itsLocation = Ax;
}

CAxisSystem CParabola3D::GetLocation() const
{
	return itsLocation;
}

void CParabola3D::SetFocalLength(const double& fLen)
{
	focalLength = fLen;
}

double CParabola3D::GetFocalLength() const
{
	return focalLength;
}

void CParabola3D::SetYRange(const double& lower, const double& upper)
{
	lowerRange = lower;
	upperRange = upper;
	rangeSet = true;
}

double CParabola3D::GetLowerRange() const
{
	return lowerRange;
}

double CParabola3D::GetUpperRange() const
{
	return upperRange;
}

CCurve* CParabola3D::Copy() const
{
	CParabola3D* P = new CParabola3D(itsLocation, focalLength);
	P->SetYRange(lowerRange, upperRange);
	return P;
}

void CParabola3D::Translate(const COneAxis& Ax, const double& amt)
{
	CGeometry::Translate(Ax, amt);
}

void CParabola3D::Translate(double dx, double dy, double dz)
{
	itsLocation.Translate(dx,dy,dz);
}

void CParabola3D::Translate(const CVector3D& V)
{
	itsLocation.Translate(V);
}

void CParabola3D::Translate(const CPoint3D& P1, const CPoint3D& P2)
{
	itsLocation.Translate(P1, P2);
}

void CParabola3D::Rotate(const COneAxis& Ax, double fact)
{
	itsLocation.Rotate(Ax, fact);
}

void CParabola3D::Scale(const CPoint3D& P, double fact)
{
	itsLocation.Scale(P, fact);
	focalLength *= fact;
	lowerRange *= fact;
	upperRange *= fact;
	if(focalLength < 0)
		focalLength = -focalLength;
}

void CParabola3D::Mirror(const CPoint3D& P)
{
	itsLocation.Mirror(P);
}

void CParabola3D::Mirror(const COneAxis& Ax)
{
	itsLocation.Mirror(Ax);
}

void CParabola3D::Mirror(const CPlane& Pln)
{
	itsLocation.Mirror(Pln);
}
