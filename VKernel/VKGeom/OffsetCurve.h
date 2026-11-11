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
 *            OffsetCurve.h
 *
 *  Thu Jun 29 22:28:50 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _OFFSETCURVE_H
#define _OFFSETCURVE_H

#include "VKGeom.h"

#include "Curve.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "Plane.h"

class VKGEOM_API COffsetCurve : public CCurve  
{
public:
	COffsetCurve(const CCurve& aCurve, const double& offset, const CPlane& refPlane, const bool& sense = true);
	virtual ~COffsetCurve();
	virtual void Reverse();
	virtual CPoint3D PointAtPara(const double& para);
	virtual double FirstParameter() const;
	virtual double LastParameter() const;
	virtual bool IsClosed() const;
	CurveType Type() const;
	bool IsOfType(const CurveType&) const;
	virtual CCurve* Copy() const;
	virtual void Translate(double dx, double dy, double dz);
	virtual void Translate(const CVector3D&);
	virtual void Translate(const CPoint3D&, const CPoint3D&);
	virtual void Rotate(const COneAxis&, double);
	virtual void Scale(const CPoint3D&, double);
	virtual void Mirror(const CPoint3D&);
	virtual void Mirror(const COneAxis&);
	virtual void Mirror(const CPlane&);
private:
	CCurve* itsCurve;
	double itsOffset;
	CPlane itsPlane;
	bool itsSense;
};

#endif /* _OFFSETCURVE_H */
