// BSplineCurve.cpp: implementation of the CBSplineCurve class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "BSplineCurve.h"
#include "Point3D.h"
#include "GeomException.h"

#include "Bsp.h" //For The Actual BSpline Algorithm 
				 //Copyright 1994 by Keith Vertanen (vertankd@cda.mrs.umn.edu)
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBSplineCurve::CBSplineCurve()
{
	cType = BSPLINE;
}

CBSplineCurve::CBSplineCurve(CListOfCPoint3D *aList, const int& degree) : itsDegree(degree)
{
	cType = BSPLINE;
	CPoint3D curP;
	cpList = new CListOfCPoint3D;
	cpCount = 0;
	CListIteratorOfListOfCPoint3D myListIter(aList);
	for(myListIter.Init(); myListIter.More(); myListIter.Next())
	{
		curP = myListIter.Current();
		cpList->Append(curP);
		cpCount++;
	}
	if(itsDegree >= cpCount)
		itsDegree = cpCount-1;


	if(itsDegree > GetMaxDegree())
		throw CGeomException(MaxDegree);
}

CBSplineCurve::~CBSplineCurve()
{
	cpList->Clear();
	if(cpList)
		delete cpList;
}

void CBSplineCurve::Reverse()
{
	CListOfCPoint3D *temp = new CListOfCPoint3D;
	CListIteratorOfListOfCPoint3D myListIter(cpList);

	for(myListIter.Init(); myListIter.More(); myListIter.Next())
		temp->Append(myListIter.Current());

	cpList->Clear();

	myListIter.SetList(temp);

	for(myListIter.Init(); myListIter.More(); myListIter.Next())
		cpList->Prepend(myListIter.Current());

	delete temp;
}

CPoint3D CBSplineCurve::PointAtPara(const double& para)
{
	CPoint3D curP;
	CPoint3D P;
	point p;
	int cnt = 0;
	int* u;
	int n = cpCount-1;
	int t = itsDegree+1;
	point calcxyz;
	CListIteratorOfListOfCPoint3D myListIter(cpList);

	if(para == FirstParameter())
	{
		P = cpList->First();
		return P;
	}


	if(para == LastParameter())
	{
		P = cpList->Last();
		return P;
	}

	u=new int[n+t+1];
	point *control = new point[n+1];

	for(myListIter.Init(); myListIter.More(); myListIter.Next())
	{
		curP = myListIter.Current();
		p.x = curP.GetX();
		p.y = curP.GetY();
		p.z = curP.GetZ();
		control[cnt] = p;
		cnt++;
	}

	compute_intervals(u, n, t);//Copyright 1994 by Keith Vertanen (vertankd@cda.mrs.umn.edu)
	compute_point(u, n, t, para, control, &calcxyz);//Copyright 1994 by Keith Vertanen (vertankd@cda.mrs.umn.edu)

    P.SetParam(calcxyz.x, calcxyz.y, calcxyz.z);
	delete u;
	delete [] control;
	return P;
}

double CBSplineCurve::FirstParameter() const
{
	return 0.0;
}

double CBSplineCurve::LastParameter() const
{
	int n = cpCount-1 ;
	int k = itsDegree+1;
	return (n-k+2);
}

bool CBSplineCurve::IsClosed() const
{
	return false;
}

CCurve* CBSplineCurve::Copy() const
{
	CBSplineCurve* C = new CBSplineCurve(cpList, itsDegree);
	return C;
}

void CBSplineCurve::Translate(const COneAxis& Ax, const double& amt)
{
	CGeometry::Translate(Ax, amt);
}

void CBSplineCurve::Translate(double dx, double dy, double dz)
{
	CListOfCPoint3D *temp = new CListOfCPoint3D;
	CPoint3D curP;
	CListIteratorOfListOfCPoint3D myListIter(cpList);
	for(myListIter.Init(); myListIter.More(); myListIter.Next())
	{
		curP = myListIter.Current();
		curP.Translate(dx,dy,dz);
		temp->Append(curP);
	}
	cpList->Clear();
	myListIter.SetList(temp);
	for(myListIter.Init(); myListIter.More(); myListIter.Next())
		cpList->Append(myListIter.Current());
	delete temp;
}

void CBSplineCurve::Translate(const CVector3D& V)
{
	CListOfCPoint3D *temp = new CListOfCPoint3D;
	CPoint3D curP;
	CListIteratorOfListOfCPoint3D myListIter(cpList);
	for(myListIter.Init(); myListIter.More(); myListIter.Next())
	{
		curP = myListIter.Current();
		curP.Translate(V);
		temp->Append(curP);
	}
	cpList->Clear();
	myListIter.SetList(temp);
	for(myListIter.Init(); myListIter.More(); myListIter.Next())
		cpList->Append(myListIter.Current());
	delete temp;
}

void CBSplineCurve::Translate(const CPoint3D& P1, const CPoint3D& P2)
{
	CListOfCPoint3D *temp = new CListOfCPoint3D;
	CPoint3D curP;
	CListIteratorOfListOfCPoint3D myListIter(cpList);
	for(myListIter.Init(); myListIter.More(); myListIter.Next())
	{
		curP = myListIter.Current();
		curP.Translate(P1, P2);
		temp->Append(curP);
	}
	cpList->Clear();
	myListIter.SetList(temp);
	for(myListIter.Init(); myListIter.More(); myListIter.Next())
		cpList->Append(myListIter.Current());
	delete temp;
}

void CBSplineCurve::Rotate(const COneAxis& Ax, double ang)
{
	CListOfCPoint3D *temp = new CListOfCPoint3D;
	CPoint3D curP;
	CListIteratorOfListOfCPoint3D myListIter(cpList);
	for(myListIter.Init(); myListIter.More(); myListIter.Next())
	{
		curP = myListIter.Current();
		curP.Rotate(Ax, ang);
		temp->Append(curP);
	}
	cpList->Clear();
	myListIter.SetList(temp);
	for(myListIter.Init(); myListIter.More(); myListIter.Next())
		cpList->Append(myListIter.Current());
	delete temp;
}

void CBSplineCurve::Scale(const CPoint3D& P, double fact)
{
	CListOfCPoint3D *temp = new CListOfCPoint3D;
	CPoint3D curP;
	CListIteratorOfListOfCPoint3D myListIter(cpList);
	for(myListIter.Init(); myListIter.More(); myListIter.Next())
	{
		curP = myListIter.Current();
		curP.Scale(P, fact);
		temp->Append(curP);
	}
	cpList->Clear();
	myListIter.SetList(temp);
	for(myListIter.Init(); myListIter.More(); myListIter.Next())
		cpList->Append(myListIter.Current());
	delete temp;
}

void CBSplineCurve::Mirror(const CPoint3D& P)
{
	CListOfCPoint3D *temp = new CListOfCPoint3D;

	CPoint3D curP;
	CListIteratorOfListOfCPoint3D myListIter(cpList);
	for(myListIter.Init(); myListIter.More(); myListIter.Next())
	{
		curP = myListIter.Current();
		curP.Mirror(P);
		temp->Append(curP);
	}
	cpList->Clear();
	myListIter.SetList(temp);
	for(myListIter.Init(); myListIter.More(); myListIter.Next())
		cpList->Append(myListIter.Current());
	delete temp;
}

void CBSplineCurve::Mirror(const COneAxis& Ax)
{
	CListOfCPoint3D *temp = new CListOfCPoint3D;
	CPoint3D curP;
	CListIteratorOfListOfCPoint3D myListIter(cpList);
	for(myListIter.Init(); myListIter.More(); myListIter.Next())
	{
		curP = myListIter.Current();
		curP.Mirror(Ax);
		temp->Append(curP);
	}
	cpList->Clear();
	myListIter.SetList(temp);
	for(myListIter.Init(); myListIter.More(); myListIter.Next())
		cpList->Append(myListIter.Current());
	delete temp;
}

void CBSplineCurve::Mirror(const CPlane& Pln)
{
	CListOfCPoint3D *temp = new CListOfCPoint3D;
	CPoint3D curP;
	CListIteratorOfListOfCPoint3D myListIter(cpList);
	for(myListIter.Init(); myListIter.More(); myListIter.Next())
	{
		curP = myListIter.Current();
		curP.Mirror(Pln);
		temp->Append(curP);
	}
	cpList->Clear();
	myListIter.SetList(temp);
	for(myListIter.Init(); myListIter.More(); myListIter.Next())
		cpList->Append(myListIter.Current());
	delete temp;
}

