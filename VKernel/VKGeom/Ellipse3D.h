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
 *            Ellipse3D.h
 *
 *  Thu Jun 29 22:16:02 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _ELLIPSE3D_H
#define _ELLIPSE3D_H

#include "VKGeom.h"

#include "Conic.h"
#include "MMath.h"

class VKGEOM_API CEllipse3D : public CConic  
{
public:
	CEllipse3D();
	CEllipse3D(const CPoint3D& cp, const double min, const double maj);
	CEllipse3D(const CAxisSystem& Ax, double min, double maj);
	virtual ~CEllipse3D();
	virtual void Reverse();
	CEllipse3D Reversed() const;
	virtual CPoint3D PointAtPara(const double& para);
	inline virtual double FirstParameter() const { return 0; }
	inline virtual double LastParameter() const { return 2 * PI; }
	virtual double Eccentricity() const;
	virtual bool IsClosed() const;
	bool operator == (const CEllipse3D& L) const;
	void operator = (const CEllipse3D& L);
	inline void SetMinorRadius(const double& rad) { minorRadius = rad; }
	inline void SetMajorRadius(const double& rad) { majorRadius = rad; }
	inline double GetMinorRadius() const { return minorRadius; }
	inline double GetMajorRadius() const { return majorRadius; }
	inline void SetLocation(const CAxisSystem& ax) { itsLocation = ax; }
	inline CAxisSystem GetLocation() const { return itsLocation; }
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
	double minorRadius;
	double majorRadius;
};

#endif /* _ELLIPSE3D_H */
