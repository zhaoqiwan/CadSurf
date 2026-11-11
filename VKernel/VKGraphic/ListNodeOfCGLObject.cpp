// ListNodeOfCGLObject.cpp: implementation of the CListNodeOfCGLObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "ListNodeOfCGLObject.h"
#include <MMath.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListNodeOfCGLObject::CListNodeOfCGLObject( CGLObject* P) : data(P), nextPtr(0)
{

}

CListNodeOfCGLObject::~CListNodeOfCGLObject()
{
	if(data)
		delete data;
}

CGLObject* CListNodeOfCGLObject::GetData() const
{
	return data;
}

bool CListNodeOfCGLObject::operator==(CListNodeOfCGLObject& rhs)
{
	return (this == &rhs);
}

bool CListNodeOfCGLObject::operator!=(CListNodeOfCGLObject& rhs)
{
	return (this != &rhs);
}
