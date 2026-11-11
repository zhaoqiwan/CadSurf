// GLCurve.cpp: implementation of the CGLCurve class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "GLCurve.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGLCurve::CGLCurve(const CCurve* crv , const GLLineStyle& lt) : lineStyle(lt)
{
	pCurve = crv->Copy();
	glObjType = GLCURVE;
	glObjID = (int)glObjType + CGLObject::GetCount();

	SMOOTH = 1000;

	itsRed = 255;
	itsGreen = 0;
	itsBlue = 255;

	pointList = new CListOfCPoint3D;
	ComputePoints();
	ComputeBoundLimits();

	listNum = 1;
}

CGLCurve::~CGLCurve()
{
	if(pointList)
	{
		pointList->Clear();
		delete pointList;
	}
	delete pCurve;
}

CGLObject* CGLCurve::Copy() const
{
	CGLCurve* C = new CGLCurve(pCurve, lineStyle);
	C->SetColor(itsRed, itsGreen, itsBlue);
	C->SetSmooth(SMOOTH);
	C->glObjID = glObjID;
	return C;
}

CGeometry* CGLCurve::Geometry() const
{
	CCurve* crv = pCurve->Copy();
	return crv;
}

void CGLCurve::DefineDisplay()
{
	CPoint3D curP;
	CListIteratorOfListOfCPoint3D myListIter(pointList);

	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_DEPTH_TEST);

	glColor3ub(itsRed, itsGreen, itsBlue);

	if(lineStyle == LINE_SOLID)
		glDisable(GL_LINE_STIPPLE);
	else
		glEnable(GL_LINE_STIPPLE);

	if(lineStyle == LINE_DOTTED)
		glLineStipple (1, 0x0101);  /*  dotted  */
	if(lineStyle == LINE_DASHED)
		glLineStipple (1, 0x00FF);  /*  dashed  */
	if(lineStyle == LINE_DASHDOTTED)
		glLineStipple (2, 0x1C47);  /*  dash/dot/dash  */

	glBegin(GL_LINE_STRIP);		//LINE_STRIP So that it will be common for Open and Closed Curves
	for(myListIter.Init(); myListIter.More(); myListIter.Next())
	{
		curP = myListIter.Current();
		glVertex3d(curP.GetX(), curP.GetY(), curP.GetZ());
	}
	glEnd();

	glDisable(GL_LINE_STIPPLE);
}

void CGLCurve::Display(const GLDisplayMode&)
{
	DefineDisplay();
}

void CGLCurve::DisplayRubber()
{
	itsRed = 255; itsGreen = 255; itsBlue = 0;
	glColor3ub(itsRed, itsGreen, itsBlue);
	CPoint3D curP;
	CListIteratorOfListOfCPoint3D myListIter(pointList);

	glBegin(GL_LINE_STRIP);		//LINE_STRIP So that it will be common for Open and Closed Curves
	for(myListIter.Init(); myListIter.More(); myListIter.Next())
	{
		curP = myListIter.Current();
		glVertex3d(curP.GetX(), curP.GetY(), curP.GetZ());
	}
	glEnd();
}

void CGLCurve::Hilight(const GLDisplayMode&)
{
	itsRed = 0; itsGreen = 255; itsBlue = 255;
	DefineDisplay();
}

void CGLCurve::SetSmooth(const double& sm)
{
	SMOOTH = sm;
	ComputePoints();
	ComputeBoundLimits();
	DefineDisplay();
}

void CGLCurve::SetCurve(const CCurve* crv)
{
	pCurve = crv->Copy();
	ComputePoints();
	ComputeBoundLimits();
}

void CGLCurve::ComputePoints()
{
	CPoint3D sP, eP, curP;
	double parts , step;
	double fPar, lPar;

	fPar = pCurve->FirstParameter();
	lPar = pCurve->LastParameter();

	parts = SMOOTH/10;

	step = fabs(lPar-fPar)/parts;

	sP = pCurve->PointAtPara(fPar);

	if(pCurve->IsClosed())
		eP = sP;
	else
		eP = pCurve->PointAtPara(lPar);

	if(!pointList->IsEmpty())
		pointList->Clear();

	if(pCurve->IsOfType(LINE))
	{
		pointList->Append(sP);
		pointList->Append(eP);
	}
	else
	{
		for(double i = fPar; i < lPar; i += step)
		{
			curP = pCurve->PointAtPara(i);
			pointList->Append(curP);
		}
		pointList->Append(eP);//To Ensure Closure If Curve Is Closed
	}
}

void CGLCurve::ComputeBoundLimits()
{
	/*
	double lx=0,ly=0,lz=0,sx=0,sy=0,sz=0;
	CPoint3D P;
	QList<double> pxArray, pyArray, pzArray;
	pxArray.setAutoDelete(TRUE);
	pyArray.setAutoDelete(TRUE);
	pzArray.setAutoDelete(TRUE);
	
	CListIteratorOfListOfCPoint3D myListIter(pointList);
	for(myListIter.Init(); myListIter.More(); myListIter.Next())
	{
		double *x = new double, *y = new double, *z = new double;
		*x = P.GetX(); *y = P.GetY(); *z = P.GetZ();
		pxArray.append(x);
		pyArray.append(y);
		pzArray.append(z);
	}
	pxArray.sort();
	pyArray.sort();
	pzArray.sort();
	sx = *pxArray.first();  lx = *pxArray.last();
	sy = *pyArray.first();  ly = *pyArray.last();
	sz = *pzArray.first();  lz = *pzArray.last();
	itsBox.SetLimits(sx,lx,sy,ly,sz,lz);
	*/
}
