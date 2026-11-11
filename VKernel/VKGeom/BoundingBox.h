/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
/***************************************************************************
 *            BoundingBox.h
 *
 *  Thu Jun 29 22:07:12 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _BOUNDINGBOX_H
#define _BOUNDINGBOX_H

#include "VKGeom.h"

class CPoint3D;

class VKGEOM_API CBoundingBox  
{
public:
	CBoundingBox();
	CBoundingBox(const double&, const double&, const double&, const double&, const double&, const double&);
	virtual ~CBoundingBox();
	void SetLimits(const double&, const double&, const double&, const double&, const double&, const double&);
	void GetLimits(double& xMin, double& xMax, double& yMin, double& yMax, double& zMin, double& zMax);
	inline double XMax() const { return itsXMax; }
	inline double XMin() const { return itsXMin; }
	inline double YMax() const { return itsYMax; }
	inline double YMin() const { return itsYMin; }
	inline double ZMax() const { return itsZMax; }
	inline double ZMin() const { return itsZMin; }
	CPoint3D Center() const;
	double BoundingRadius() const;
	bool Contains(const CPoint3D& P) const;
	void AddBox(const CBoundingBox&);
private:
	double itsXMax;
	double itsXMin;
	double itsYMax;
	double itsYMin;
	double itsZMax;
	double itsZMin;
};

#endif /* _BOUNDINGBOX_H */
