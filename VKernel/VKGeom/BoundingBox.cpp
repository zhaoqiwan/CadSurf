// BoundingBox.cpp: implementation of the CBoundingBox class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "BoundingBox.h"
#include "Point3D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBoundingBox::CBoundingBox()
{

}

CBoundingBox::CBoundingBox(const double& xMin, const double& xMax, 
						   const double& yMin, const double& yMax, 
						   const double& zMin, const double& zMax) : itsXMax(xMax), itsXMin(xMin),
						   itsYMax(yMax), itsYMin(yMin),
						   itsZMax(zMax), itsZMin(zMin)
{
}

CBoundingBox::~CBoundingBox()
{

}

CPoint3D CBoundingBox::Center() const
{
	CPoint3D P;
	P.SetX((XMax() + XMin())/2);
	P.SetY((YMax() + YMin())/2);
	P.SetZ((ZMax() + ZMin())/2);
	return P;
}

double CBoundingBox::BoundingRadius() const
{
	double rad;
	CPoint3D Pmax(XMax(), YMax(), ZMax());
	CPoint3D Cen = Center();
	rad = Pmax.Distance(Cen);
	return rad;
}

void CBoundingBox::SetLimits(const double& xMin, const double& xMax, 
						   const double& yMin, const double& yMax, 
						   const double& zMin, const double& zMax)
{
	itsXMax = xMax;
	itsXMin = xMin;
	itsYMax = yMax;
	itsYMin = yMin;
	itsZMax = zMax;
	itsZMin = zMin;
}

void CBoundingBox::GetLimits(double& xMin, double& xMax,
						   double& yMin, double& yMax,
						   double& zMin, double& zMax)
{
	xMax = itsXMax;
	xMin = itsXMin;
	yMax = itsYMax;
	yMin = itsYMin;
	zMax = itsZMax;
	zMin = itsZMin;
}

bool CBoundingBox::Contains(const CPoint3D& P) const
{
	bool bx = false, by = false, bz = false;
	bx = (P.GetX() <= XMax() && P.GetX() >= XMin());
	by = (P.GetY() <= YMax() && P.GetY() >= YMin());
	bz = (P.GetZ() <= ZMax() && P.GetZ() >= ZMin());

	return (bx && by &&bz);
}

void CBoundingBox::AddBox(const CBoundingBox& B)
{
	if(B.XMax() > XMax())
		itsXMax = B.XMax();
	if(B.XMin() < XMin())
		itsXMin = B.XMin();

	if(B.YMax() > YMax())
		itsYMax = B.YMax();
	if(B.YMin() < YMin())
		itsYMin = B.YMin();

	if(B.ZMax() > ZMax())
		itsZMax = B.ZMax();
	if(B.ZMin() < ZMin())
		itsZMin = B.ZMin();
}
