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
 *            Cylinder.h
 *
 *  Thu Jun 29 22:15:15 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _CYLINDER_H
#define _CYLINDER_H

#include "VKGeom.h"

#include "BasicSurface.h"
#include "MMath.h"

class CCircle3D;
class CAxisSystem;

class VKGEOM_API CCylinder : public CBasicSurface  
{
    public:
	CCylinder(){}
	CCylinder(const CCircle3D&, const double& ht);
	CCylinder(const CAxisSystem&, const double& ht, const double& rad, const double& uStart = 0, const double& uEnd = 2*PI);
	virtual ~CCylinder();
	inline void SetRadius(const double& r) { itsRadius = r; }
	inline void SetHeight(const double& h) { itsHeight = h; }
	inline double GetRadius() const { return itsRadius; }
	inline double GetHeight() const { return itsHeight; }
	virtual CPoint3D PointAtPara(const double uPar, const double vPar);
	inline virtual double FirstUParameter() const { return itsFirstUParameter; }
	inline virtual double LastUParameter() const  { return itsLastUParameter; }
	inline virtual double FirstVParameter() const { return itsFirstVParameter; }
	inline virtual double LastVParameter() const { return itsLastVParameter; }
	virtual bool IsUClosed() const;
	virtual bool IsVClosed() const;
	virtual CSurface* Copy() const;
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
	double itsHeight;	
	double itsRadius;
};

#endif /* _CYLINDER_H */
