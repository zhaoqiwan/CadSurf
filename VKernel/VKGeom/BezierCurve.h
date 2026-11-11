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
 *            BezierCurve.h
 *
 *  Thu Jun 29 22:06:01 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _BEZIERCURVE_H
#define _BEZIERCURVE_H

#include "VKGeom.h"

#include "PCurve.h"
#include "ListOfCPoint3D.h"
#include "ListIteratorOfListOfCPoint3D.h"

class VKGEOM_API CBezierCurve : public CPCurve  
{
public:
	CBezierCurve();
	CBezierCurve(CListOfCPoint3D*);
	virtual ~CBezierCurve();
	virtual void Reverse();
	virtual CPoint3D PointAtPara(const double& para);
	virtual double FirstParameter() const;
	virtual double LastParameter() const;
	virtual bool IsClosed() const;
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
	double BlendingFunc(int n,int r,double t);
private:
	CListOfCPoint3D *cpList;
	int cpCount;
};

#endif /* _BEZIERCURVE_H */
