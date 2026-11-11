// ListOfCGLObject.cpp: implementation of the CListOfCGLObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "ListOfCGLObject.h"
#include "ListIteratorOfListOfCGLObject.h"
#include "ListException.h"
#include <MMath.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListOfCGLObject::CListOfCGLObject() : firstPtr(0), lastPtr(0)
{
}


CListOfCGLObject::~CListOfCGLObject()
{
	CListNodeOfCGLObject* curPtr = firstPtr, *tmp;
	if(!IsEmpty())
	{
		while(curPtr != 0)
		{
			tmp = curPtr;
			if(tmp->data)
			{
				delete tmp->data;
				tmp->data = 0;
			}
			curPtr = curPtr->nextPtr;
			delete tmp;
		}
	}
}


void CListOfCGLObject::Append( CGLObject* data)
{
	CListNodeOfCGLObject* newPtr = new CListNodeOfCGLObject(data);
	if(IsEmpty())
		firstPtr = lastPtr = newPtr;
	else
	{
		lastPtr->nextPtr = newPtr;
		lastPtr = newPtr;
	}

}


void CListOfCGLObject::Prepend( CGLObject* data)
{
	CListNodeOfCGLObject* newPtr = new CListNodeOfCGLObject(data);
	if(IsEmpty())
		firstPtr = lastPtr = newPtr;
	else
	{
		newPtr->nextPtr = firstPtr;
		firstPtr = newPtr;
	}
}

void CListOfCGLObject::InsertAt(CGLObject* data, std::size_t index)
{
	if (index == 0 || IsEmpty())
	{
		Prepend(data);
		return;
	}

	CListNodeOfCGLObject* previous = firstPtr;
	while (index > 1 && previous->nextPtr)
	{
		previous = previous->nextPtr;
		--index;
	}

	CListNodeOfCGLObject* node = new CListNodeOfCGLObject(data);
	node->nextPtr = previous->nextPtr;
	previous->nextPtr = node;
	if (!node->nextPtr)
		lastPtr = node;
}

CGLObject* CListOfCGLObject::First() const
{
	if(IsEmpty())
		throw CListException(LIST_EMPTY);
	return firstPtr->GetData();
}

CGLObject* CListOfCGLObject::Last() const
{
	if(IsEmpty())
		throw CListException(LIST_EMPTY);
	return lastPtr->GetData();
}


CListNodeOfCGLObject* CListOfCGLObject::Previous(const CGLObject* O)
{
	CListIteratorOfListOfCGLObject it(this);
	for(it.Init(); it.More(); it.Next())
	{
		if(O == it.CurrentPtr()->nextPtr->data)
			break;
	}
	return it.CurrentPtr();
}

CGLObject* CListOfCGLObject::Remove(const CGLObject* O)
{
	if(IsEmpty())
		return 0;
	CListNodeOfCGLObject* tmp = 0;
	if(O == First())
	{
		tmp = firstPtr;
		firstPtr = firstPtr->nextPtr;
		if(tmp == lastPtr)
			lastPtr = firstPtr;
	}
	else
	{
		CListNodeOfCGLObject* previous = firstPtr;
		while(previous && previous->nextPtr && previous->nextPtr->data != O)
			previous = previous->nextPtr;
		if(!previous || !previous->nextPtr)
			return 0;

		tmp = previous->nextPtr;
		previous->nextPtr = tmp->nextPtr;
		if(tmp == lastPtr)
			lastPtr = previous;
	}

	CGLObject* rO = tmp->data;
	tmp->data = 0;
	delete tmp;

	return rO;
}

bool CListOfCGLObject::IsEmpty() const
{
	return (firstPtr==0);
}		


void CListOfCGLObject::Clear()
{
	CListNodeOfCGLObject* curPtr = firstPtr, *tmp;
	if(!IsEmpty())
	{
		while(curPtr != 0)
		{
			tmp = curPtr;
			if(tmp->data)
			{
				delete tmp->data;
				tmp->data = 0;
			}
			curPtr = curPtr->nextPtr;
			delete tmp;
		}
		
		firstPtr = lastPtr = 0;
	}
}

CListNodeOfCGLObject* CListOfCGLObject::NewNode(CGLObject* P)
{
	CListNodeOfCGLObject* newPtr= new CListNodeOfCGLObject(P);
	if(!newPtr)
		throw CListException(LIST_OUT_OF_MEMORY);
	newPtr->nextPtr = 0;
	return newPtr;
}
