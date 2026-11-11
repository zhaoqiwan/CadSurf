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
 *            Circle3D.h
 *
 *  Thu Jun 29 22:10:19 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _CIRCLE3D_H
#define _CIRCLE3D_H

#include "VKGeom.h"

#include "Conic.h"
#include "AxisSystem.h"
#include "Matrix33.h"
#include "MMath.h"

class CPoint3D;
class CAxisSystem;

class VKGEOM_API CCircle3D : public CConic  
{
    public:
	CCircle3D();
	CCircle3D(const CPoint3D&, const double);
	CCircle3D(const CAxisSystem& ax, const double rad);
	CCircle3D(const CPoint3D&, const CPoint3D&, const CPoint3D&);
	CCircle3D(const CCircle3D& C);
	virtual ~CCircle3D();
	virtual void Reverse();
	CCircle3D Reversed() const;
	virtual CPoint3D PointAtPara(const double& para);
	inline virtual double FirstParameter() const { return 0; }
	inline virtual double LastParameter() const { return 2*PI; }
	inline virtual double Eccentricity() const { return 0.0; }
	virtual bool IsClosed() const;
	bool operator == (const CCircle3D& L) const;
	CCircle3D operator = (const CCircle3D& C);
	inline void SetRadius(const double& r) { itsRadius = r; }
	inline void SetLocation(const CAxisSystem& ax) { itsLocation = ax; }
	inline CAxisSystem GetLocation() const { return itsLocation; }
	inline double GetRadius() const { return itsRadius; }
	inline CPoint3D Center() const { return itsLocation.GetPosition(); }
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
    protected:
	double itsRadius;
};

#endif /* _CIRCLE3D_H */
