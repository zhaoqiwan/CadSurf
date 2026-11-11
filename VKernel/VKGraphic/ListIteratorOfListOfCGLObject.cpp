// ListIteratorOfListOfCGLObject.cpp: implementation of the CListIteratorOfListOfCGLObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "ListIteratorOfListOfCGLObject.h"
#include "ListException.h"
#include <MMath.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListIteratorOfListOfCGLObject::CListIteratorOfListOfCGLObject(CListOfCGLObject* aList) : theList(aList)
{
}

CListIteratorOfListOfCGLObject::~CListIteratorOfListOfCGLObject()
{
}

void CListIteratorOfListOfCGLObject::SetList(CListOfCGLObject* aList)
{
	theList = aList;
}

void CListIteratorOfListOfCGLObject::Init()
{
	curPtr = theList->firstPtr;
}

bool CListIteratorOfListOfCGLObject::More() const
{
	return (curPtr != 0);
}

bool CListIteratorOfListOfCGLObject::IsFound(const CGLObject* O) const
{
	bool b = false;
	CListIteratorOfListOfCGLObject li(theList);
	for(li.Init(); li.More(); li.Next())
	{
		if(li.Current() == O)
		{
			b = true;
			break;
		}
	}
	return b;
}

void CListIteratorOfListOfCGLObject::Next()
{
	curPtr = (curPtr) ? curPtr->nextPtr : 0;
}

CGLObject* CListIteratorOfListOfCGLObject::Current() const
{
	return (curPtr) ? curPtr->data : 0;
}

CListNodeOfCGLObject* CListIteratorOfListOfCGLObject::CurrentPtr() const
{
	return curPtr;
}

void CListIteratorOfListOfCGLObject::SetCurrent(CGLObject* Obj)
{
	curPtr->data = Obj;
}

CGLObject* CListIteratorOfListOfCGLObject::ValueAt(const int index)
{
	int cnt =0;
	Init();
	while(curPtr != 0)
	{
		Next();
		cnt++;
	}
	
	if(cnt < index)
		throw CListException(LIST_BOUND_UPPER);
		
	else if(index<1)
		throw CListException(LIST_BOUND_LOWER);
		
	/*else*/ if(index ==1 )
		return theList->firstPtr->data;
	else
	{
		Init();
		for(int i=1; i < index; i++)
		{
			if(curPtr->nextPtr == 0)
				break;
			Next();
		}
		
		return curPtr->data;
	}
		
}
 
