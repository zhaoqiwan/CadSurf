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
 *            GLPlane.h
 *
 *  Thu Jun 29 22:58:33 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _GLPLANE_H
#define _GLPLANE_H

#include "VKGraphic.h"

#include "GLObject.h"
#include <Point3D.h>
#include <Vector3D.h>
#include <Plane.h>

class CPlane;

class VKGRAPHIC_API CGLPlane : public CGLObject  
{
public:
	CGLPlane(const CPlane&, const double& = 200, const double& = 200);
	virtual ~CGLPlane();
	virtual CGLObject* Copy() const;
	virtual void DefineDisplay();
	virtual void Display(const GLDisplayMode& = GLWIREFRAME);
	virtual void Hilight(const GLDisplayMode&);
	virtual void SetColor(const GLubyte&, const GLubyte&, const GLubyte&);
	virtual void GetColor(GLubyte*) const;
	virtual CGeometry* Geometry() const;
private:
	void ComputeBoundLimits();
private:
	CPlane itsPlane;
	double uSize;
	double vSize;
};

#endif /* _GLPLANE_H */
