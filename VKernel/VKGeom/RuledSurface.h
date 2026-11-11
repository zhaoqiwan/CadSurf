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
 *            RuledSurface.h
 *
 *  Thu Jun 29 22:38:04 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _RULEDSURFACE_H
#define _RULEDSURFACE_H

#include "VKGeom.h"

#include "SweptSurface.h"
#include "Curve.h"

class VKGEOM_API CRuledSurface : public CSweptSurface  
{
public:
	CRuledSurface(){}
	CRuledSurface(const CCurve*, const CCurve*);
	virtual ~CRuledSurface();
	CCurve* RailCurve1() const;
	CCurve* RailCurve2() const;
	virtual CPoint3D PointAtPara(const double uPar, const double vPar);
	virtual CVector3D NormalAt(const double uPar, const double vPar);
	virtual double FirstUParameter() const;
	virtual double LastUParameter() const;
	virtual double FirstVParameter() const;
	virtual double LastVParameter() const;
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
	CCurve* secondRail;
};

#endif /* _RULEDSURFACE_H */
