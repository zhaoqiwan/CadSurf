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
 *            Surface.h
 *
 *  Thu Jun 29 22:40:38 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _SURFACE_H
#define _SURFACE_H

#include "VKGeom.h"

#include "Geometry.h"
#include "Point3D.h"
#include "AxisSystem.h"

enum SurfaceType { CONE, CYLINDER, EXTRUDED, OFFSET, PIPE, PLANE, REVOLVED, RULED, SPHERE, TORUS };

class CPoint3D;
class CAxisSystem;

//Abstract Base Class For All Surfaces
class VKGEOM_API CSurface : public CGeometry  
{
public:
	CSurface();
	virtual ~CSurface();
	virtual CPoint3D PointAtPara(const double uPar, const double vPar) = 0;
	virtual CVector3D NormalAt(const double uPar, const double vPar);
	virtual double FirstUParameter() const = 0;
	virtual double LastUParameter() const = 0;
	virtual double FirstVParameter() const = 0;
	virtual double LastVParameter() const = 0;
	virtual bool IsUClosed() const = 0;
	virtual bool IsVClosed() const = 0;
	SurfaceType Type() const;
	bool IsOfType(const SurfaceType&) const;
	virtual CSurface* Copy() const = 0;
	virtual void Translate(double dx, double dy, double dz) = 0;
	virtual void Translate(const CVector3D&) = 0;
	virtual void Translate(const CPoint3D&, const CPoint3D&) = 0;
	virtual void Rotate(const COneAxis&, double) = 0;
	virtual void Scale(const CPoint3D&, double) = 0;
	virtual void Mirror(const CPoint3D&) = 0;
	virtual void Mirror(const COneAxis&) = 0;
	virtual void Mirror(const CPlane&) = 0;
protected:
	SurfaceType sType;
};

#endif /* _SURFACE_H */
