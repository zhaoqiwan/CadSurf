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
 *            ListIteratorOfListOfCPoint3D.h
 *
 *  Thu Jun 29 20:40:28 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _LISTITERATOROFLISTOFCPOINT3D_H
#define _LISTITERATOROFLISTOFCPOINT3D_H

#include "VKGeom.h"

#include "ListNodeOfCPoint3D.h"
#include "ListOfCPoint3D.h"

class VKGEOM_API CListIteratorOfListOfCPoint3D
{
public:
	CListIteratorOfListOfCPoint3D() {;}
	CListIteratorOfListOfCPoint3D(CListOfCPoint3D*);
	virtual ~CListIteratorOfListOfCPoint3D();
	void SetList(CListOfCPoint3D*);
	void Init();
	bool More() const;
	void Next();
	CPoint3D Current() const;
	CPoint3D ValueAt(const int index);
private:
	CListNodeOfCPoint3D* curPtr;
	CListOfCPoint3D* theList;
};

#endif /* _LISTITERATOROFLISTOFCPOINT3D_H */
