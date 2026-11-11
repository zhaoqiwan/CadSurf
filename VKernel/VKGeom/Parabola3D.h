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
 *            Parabola3D.h
 *
 *  Thu Jun 29 22:33:00 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _PARABOLA3D_H
#define _PARABOLA3D_H

#include "VKGeom.h"

#include "Conic.h"

class VKGEOM_API CParabola3D : public CConic  
{
public:
	CParabola3D();
	CParabola3D(const CAxisSystem& Ax, const double fLen);
	virtual ~CParabola3D();
	virtual void Reverse();
	CParabola3D Reversed() const;
	virtual double Eccentricity() const;
	virtual CPoint3D PointAtPara(const double& para);
	virtual double FirstParameter() const;
	virtual double LastParameter() const;
	virtual bool IsClosed() const;
	bool operator == (const CParabola3D& L) const;
	void operator = (const CParabola3D& L);
	void SetLocation(const CAxisSystem&);
	CAxisSystem GetLocation() const;
	void SetFocalLength(const double&);
	double GetFocalLength() const;
	void SetYRange(const double& lower, const double& upper);
	double GetLowerRange() const;
	double GetUpperRange() const;
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
	double focalLength;
	double lowerRange;
	double upperRange;
	bool rangeSet;
};

#endif /* _PARABOLA3D_H */
