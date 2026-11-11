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
 *            OneAxis.h
 *
 *  Thu Jun 29 22:30:18 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _ONEAXIS_H
#define _ONEAXIS_H

#include "VKGeom.h"

#include "Geometry.h"
#include "Point3D.h"
#include "Vector3D.h"

class CPoint3D;
class CVector3D;

class VKGEOM_API COneAxis : public CGeometry  
{
public:
	COneAxis();
	COneAxis(const CPoint3D& Pos, const CVector3D& Dir);
	~COneAxis();
	void SetPosition(const CPoint3D& P);
	void SetPosition(double x, double y, double z);
	void SetDirection(const CVector3D& D);
	void SetDirection(double dx, double dy, double dz);
	CPoint3D GetPosition() const;
	CVector3D GetOrigin() const;
	CVector3D GetDirection() const;
	bool operator == (const COneAxis&) const;
	virtual void Translate(const COneAxis&, const double&);
	virtual void Translate(double dx, double dy, double dz);
	virtual void Translate(const CVector3D&);
	virtual void Translate(const CPoint3D&, const CPoint3D&);
	virtual void Rotate(const COneAxis&, double);
	virtual void Scale(const CPoint3D&, double);
	virtual void Mirror(const CPoint3D&);
	virtual void Mirror(const COneAxis&);
	virtual void Mirror(const CPlane&);
	static COneAxis OX();
	static COneAxis OY();
	static COneAxis OZ();
private:
	CPoint3D Position;
	CVector3D Origin;
	CVector3D Direction;
};

#endif /* _ONEAXIS_H */
