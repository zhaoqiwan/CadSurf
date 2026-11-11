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
 *            Point2D.h
 *
 *  Thu Jun 29 22:35:47 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _POINT2D_H
#define _POINT2D_H


#include <iostream>
using namespace std;

#include "VKGeom.h"

#include "Geometry2D.h"


class CVector2D;
class COneAxis2D;

enum ORIENTATION 
{ 
	ORIENTED_CCW, 
	ORIENTED_CW, 
	ORIENTED_COLLINEAR 
};

enum CLASSIFICATION
{
	CLASSIFIED_LEFT,
	CLASSIFIED_RIGHT,
	CLASSIFIED_BEHIND,
	CLASSIFIED_BEYOND,
	CLASSIFIED_BETWEEN,
	CLASSIFIED_ORIGIN,
	CLASSIFIED_DESTINATION
};

class VKGEOM_API CPoint2D  : public CGeometry2D
{
    friend std::istream &operator>>(std::istream& , CPoint2D&);
    friend std::ostream &operator<<(std::ostream& , CPoint2D&);
    public:
    CPoint2D();
    CPoint2D(double, double);
    virtual ~CPoint2D();
    void SetParam(double, double);
    inline void SetX(double x) {itsX=x;}
    inline void SetY(double y) {itsY=y;}
    inline double GetX() const { return itsX; }
    inline double GetY() const { return itsY; }
    CPoint2D operator+(const CPoint2D&);
    CPoint2D operator+(const double&);
    CPoint2D operator-(const CPoint2D&);
    CPoint2D operator-(const double&);
    CPoint2D operator*(const double scale);
    CPoint2D operator/(const double scale);
    void operator+=(const CPoint2D&);
    void operator+=(const double&);
    void operator-=(const CPoint2D&);
    void operator-=(const double&);
    void operator*=(const double scale);
    void operator/=(const double scale);
    CPoint2D operator = (const CPoint2D& p);    
    double Distance(const CPoint2D&);
    
    double PolarAngle(const CPoint2D& P = CPoint2D::Origin()) const;
    bool operator == (const CPoint2D& p) const;
    ORIENTATION Orientation(const CPoint2D& P1, const CPoint2D& P2);
    CLASSIFICATION Classification(const CPoint2D& P1, const CPoint2D& P2);
    operator CVector2D();
    virtual void Translate(const COneAxis2D&, const double&);
    virtual void Translate(double dx, double dy);
    virtual void Translate(const CVector2D&);
    virtual void Translate(const CPoint2D&, const CPoint2D&);
    virtual void Rotate(const CPoint2D&, double);
    virtual void Scale(const CPoint2D&, double);
    virtual void Mirror(const CPoint2D&);
    virtual void Mirror(const COneAxis2D&);
    static CPoint2D Origin();
    private:
    double itsX;
    double itsY;
};

#endif /* _POINT2D_H */
