// OffsetCurve.cpp: implementation of the COffsetCurve class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "OffsetCurve.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COffsetCurve::COffsetCurve(const CCurve& aCurve, const double& offset, const CPlane& refPlane, const bool& sense)
{
	itsCurve = aCurve.Copy();
	itsOffset = offset;
	itsPlane = refPlane;
	itsSense = sense;
}

COffsetCurve::~COffsetCurve()
{
	if(itsCurve)
		delete itsCurve;
}

void COffsetCurve::Reverse()
{
	itsCurve->Reverse();
}

CPoint3D COffsetCurve::PointAtPara(const double& para)
{
	CVector3D dir = itsPlane.GetDirection();
	CVector3D tangent(itsCurve->PointAtPara(para), itsCurve->PointAtPara(para+0.001));
	CVector3D normal = dir^tangent;
	normal.Normalize();
	if(!itsSense)
		normal = normal * -1.0;
	CPoint3D P = itsCurve->PointAtPara(para);
	P = P + normal * itsOffset;
	return P;
}

double COffsetCurve::FirstParameter() const
{
	return itsCurve->FirstParameter();
}

double COffsetCurve::LastParameter() const
{
	return itsCurve->LastParameter();
}

bool COffsetCurve::IsClosed() const
{
	return itsCurve->IsClosed();
}

bool COffsetCurve::IsOfType(const CurveType& type) const
{
	bool b = (type == OFFSETCURVE);
	return b;

}

CCurve* COffsetCurve::Copy() const
{
	COffsetCurve* myCurve = new COffsetCurve(*itsCurve, itsOffset, itsPlane, itsSense);
	return myCurve;
}

void COffsetCurve::Translate(double dx, double dy, double dz)
{
    itsCurve->Translate(dx, dy, dz);
}

void COffsetCurve::Translate(const CVector3D& V)
{
    itsCurve->Translate(V);
}

void COffsetCurve::Translate(const CPoint3D& fro, const CPoint3D& to)
{
    itsCurve->Translate(fro, to);
}

void COffsetCurve::Rotate(const COneAxis& axis, double angle)
{
    itsCurve->Rotate(axis, angle);
}

void COffsetCurve::Scale(const CPoint3D& point, double fact)
{
    itsCurve->Scale(point, fact);
}

void COffsetCurve::Mirror(const CPoint3D& point)
{
    itsCurve->Mirror(point );
}

void COffsetCurve::Mirror(const COneAxis& axis)
{
    itsCurve->Mirror(axis);
}

void COffsetCurve::Mirror(const CPlane& plane)
{
    itsCurve->Mirror(plane);
}

