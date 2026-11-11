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
 *            Cone.h
 *
 *  Thu Jun 29 22:11:00 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _CONE_H
#define _CONE_H

#include "VKGeom.h"

#include "BasicSurface.h"
#include "MMath.h"


class VKGEOM_API CCone : public CBasicSurface  
{
    public:
	CCone(){}
	CCone(const CCircle3D&, const double& ht);	
	CCone(const CAxisSystem&, const double& ht, const double& rad1, const double& rad2 = 0, const double& uStart = 0, const double& uEnd = 2*PI);
	virtual ~CCone();
	inline void SetBaseRadius(const double& r) { baseRadius = r; }
	inline void SetTopRadius(const double& r) { topRadius = r; }
	inline void SetHeight(const double& h) { itsHeight = h; }
	inline double GetBaseRadius() const { return baseRadius; }
	inline double GetTopRadius() const { return topRadius; }
	inline double GetHeight() const { return itsHeight; }
	virtual CPoint3D PointAtPara(const double uPar, const double vPar);
	inline virtual double FirstUParameter() const { return itsFirstUParameter; }
	inline virtual double LastUParameter() const { return itsLastUParameter; }
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
	double baseRadius;
	double topRadius;	
};

#endif /* _CONE_H */
