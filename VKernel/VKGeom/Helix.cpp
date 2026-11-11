// Helix.cpp: implementation of the CHelix class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Helix.h"

#include "Point3D.h"
#include "Vector3D.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHelix::CHelix(const CAxisSystem& Ax, const double& radius, const double& start, const double& end, const double lead) : itsLead(lead)
{
	itsLocation = Ax;
	itsRadius = radius;
	itsFirstParameter = start;
	itsLastParameter = end;
}

CHelix::~CHelix()
{

}

void CHelix::Reverse()
{
}

CPoint3D CHelix::PointAtPara(const double& Par)
{
	CPoint3D P; CVector3D V;
	double para = Par;

	double c = cos(para);
	double s = sin(para);
	double v = ((para-itsFirstParameter)/(PI*2))*itsLead;
	V = itsLocation.GetOrigin()+(itsLocation.GetXDirection()*c+itsLocation.GetYDirection()*s)
			*GetRadius()+itsLocation.GetDirection()*v;
	P.SetParam(V.GetX(), V.GetY(), V.GetZ());

	return P;
}

double CHelix::FirstParameter() const
{
	return itsFirstParameter;
}

double CHelix::LastParameter() const
{
	return itsLastParameter;
}

bool CHelix::IsClosed() const
{
	return false;
}

CurveType CHelix::Type() const
{
	return HELIX;
}

bool CHelix::IsOfType(const CurveType& theType) const
{
	bool b = (theType == HELIX);
	return b;
}

CCurve* CHelix::Copy() const
{
	CHelix* H = new CHelix(itsLocation, itsRadius, itsFirstParameter, itsLastParameter, itsLead);
	return H;
}

void CHelix::Translate(double dx, double dy, double dz)
{
}

void CHelix::Translate(const CVector3D&)
{
}

void CHelix::Translate(const CPoint3D&, const CPoint3D&)
{
}

void CHelix::Rotate(const COneAxis&, double)
{
}

void CHelix::Scale(const CPoint3D&, double)
{
}

void CHelix::Mirror(const CPoint3D&)
{
}

void CHelix::Mirror(const COneAxis&)
{
}

void CHelix::Mirror(const CPlane&)
{
}
