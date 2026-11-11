// Curve.cpp: implementation of the CCurve class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Curve.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCurve::CCurve()
{
	cReversed = false;
	geomType = CCURVE;
}

CCurve::~CCurve()
{

}

CurveType CCurve::Type() const
{
	return cType;
}

bool CCurve::IsOfType(const CurveType& type) const
{
	CurveType typ = type;
	bool b = (cType == typ);
	return b;
}

