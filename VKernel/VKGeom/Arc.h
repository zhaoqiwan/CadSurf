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
 *            Arc.h
 *
 *  Thu Jun 29 21:59:32 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _ARC_H
#define _ARC_H

#include "VKGeom.h"

#include "Circle3D.h"
#include "Point3D.h"

#include "ListOfCPoint3D.h"
#include "ListIteratorOfListOfCPoint3D.h"

class VKGEOM_API CArc : public CCircle3D  
{
public:
	CArc(int plane, const CPoint3D& P1, const CPoint3D& P2, const CPoint3D& Cen, double step, bool clock);
	virtual ~CArc();
public:

	CListOfCPoint3D* ArcPoints;
};

#endif /* _ARC_H */
