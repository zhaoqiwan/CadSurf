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
 *            GLPoint.h
 *
 *  Thu Jun 29 22:59:08 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _GLPOINT_H
#define _GLPOINT_H

#include "VKGraphic.h"

#include "GLObject.h"
#include <Point3D.h>

class CPoint3D;

class VKGRAPHIC_API CGLPoint : public CGLObject
{
public:
	CGLPoint(const CPoint3D&, const GLPointMarker& = MARKER_POINT);
	virtual ~CGLPoint();
	virtual CGLObject* Copy() const;
	virtual void DefineDisplay();
	virtual void Display(const GLDisplayMode& = GLWIREFRAME);
	virtual void Hilight(const GLDisplayMode&);
	virtual CGeometry* Geometry() const;
private:
	void ComputeBoundLimits();
	void DrawMarker();
private:
	CPoint3D itsPoint;
	GLuint base;        /* Display list number of Marker */
    int    fwidths[256]; /* Width of each Marker in pixels */
    int    fheight;      /* Height of Marker */
	GLPointMarker itsMarker;
};

#endif /* _GLPOINT_H */
