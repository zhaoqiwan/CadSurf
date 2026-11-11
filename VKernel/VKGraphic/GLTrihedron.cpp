// GLTrihedron.cpp: implementation of the CGLTrihedron class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "GLTrihedron.h"
#include "MMath.h"

#include <cstring>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGLTrihedron::CGLTrihedron()
{
    glObjType = GLTRIHEDRON;
    coneObj = gluNewQuadric();
    gluQuadricOrientation(coneObj, GLU_OUTSIDE);
    gluQuadricDrawStyle(coneObj,GLU_SILHOUETTE);
    firstTime = true;
    triList = 0;
    InitDisplayLists();
}

CGLTrihedron::CGLTrihedron(GLfloat size)
{
    glObjType = GLTRIHEDRON;
    coneObj = gluNewQuadric();
    gluQuadricOrientation(coneObj, GLU_OUTSIDE);
    gluQuadricDrawStyle(coneObj,GLU_SILHOUETTE);
    csSize = size;
    ComputeBoundLimits();
    glObjID = (int)glObjType + CGLObject::GetCount();
    firstTime = true;
    triList = 0;
    InitDisplayLists();
}

CGLTrihedron::~CGLTrihedron()
{
    glDeleteLists(base, 96);
    glDeleteLists(triList, 1);
}

CGLObject* CGLTrihedron::Copy() const
{
    CGLTrihedron* T = new CGLTrihedron(csSize);
    T->glObjID = glObjID;
    return T;
}

void CGLTrihedron::DefineDisplay()
{
    glCallList(triList);
}

void CGLTrihedron::InitDisplayLists()
{
    if(firstTime)
    {
	Font = FontCreate(wglGetCurrentDC(), "Monospace", 15, 0, 0);
	if(triList)
	    glDeleteLists(triList, 1);
	triList = glGenLists(1);
	glNewList(triList, GL_COMPILE);
	firstTime = false;
    }
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    //glDisable(GL_DEPTH_TEST);

    //Axes' label
    glColor3f(1.0f, 1.0f, 0.0f);
    glRasterPos3d(0.5*csSize, 0, 0);
    DrawText("X");
    glRasterPos3d(0, 0.5*csSize, 0);
    DrawText("Y");
    glRasterPos3d(0, 0, 0.5*csSize);
    DrawText("Z");

    glColor3f(1.0f, 0, 0);
    //X Axis
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.8f*csSize*0.5f, 0.0f, 0.0f);
    glEnd();
    glPushMatrix();	
    glTranslatef(0.8f*csSize*0.5f, 0.0f, 0.0f);
    glRotatef(90.0f,0.0f,1.0f,0.0f);
    gluCylinder(coneObj,0.027*csSize,0,0.09*csSize,10,10);
    glPopMatrix();


    glColor3f(0, 1.0f, 0);
    //Y Axis
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.8f*csSize*0.5f, 0.0f);
    glEnd();
    glPushMatrix();	
    glTranslatef(0.0f, 0.8f*csSize*0.5f, 0.0f);
    glRotatef(-90.0f,1.0f,0.0f,0.0f);
    gluCylinder(coneObj,0.027*csSize,0,0.09*csSize,10,10);
    glPopMatrix();

    glColor3f(0, 0, 1.0f);
    //Z Axis
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.8f*csSize*0.5f);
    glEnd();
    glPushMatrix();	
    glTranslatef(0.0f, 0.0f, 0.8f*csSize*0.5f);
    gluCylinder(coneObj,0.027*csSize,0,0.09*csSize,10,10);
    glPopMatrix();

    glEndList();
}

void CGLTrihedron::Display(const GLDisplayMode& /*dMode*/)
{
    DefineDisplay();
}

void CGLTrihedron::Hilight(const GLDisplayMode& /*dMode*/)
{
}

void CGLTrihedron::SetSize(GLfloat size)
{
    csSize = size;
    firstTime = true;
    InitDisplayLists();
}

void CGLTrihedron::DrawText(char* text)
{
    FontPrintf(Font, 1, text);
}

void CGLTrihedron::ComputeBoundLimits()
{
    itsBox.SetLimits(-csSize, csSize, -csSize, csSize, -csSize, csSize);
}
