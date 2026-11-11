// BezierCurve.cpp: implementation of the CBezierCurve class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "BezierCurve.h"
#include "Point3D.h"
#include "GeomException.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBezierCurve::CBezierCurve()
{
	cType = BEZIER;
}

CBezierCurve::CBezierCurve(CListOfCPoint3D *aList)
{
	cType = BEZIER;
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
	if(cpCount > GetMaxDegree())
		throw CGeomException(MaxDegree);
}

CBezierCurve::~CBezierCurve()
{
	cpList->Clear();
	if(cpList)
		delete cpList;
}

void CBezierCurve::Reverse()
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

CPoint3D CBezierCurve::PointAtPara(const double& para)
{
	CPoint3D P;
	int cnt = 0;
	double x=0, y=0, z=0;
	double degree = cpCount-1;
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

	for(myListIter.Init(); myListIter.More(); myListIter.Next(), cnt++)
	{
		x += myListIter.Current().GetX()*BlendingFunc((int)degree,cnt,para);
		y += myListIter.Current().GetY()*BlendingFunc((int)degree,cnt,para);
		z += myListIter.Current().GetZ()*BlendingFunc((int)degree,cnt,para);
	}
	P.SetParam(x,y,z);
	return P;
}

double CBezierCurve::FirstParameter() const
{
	return 0.0;
}

double CBezierCurve::LastParameter() const
{
	return 1.0;
}

bool CBezierCurve::IsClosed() const
{
	return false;
}

CCurve* CBezierCurve::Copy() const
{
	CBezierCurve* C = new CBezierCurve(cpList);
	return C;
}

void CBezierCurve::Translate(const COneAxis& Ax, const double& amt)
{
	CGeometry::Translate(Ax, amt);
}

void CBezierCurve::Translate(double dx, double dy, double dz)
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


void CBezierCurve::Translate(const CVector3D& V)
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

void CBezierCurve::Translate(const CPoint3D& P1, const CPoint3D& P2)
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

void CBezierCurve::Rotate(const COneAxis& Ax, double ang)
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

void CBezierCurve::Scale(const CPoint3D& P, double fact)
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

void CBezierCurve::Mirror(const CPoint3D& P)
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

void CBezierCurve::Mirror(const COneAxis& Ax)
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

void CBezierCurve::Mirror(const CPlane& Pln)
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

double CBezierCurve::BlendingFunc(int n, int r, double t)
{
	long int p=1,m=1;
	int i;
	if(t<1e-5)
		return  (r==0 ?  1.0 : 0.0);
	if(t>.999995)
		return (r==n ? 1.0 : 0.0) ;

	for(i=1;i<=r;i++)
	{
		m*=i;
		p*=(n-i+1);
	}
	return ((double)(p/m))*pow(t,r)*pow(1.0-t,n-r);
}

