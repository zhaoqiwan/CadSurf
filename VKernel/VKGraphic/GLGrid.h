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
 *            GLGrid.h
 *
 *  Thu Jun 29 22:52:58 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _GLGRID_H
#define _GLGRID_H

#include "VKGraphic.h"

#include "GLObject.h"
#include <Plane.h>
#include <OneAxis.h>

class VKGRAPHIC_API CGLGrid : public CGLObject  
{
public:
	CGLGrid();
	CGLGrid(const CPlane& plane, const double& size = 100, const double& step = 10);
	virtual ~CGLGrid();
	void SetPlane(const CPlane& plane);
	void SetSize(const double& size);
	void SetStep(const double& step);
	CPlane GetPlane() const { return itsPlane; }
	double GetSize() const { return itsSize; }
	double GetStep() const { return itsStep; }
	virtual CGLObject* Copy() const;
	virtual void DefineDisplay();
	virtual void Display(const GLDisplayMode& = GLWIREFRAME);
	virtual void Hilight(const GLDisplayMode&) {;}
private:
	double itsSize;
	double itsStep;
	CPlane itsPlane;
	GLuint gridList;
private:
	void InitDisplayLists();
};

#endif /* _GLGRID_H */
