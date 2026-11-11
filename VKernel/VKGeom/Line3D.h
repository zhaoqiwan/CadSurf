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
 *            Line3D.h
 *
 *  Thu Jun 29 22:23:15 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _LINE3D_H
#define _LINE3D_H

#include "VKGeom.h"

#include "Curve.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "OneAxis.h"

class CPoint3D;
class CVector3D;
class COneAxis;

class VKGEOM_API CLine3D : public CCurve  
{
public:
	CLine3D();
	CLine3D(const CPoint3D&, const CPoint3D&);
	CLine3D(const CPoint3D&, const CVector3D&);
	virtual ~CLine3D();
	virtual void Reverse();
	virtual CPoint3D PointAtPara(const double& par);
	inline virtual double FirstParameter() const { return 0; }
	inline virtual double LastParameter() const { 	return 1; }
	virtual bool IsClosed() const;
	bool operator == (const CLine3D& L) const;
	void operator = (const CLine3D& L);
	CVector3D Direction() const;
	inline COneAxis GetLocation() const { return itsLocation; }
	CVector3D NormalThrough(const CPoint3D&);
	double Distance(const CPoint3D&) const;
	inline double SegmentLength() const { return startPoint.Distance(endPoint); }
	inline CPoint3D StartPoint() const { return startPoint; }
	inline CPoint3D EndPoint() const { return endPoint; }
	virtual CCurve* Copy() const;
	virtual void Translate(const COneAxis&, const double&);
	virtual void Translate(double dx, double dy, double dz);
	virtual void Translate(const CVector3D&);
	virtual void Translate(const CPoint3D&, const CPoint3D&);
	virtual void Rotate(const COneAxis&, double);
	virtual void Scale(const CPoint3D&, double);
	virtual void Mirror(const CPoint3D&);
	virtual void Mirror(const COneAxis&);
	virtual void Mirror(const CPlane&);
private:
	CPoint3D startPoint;
	CPoint3D endPoint;
	COneAxis itsLocation;
};

#endif /* _LINE3D_H */
