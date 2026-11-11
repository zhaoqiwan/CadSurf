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
 *            Geometry.h
 *
 *  Thu Jun 29 22:18:35 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _GEOMETRY_H
#define _GEOMETRY_H

#include "VKGeom.h"

enum GeometryType { CPOINT, CPLANE, CPOLYGON, CONEAXIS, CAXISSYSTEM, CCURVE, CSURFACE };

class CPoint3D;
class CVector3D;
class COneAxis;
class CAxisSystem;
class CLine3D;
class CCircle3D;
class CEllipse3D;
class CHyperbola3D;
class CParabola3D;
class CBezierCurve;
class CBSplineCurve;
class CPlane;
class CCone;
class CCylinder;
class CSphere;
class CTorus;
class CExtrudedSurface;
class CRevolvedSurface;
class CRuledSurface;
class CPipeSurface;

class VKGEOM_API CGeometry
{

public:
	CGeometry();
	virtual ~CGeometry();
	virtual void Translate(const COneAxis&, const double& amt);
	virtual void Translate(double dx, double dy, double dz) = 0;
	virtual void Translate(const CVector3D&) = 0;
	virtual void Translate(const CPoint3D&, const CPoint3D&) = 0;
	virtual void Rotate(const COneAxis&, double) = 0;
	virtual void Scale(const CPoint3D&, double) = 0;
	virtual void Mirror(const CPoint3D&) = 0;
	virtual void Mirror(const COneAxis&) = 0;
	virtual void Mirror(const CPlane&) = 0;
	inline GeometryType GeomType() const { return geomType; }
private:
	static int refCount;
protected:
	GeometryType geomType;

};

#endif /* _GEOMETRY_H */
