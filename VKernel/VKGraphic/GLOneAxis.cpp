// GLOneAxis.cpp: implementation of the CGLOneAxis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "GLOneAxis.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGLOneAxis::CGLOneAxis(const COneAxis& Ax, const int& length, const int& arrow) : itsAxis(Ax), 
															itsLength(length), itsArrow(arrow)
{
	glObjType = GLONEAXIS;
	glObjID = (int)glObjType + CGLObject::GetCount();
	itsRed = 125;
	itsGreen = 125;
	itsBlue = 220;
	ComputeBoundLimits();
}

CGLOneAxis::~CGLOneAxis()
{

}

CGLObject* CGLOneAxis::Copy() const
{
	CGLOneAxis* O = new CGLOneAxis(itsAxis, itsLength, itsArrow);
	O->SetColor(itsRed, itsGreen ,itsBlue);
	O->glObjID = glObjID;
	return O;
}

CGeometry* CGLOneAxis::Geometry() const
{
	COneAxis* Ax = new COneAxis(itsAxis.GetPosition(), itsAxis.GetDirection());
	return Ax;

}

void CGLOneAxis::DefineDisplay()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_DEPTH_TEST);

	glColor3ub(itsRed, itsGreen, itsBlue);
	DrawArrow(itsAxis);
}

void CGLOneAxis::Display(const GLDisplayMode&)
{
    DefineDisplay();
}

void CGLOneAxis::Hilight(const GLDisplayMode&)
{
	
	itsRed = 0; itsGreen = 255; itsBlue = 255;
	DefineDisplay();
}

void CGLOneAxis::ComputeBoundLimits()
{
	CPoint3D P1 = itsAxis.GetPosition();
	CPoint3D P2 = P1;
	P2.Translate(itsAxis, itsLength);
	itsBox.SetLimits(P1.GetX(), P2.GetX(), P1.GetY(), P2.GetY(), P1.GetZ(), P2.GetZ());
}

void CGLOneAxis::DrawArrow(const COneAxis& theAxis)
{
	CPoint3D pos = theAxis.GetPosition();
	pos.Translate(theAxis, itsLength-(itsArrow/2));
	CAxisSystem CordSys(pos, theAxis.GetDirection());
	int part=20;
	float step, uParam=0, vParam=0;
	step =2*PI/part;

	CVector3D P, P1, P2;
	float R = itsArrow/3.0f;

	vParam = itsArrow*2.0f;

	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glDisable(GL_LIGHTING);

	glBegin(GL_TRIANGLE_FAN);

	float c = cos(uParam);
	float s = sin(uParam);
	P = CordSys.GetOrigin()+(CordSys.GetXDirection()*c+
					CordSys.GetYDirection()*s)*0+CordSys.GetDirection()*(vParam);

	glVertex3d(P.GetX(),P.GetY(),P.GetZ());

	vParam = 0;
	for(uParam=0; uParam<=2*PI+2; uParam+=step)
	{		

		float c = cos(uParam);
		float s = sin(uParam);
		P = CordSys.GetOrigin()+(CordSys.GetXDirection()*c+
				CordSys.GetYDirection()*s)*R+CordSys.GetDirection()*(vParam);

		glVertex3d(P.GetX(),P.GetY(),P.GetZ());
	}
	glEnd();

	CPoint3D O = theAxis.GetPosition();
	CPoint3D D = O;
	D.Translate(theAxis, itsLength);
	glBegin(GL_LINES);
		glVertex3d(O.GetX(), O.GetY(), O.GetZ());
		glVertex3d(D.GetX(), D.GetY(), D.GetZ());
	glEnd();
}
