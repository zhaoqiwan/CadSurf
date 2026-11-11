// GeomException.cpp: implementation of the CGeomException class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "GeomException.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGeomException::CGeomException()
{

}

CGeomException::CGeomException(ErrorType type)
{
	msg = new char[50];
	switch (type)
	{
		case DivideByZero:
			msg = "\nDivide By Zero Raised!\n";
			break;
		case ConstructionFailure:
			msg = "\nConstruction Failure Raised!\n";
			break;
		case MaxDegree:
			msg = "\nDegree Greater Than Maximum Degree Allowed!\n";
				break;
		default:
			break;
	}
}

CGeomException::~CGeomException()
{
	delete [] msg;
}

char* CGeomException::ErrorMessage() const
{
	return msg;
}

