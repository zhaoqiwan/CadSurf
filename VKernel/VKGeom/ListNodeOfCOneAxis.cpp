// ListNodeOfCOneAxis.cpp: implementation of the CListNodeOfCOneAxis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "ListNodeOfCOneAxis.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListNodeOfCOneAxis::CListNodeOfCOneAxis( const COneAxis& P) : data(P), nextPtr(0)
{

}

CListNodeOfCOneAxis::~CListNodeOfCOneAxis()
{

}

COneAxis CListNodeOfCOneAxis::GetData() const
{
	return data;
}

bool CListNodeOfCOneAxis::operator==(CListNodeOfCOneAxis& rhs)
{
	return (this == &rhs);
}

bool CListNodeOfCOneAxis::operator!=(CListNodeOfCOneAxis& rhs)
{
	return (this != &rhs);
}
