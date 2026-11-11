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
 *            ListOfCGLObject.h
 *
 *  Thu Jun 29 20:47:35 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _LISTOFCGLOBJECT_H
#define _LISTOFCGLOBJECT_H

#include "VKGraphic.h"

#include "ListNodeOfCGLObject.h"

class CGLObject;
class CListIteratorOfListOfCGLObject;

class VKGRAPHIC_API CListOfCGLObject
{
	friend class CListIteratorOfListOfCGLObject;
public:
	CListOfCGLObject();
	virtual ~CListOfCGLObject();
	void Append(CGLObject*);
	void Prepend(CGLObject*);
	CGLObject* First() const;
	CGLObject* Last() const;
	CListNodeOfCGLObject* Previous(const CGLObject*);
	CGLObject* Remove(const CGLObject*);
	bool IsEmpty() const;
	void Clear();
private:
	CListNodeOfCGLObject* firstPtr;
	CListNodeOfCGLObject* lastPtr;
private:
	CListNodeOfCGLObject* NewNode(CGLObject*);
};

#endif /* _LISTOFCGLOBJECT_H */
