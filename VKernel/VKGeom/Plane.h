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
 *            Plane.h
 *
 *  Thu Jun 29 22:34:03 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/

#ifndef _PLANE_H
#define _PLANE_H

#include "VKGeom.h"

#include "Geometry.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "OneAxis.h"
#include "AxisSystem.h"

class CPoint3D;
class CVector3D;
class COneAxis;
class CAxisSystem;

class VKGEOM_API CPlane : public CGeometry
{
public:
	CPlane(){}
	CPlane(const CPoint3D&, const CPoint3D&, const CPoint3D&);
	CPlane(const CPoint3D&, const CVector3D&, const CVector3D&);
	CPlane(const double&, const double&, const double&, const double&);
	CPlane(const COneAxis&);
	virtual ~CPlane();
	virtual CPoint3D PointAtPara(const double uPar, const double vPar);
	CPoint3D GetPosition() const;
	CVector3D GetDirection() const;
	void EquationAt(const CPoint3D&, double*);
	double GetConstant() const;
	virtual void Translate(const COneAxis&, const double&);
	virtual void Translate(double dx, double dy, double dz);
	virtual void Translate(const CVector3D&);
	virtual void Translate(const CPoint3D&, const CPoint3D&);
	virtual void Rotate(const COneAxis&, double);
	virtual void Scale(const CPoint3D&, double);
	virtual void Mirror(const CPoint3D&);
	virtual void Mirror(const COneAxis&);
	virtual void Mirror(const CPlane&);
	static CPlane XOY();
	static CPlane YOZ();
	static CPlane ZOX();
private:
	void UpdateEquation();
private:
	double Equation[4];	//Plane Equation
	CAxisSystem itsLocation;

};

#endif /* _PLANE_H */
