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
 *            Point3D.h
 *
 *  Thu Jun 29 22:36:31 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _POINT3D_H
#define _POINT3D_H

#include <iostream>
using namespace std;

#include "VKGeom.h"

#include "Geometry.h"

    class CVector3D;
    class COneAxis;
    class CLine3D;
    class CPlane;


class VKGEOM_API CPoint3D  : public CGeometry
    {
	friend std::istream &operator>>(std::istream& , CPoint3D&);
	friend std::ostream &operator<<(std::ostream& , CPoint3D&);
	public:
	CPoint3D();
	CPoint3D(double, double, double);
	virtual ~CPoint3D();
	void SetParam(double, double, double);
	inline void SetX(double x) {itsX=x;}
	inline void SetY(double y) {itsY=y;}
	inline void SetZ(double z) {itsZ=z;}
	inline double GetX() const { return itsX; }
	inline double GetY() const { return itsY; }
	inline double GetZ() const { return itsZ; }
	CPoint3D operator+(const CPoint3D&);
	CPoint3D operator+(const double&);
	CPoint3D operator-(const CPoint3D&);
	CPoint3D operator-(const double&);
	CPoint3D operator*(const double scale);
	CPoint3D operator/(const double scale);
	void operator+=(const CPoint3D&);
	void operator+=(const double&);
	void operator-=(const CPoint3D&);
	void operator-=(const double&);
	void operator*=(const double scale);
	void operator/=(const double scale);
	CPoint3D operator = (const CPoint3D& p);    
	double Distance(const CPoint3D&) const;
	double Distance(const CPlane&) const;
	double PolarAngle(const CPoint3D& P = CPoint3D::Origin()) const;
	bool operator == (const CPoint3D& p) const;
	operator CVector3D();
	virtual void Translate(const COneAxis&, const double&);
	virtual void Translate(double dx, double dy, double dz);
	virtual void Translate(const CVector3D&);
	virtual void Translate(const CPoint3D&, const CPoint3D&);
	virtual void Rotate(const COneAxis&, double);
	virtual void Scale(const CPoint3D&, double);
	virtual void Mirror(const CPoint3D&);
	virtual void Mirror(const COneAxis&);
	virtual void Mirror(const CPlane&);
	static CPoint3D Origin();
	private:
	double itsX;
	double itsY;
	double itsZ;
};

#endif /* _POINT3D_H */
