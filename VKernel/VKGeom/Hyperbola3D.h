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
 *            Hyperbola3D.h
 *
 *  Thu Jun 29 22:20:12 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _HYPERBOLA3D_H
#define _HYPERBOLA3D_H

#include "VKGeom.h"

#include "Conic.h"

class VKGEOM_API CHyperbola3D : public CConic  
{
public:
	CHyperbola3D();
	CHyperbola3D(const CAxisSystem&, const double&, const double&);
	virtual ~CHyperbola3D();
	virtual void Reverse();
	CHyperbola3D Reversed() const;
	virtual double Eccentricity() const;
	virtual CPoint3D PointAtPara(const double& para);
	virtual double FirstParameter() const;
	virtual double LastParameter() const;
	virtual bool IsClosed() const;
	bool operator == (const CHyperbola3D& L) const;

	void operator = (const CHyperbola3D& L);
	void SetMinorRadius(const double&);
	void SetMajorRadius(const double&);
	double GetMinorRadius() const;
	double GetMajorRadius() const;
	CHyperbola3D GetConjugateBranch1() const;
	CHyperbola3D GetConjugateBranch2() const;
	CHyperbola3D GetOtherBranch() const;
	void SetLocation(const CAxisSystem& ax);
	CAxisSystem GetLocation() const;
	double FocalLength() const;
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
	double majorRadius;
	double minorRadius;
};

#endif /* _HYPERBOLA3D_H */
