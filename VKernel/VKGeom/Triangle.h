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
 *            Triangle.h
 *
 *  Thu Jun 29 22:43:46 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include "VKGeom.h"

#include "Geometry.h"
#include "Point3D.h"
#include "Point2D.h"
#include "Vector3D.h"

class VKGEOM_API CTriangle : public CGeometry
{
public:
    CTriangle();
    CTriangle(const CPoint3D& v1, const CPoint3D& v2, const CPoint3D& v3);
    CTriangle(const CTriangle& other);
    virtual ~CTriangle();
    void SetVertex(const int& index, const CPoint3D& v);
    CPoint3D GetVertex(const int& index) const;
    CVector3D Normal() const;    
    bool operator == (const CTriangle& p) const;
    virtual void Translate(double dx, double dy, double dz);
    virtual void Translate(const CVector3D&);
    virtual void Translate(const CPoint3D&, const CPoint3D&);
    virtual void Rotate(const COneAxis&, double);
    virtual void Scale(const CPoint3D&, double);
    virtual void Mirror(const CPoint3D&);
    virtual void Mirror(const COneAxis&);
    virtual void Mirror(const CPlane&);
private:
    CPoint3D vArray[3];
};

#endif /* _TRIANGLE_H */
