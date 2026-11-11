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
 *            ListNodeOfCGLObject.h
 *
 *  Thu Jun 29 20:42:47 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _LISTNODEOFCGLOBJECT_H
#define _LISTNODEOFCGLOBJECT_H

#include "GLObject.h"

#include "VKGraphic.h"

//class CGLObject;
class CListOfCGLObject;

class VKGRAPHIC_API CListNodeOfCGLObject  
{
	friend class CListOfCGLObject;
	friend class CListIteratorOfListOfCGLObject;
public:
	CListNodeOfCGLObject(CGLObject*);
	CGLObject* GetData() const;
	virtual ~CListNodeOfCGLObject();
	bool operator==(CListNodeOfCGLObject& rhs);
	bool operator!=(CListNodeOfCGLObject& rhs);
private:
	CGLObject* data;
	CListNodeOfCGLObject* nextPtr;

};

#endif /* _LISTNODEOFCGLOBJECT_H */
