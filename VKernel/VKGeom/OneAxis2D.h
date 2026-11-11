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
 *            OneAxis2D.h
 *
 *  Thu Jun 29 22:31:25 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _ONEAXIS2D_H
#define _ONEAXIS2D_H

#include "VKGeom.h"

#include "Geometry2D.h"
#include "Point2D.h"
#include "Vector2D.h"

class CPoint2D;
class CVector2D;

class VKGEOM_API COneAxis2D : public CGeometry2D
{
public:
	COneAxis2D();
	COneAxis2D(const CPoint2D& Pos, const CVector2D& Dir);
	~COneAxis2D();
	void SetPosition(const CPoint2D& P);
	void SetPosition(double x, double y);
	void SetDirection(const CVector2D& D);
	void SetDirection(double dx, double dy);
	CPoint2D GetPosition() const;
	CVector2D GetOrigin() const;
	CVector2D GetDirection() const;
	bool operator == (const COneAxis2D&) const;
	virtual void Translate(const COneAxis2D&, const double&);
	virtual void Translate(double dx, double dy);
	virtual void Translate(const CVector2D&);
	virtual void Translate(const CPoint2D&, const CPoint2D&);
	virtual void Rotate(const CPoint2D&, double);
	virtual void Scale(const CPoint2D&, double);
	virtual void Mirror(const CPoint2D&);
	virtual void Mirror(const COneAxis2D&);
	static COneAxis2D OX();
	static COneAxis2D OY();
private:
	CPoint2D Position;
	CVector2D Origin;
	CVector2D Direction;
};

#endif /* _ONEAXIS2D_H */
