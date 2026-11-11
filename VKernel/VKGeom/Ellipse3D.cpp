// Ellipse3D.cpp: implementation of the CEllipse3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Ellipse3D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEllipse3D::CEllipse3D()
{
	cType = ELLIPSE;
}

CEllipse3D::CEllipse3D(const CPoint3D& cp, const double min, const double maj) : minorRadius(min)
, majorRadius(maj)
{
	CAxisSystem ax(cp, CVector3D(0,0,1), CVector3D(1,0,0));
	itsLocation = ax;
	cType = ELLIPSE;
}

CEllipse3D::CEllipse3D(const CAxisSystem& Ax, double min, double maj) : minorRadius(min), majorRadius(maj)
{
	itsLocation = Ax;
	cType = ELLIPSE;
}

CEllipse3D::~CEllipse3D()
{
}

void CEllipse3D::Reverse()
{
	cReversed = !cReversed;
}

CEllipse3D CEllipse3D::Reversed() const
{
	CEllipse3D E = (*this);
	E.Reverse();
	return E;
}

double CEllipse3D::Eccentricity() const
{
	if(majorRadius == 0.0)
		return 0.0;
	else
		return sqrt(majorRadius * majorRadius - 
		minorRadius * minorRadius) / majorRadius;
}

CPoint3D CEllipse3D::PointAtPara(const double& Par)
{
	CVector3D V;
	double para = Par;

	if(cReversed)
		para = 2*PI - para;

	double c = cos(para);
	double s = sin(para);
	V = itsLocation.GetOrigin() + itsLocation.GetXDirection()*majorRadius*c
		+ itsLocation.GetYDirection()*minorRadius*s;

	return V.Point();
}

bool CEllipse3D::IsClosed() const
{
	return true;
}

bool CEllipse3D::operator == (const CEllipse3D& E) const
{
	bool eq = (itsLocation == E.GetLocation() && majorRadius == E.GetMajorRadius()
				&& minorRadius == E.GetMinorRadius());
	return eq;
}

void CEllipse3D::operator = (const CEllipse3D& E)
{
	itsLocation = E.GetLocation();
	majorRadius = E.GetMajorRadius();
	minorRadius = E.GetMinorRadius();
}

CCurve* CEllipse3D::Copy() const
{
	CEllipse3D* E = new CEllipse3D(itsLocation, majorRadius, minorRadius);
	return E;
}

void CEllipse3D::Translate(const COneAxis& Ax, const double& amt)
{
	CGeometry::Translate(Ax, amt);
}

void CEllipse3D::Translate(double dx, double dy, double dz)
{
	itsLocation.Translate(dx,dy,dz);
}

void CEllipse3D::Translate(const CVector3D& V)
{
	itsLocation.Translate(V);
}

void CEllipse3D::Translate(const CPoint3D& P1, const CPoint3D& P2)
{
	itsLocation.Translate(P1, P2);
}

void CEllipse3D::Rotate(const COneAxis& Ax, double fact)
{
	itsLocation.Rotate(Ax, fact);
}

void CEllipse3D::Scale(const CPoint3D& P, double fact)
{
	itsLocation.Scale(P, fact);
	majorRadius *= fact;
	minorRadius *= fact;
	if(majorRadius < 0)
		majorRadius = -majorRadius;
	if(minorRadius < 0)
		minorRadius = -minorRadius;
}

void CEllipse3D::Mirror(const CPoint3D& P)
{
	itsLocation.Mirror(P);
}

void CEllipse3D::Mirror(const COneAxis& Ax)
{
	itsLocation.Mirror(Ax);
}

void CEllipse3D::Mirror(const CPlane& Pln)
{
	itsLocation.Mirror(Pln);
}
