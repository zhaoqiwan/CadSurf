// Curve.cpp: implementation of the CCurve class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Curve2D.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCurve2D::CCurve2D()
{
	cReversed = false;
	geomType = CCURVE2D;
}

CCurve2D::~CCurve2D()
{

}

Curve2DType CCurve2D::Type() const
{
	return cType;
}

bool CCurve2D::IsOfType(const Curve2DType& type) const
{
	Curve2DType typ = type;
	bool b = (cType == typ);
	return b;
}

