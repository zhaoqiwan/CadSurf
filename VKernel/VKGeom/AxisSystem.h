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
 *            AxisSystem.h
 *
 *  Thu Jun 29 22:02:15 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _AXISSYSTEM_H
#define _AXISSYSTEM_H

#include "VKGeom.h"

#include "Geometry.h"
#include "Point3D.h"
#include "Vector3D.h"

class CPoint3D;
class CVector3D;

class VKGEOM_API CAxisSystem : public CGeometry  
{
public:
	CAxisSystem();
	CAxisSystem(const CPoint3D& Pos, const CVector3D& Dir, const CVector3D& XDir);
	CAxisSystem(const CPoint3D& Pos, const CVector3D& Dir);
	CAxisSystem(const CAxisSystem&);
	~CAxisSystem();
	void SetPosition(const CPoint3D& P);
	void SetPosition(double x, double y, double z);
	void SetDirection(const CVector3D& D);
	void SetDirection(double dx, double dy, double dz);
	void SetXDirection(const CVector3D& XD);
	void SetXDirection(double dxx, double dxy, double dxz);
	void SetYDirection(const CVector3D& YD);
	void SetYDirection(double dyx, double dyy, double dyz);
	inline CPoint3D GetPosition() const { return Position; }
	inline CVector3D GetOrigin() const	{ return Origin; }
	inline CVector3D GetDirection() const	{ return Direction; }
	inline CVector3D GetXDirection() const { return XDirection; }
	inline CVector3D GetYDirection() const { return YDirection; }
	bool operator == (const CAxisSystem& C) const;
	virtual void Translate(const COneAxis&, const double&);
	virtual void Translate(double dx, double dy, double dz);
	virtual void Translate(const CVector3D&);
	virtual void Translate(const CPoint3D& P1, const CPoint3D& P2);
	virtual void Rotate(const COneAxis&, double);
	virtual void Scale(const CPoint3D&, double);
	virtual void Mirror(const CPoint3D&);
	virtual void Mirror(const COneAxis&);
	virtual void Mirror(const CPlane&);
private:
	CPoint3D Position;
	CVector3D Origin;
	CVector3D Direction;
	CVector3D XDirection;
	CVector3D YDirection;
};

#endif /* _AXISSYSTEM_H */
