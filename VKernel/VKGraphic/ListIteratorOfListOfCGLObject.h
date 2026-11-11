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
 *            ListIteratorOfListOfCGLObject.h
 *
 *  Thu Jun 29 20:37:00 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/

#ifndef _LISTITERATOROFLISTOFCGLOBJECT_H
#define _LISTITERATOROFLISTOFCGLOBJECT_H

#include "VKGraphic.h"

#include "ListNodeOfCGLObject.h"
#include "ListOfCGLObject.h"

class VKGRAPHIC_API CListIteratorOfListOfCGLObject
{
public:
	CListIteratorOfListOfCGLObject(){}
	CListIteratorOfListOfCGLObject(CListOfCGLObject*);
	virtual ~CListIteratorOfListOfCGLObject();
	void SetList(CListOfCGLObject*);
	void Init();
	bool More() const;
	bool IsFound(const CGLObject*) const;
	void Next();
	CGLObject* Current() const;
	CListNodeOfCGLObject* CurrentPtr() const;
	void SetCurrent(CGLObject*);
	CGLObject* ValueAt(const int index);
private:
	CListNodeOfCGLObject* curPtr;
	CListOfCGLObject* theList;
};

#endif /* _LISTITERATOROFLISTOFCGLOBJECT_H */
