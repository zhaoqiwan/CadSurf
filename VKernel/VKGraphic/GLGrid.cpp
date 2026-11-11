// GLGrid.cpp: implementation of the CGLGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "GLGrid.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGLGrid::CGLGrid()
{
    itsPlane = CPlane::XOY();
    itsSize = 10000;
    itsStep = 50;
    itsRed = 125;
    itsGreen = 125;
    itsBlue = 125;
    glObjID = 0;
    firstTime = true;
}

CGLGrid::CGLGrid(const CPlane& plane, const double& size, const double& step) : itsSize(size),
										itsStep(step),
										itsPlane(plane)
{
    itsRed = 255;
    itsGreen = 125;
    itsBlue = 125;
    glObjID = 0;
    firstTime = true;
}

CGLGrid::~CGLGrid()
{
    glDeleteLists(gridList, 1);
}
CGLObject* CGLGrid::Copy() const
{
    CGLGrid* G = new CGLGrid(itsPlane, itsSize, itsStep);
    return G;
}

void CGLGrid::SetPlane(const CPlane& plane)
{ 
    itsPlane = plane; 
    firstTime = true;
}

void CGLGrid::SetSize(const double& size)
{
    itsSize = size;
    firstTime = true;
}

void CGLGrid::SetStep(const double& step)
{
    itsStep = step;
    firstTime = true;
}

void CGLGrid::DefineDisplay()
{
    if(firstTime)
    {
	InitDisplayLists();
	firstTime = false;
    }
    glCallList(gridList);
}

void CGLGrid::InitDisplayLists()
{
    gridList = glGenLists(1);
    glNewList(gridList, GL_COMPILE);
    double size = itsSize;
    double step = itsStep;
    CPlane plane = itsPlane;

    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_DEPTH_TEST);
    glColor3ub(itsRed, itsGreen, itsBlue);

    double i;
    for(i = -size/2; i <= size/2; i += step)
    {
	CPoint3D P1 = plane.PointAtPara(i, -size/2);
	CPoint3D P2 = plane.PointAtPara(i, size/2);
	glBegin(GL_LINES);
	glVertex3d(P1.GetX(), P1.GetY(), P1.GetZ());
	glVertex3d(P2.GetX(), P2.GetY(), P2.GetZ());
	glEnd();
    }
    for(i = -size/2; i <= size/2; i += step)
    {
	CPoint3D P1 = plane.PointAtPara(-size/2, i);
	CPoint3D P2 = plane.PointAtPara(size/2, i);
	glBegin(GL_LINES);
	glVertex3d(P1.GetX(), P1.GetY(), P1.GetZ());
	glVertex3d(P2.GetX(), P2.GetY(), P2.GetZ());
	glEnd();
    }
    glEndList();
}

void CGLGrid::Display(const GLDisplayMode& /*mode*/)
{
    DefineDisplay();
}

