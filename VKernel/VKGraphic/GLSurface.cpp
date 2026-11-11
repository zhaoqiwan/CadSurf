// GLSurface.cpp: implementation of the CGLSurface class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "GLSurface.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGLSurface::CGLSurface(const CSurface* s)
{
    pSurf = s->Copy();
    glObjType = GLSURFACE;
    glObjID = (int)glObjType + CGLObject::GetCount();
    SMOOTH = 100;
    CSMOOTH = 100;

    itsShadeRed = 150;
    itsShadeGreen = 150;
    itsShadeBlue = 150;

    itsRed = 200;
    itsGreen = 225;
    itsBlue = 50;

    isoRed = 205;
    isoGreen = 205;
    isoBlue = 255;

    factor = 1;
    pattern = 0x5555;

    SetLight(GOLD);

    pointList = new CListOfCPoint3D;
    pointWFUList = new CListOfCPoint3D;
    pointWFVList = new CListOfCPoint3D;
    normalList = new CListOfCPoint3D;
    bpointList = new CListOfCPoint3D;

    myListWFUIter.SetList(pointWFUList);
    myListWFVIter.SetList(pointWFVList);

    ComputePoints();
    ComputeWFPoints();
    ComputeBoundLimits();
    InitDisplayList();
}

CGLSurface::~CGLSurface()
{
    if(pointList)
    {
	pointList->Clear();
	delete pointList;
    }
    if(pointWFUList)
    {
	pointWFUList->Clear();
	delete pointWFUList;
    }
    if(pointWFVList)
    {
	pointWFVList->Clear();
	delete pointWFVList;
    }
    if(normalList)
    {
	normalList->Clear();
	delete normalList;
    }
    if(bpointList)
    {
	bpointList->Clear();
	delete bpointList;
    }
    glDeleteLists(wireList, 1);
    glDeleteLists(edgeList, 1);
    glDeleteLists(shadList, 1);
    delete pSurf;
}

CGLObject* CGLSurface::Copy() const
{
    CGLSurface* S = new CGLSurface(pSurf);
    S->SetColor(itsRed, itsGreen, itsBlue);
    S->SetLight(GetMaterial());
    S->glObjID = glObjID;
    return S;
}

void CGLSurface::DefineDisplay()
{
    GLfloat bgcol[4];
    glGetFloatv(GL_COLOR_CLEAR_VALUE, bgcol);
    GLfloat  specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };
    // Enable Depth Testing
    glEnable(GL_DEPTH_TEST);


    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    // Enable lighting
    glEnable(GL_LIGHTING);


    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);

    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,specref);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,128);

    glShadeModel(GL_SMOOTH);

    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);

    if(displayMode == GLSHADED)
    {	
	glPushAttrib(GL_LIGHTING_BIT);
	ApplyMaterial(); //设置材料
	//SetLight();    //设置光源，已经注释掉，把光源作为全局环境公用
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(8, 8);
	DrawShaded();	
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glColor3ub(25, 25, 25);
	DrawEdges();
	glDisable(GL_POLYGON_OFFSET_FILL);
	glPopAttrib();
    }	
    else if(displayMode == GLWIREFRAME)
    {
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_DEPTH_TEST);
	glColor3ub(itsRed, itsGreen, itsBlue);
	DrawWired();
	glPopAttrib();
    }
    else if(displayMode == GLHLREMOVED)
    {	
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	glColor3f(bgcol[0], bgcol[1], bgcol[2]);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(5, 5);
	DrawShaded();
	DrawWired();

	glDisable(GL_POLYGON_OFFSET_FILL);
	glPopAttrib();
    }
}

void CGLSurface::Display(const GLDisplayMode& dMode)
{
    displayMode = dMode;
    DefineDisplay();
}

void CGLSurface::Hilight(const GLDisplayMode&)
{
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_DEPTH_TEST);
    itsRed = 0; itsGreen = 255; itsBlue = 255;
    isoRed = 0; isoGreen = 255; isoBlue = 255;
    glColor3ub(itsRed, itsGreen, itsBlue);
    DrawWired();
    glPopAttrib();
}

void CGLSurface::SetColor(const GLubyte& red, const GLubyte& green, const GLubyte& blue)
{
    if(displayMode == GLWIREFRAME)
    {
	itsRed = red;
	itsGreen = green;
	itsBlue = blue;
    }
    else
    {
	itsShadeRed = red;
	itsShadeGreen = green;
	itsShadeBlue = blue;
    }
    DefineDisplay();
}

void CGLSurface::GetColor(GLubyte* colVect) const
{
    colVect = new GLubyte[3];
    if(displayMode == GLWIREFRAME)
    {
	colVect[0] = itsRed;
	colVect[1] = itsGreen;
	colVect[2] = itsBlue;
    }
    else
    {

	colVect[0] = itsShadeRed;
	colVect[1] = itsShadeGreen;
	colVect[2] = itsShadeBlue;
    }
}

void CGLSurface::SetSurface(const CSurface* s)
{
    pSurf = s->Copy();
    ComputePoints();
    ComputeBoundLimits();
}

CGeometry* CGLSurface::Geometry() const
{
    CSurface* surf = pSurf->Copy();
    return surf;
}

void CGLSurface::ComputeBoundLimits()
{
    /*
       if(pSurf->IsOfType(SPHERE))
       {
       CSphere* pSph = static_cast<CSphere*> (pSurf);
       double rad = pSph->GetRadius();
       CPoint3D P = pSph->GetLocation().GetPosition();
       itsBox.SetLimits(-rad+P.GetX(), rad+P.GetX(), -rad+P.GetY(), rad+P.GetY(), -rad+P.GetZ(), rad+P.GetZ());
       return;
       }
       double lx=0,ly=0,lz=0,sx=0,sy=0,sz=0;
       CPoint3D P;
       QList<double> pxArray, pyArray, pzArray;
       pxArray.setAutoDelete(TRUE);
       pyArray.setAutoDelete(TRUE);
       pzArray.setAutoDelete(TRUE);

       CListIteratorOfListOfCPoint3D myListIter(bpointList);
       for(myListIter.Init(); myListIter.More(); myListIter.Next())
       {
       P = myListIter.Current();	
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

void CGLSurface::ComputePoints()
{
    SMOOTH *= 6;

    CPoint3D curP1;
    double uparts, vparts, istep, jstep;
    double fUPar, lUPar, fVPar, lVPar;

    double i, j;

    fUPar = pSurf->FirstUParameter();
    lUPar = pSurf->LastUParameter();

    fVPar = pSurf->FirstVParameter();
    lVPar = pSurf->LastVParameter();

    uparts = vparts = SMOOTH/15;

    istep = fabs(lUPar-fUPar)/uparts;
    jstep = fabs(lVPar-fVPar)/vparts;

    if(pointList && (!pointList->IsEmpty()))
	pointList->Clear();
    if(normalList && (!normalList->IsEmpty()))
	normalList->Clear();
    if(bpointList && (!bpointList->IsEmpty()))
	bpointList->Clear();


    for(j = fVPar; j <= lVPar; j += jstep)
    {
	for(i = fUPar; i <= lUPar; i += istep)
	{
	    curP1 = pSurf->PointAtPara(i, j);
	    bpointList->Append(curP1);
	}
    }

    CPoint3D curP; CVector3D N;

    for(j = fVPar; j < lVPar; j += jstep)
    {
	for(i = fUPar; i < lUPar; i += istep)
	{
	    curP = pSurf->PointAtPara(i, j);
	    N = pSurf->NormalAt(i,j);

	    N.Normalize();
	    normalList->Append(N.Point());
	    pointList->Append(curP);

	    curP = pSurf->PointAtPara(i, j+jstep);
	    N = pSurf->NormalAt(i,j+jstep);
	    N.Normalize();
	    normalList->Append(N.Point());
	    pointList->Append(curP);
	}
	curP = pSurf->PointAtPara(lUPar, j);
	N = pSurf->NormalAt(lUPar,j);
	N.Normalize();
	normalList->Append(N.Point());
	pointList->Append(curP);

	curP = pSurf->PointAtPara(lUPar, j+jstep);
	N = pSurf->NormalAt(lUPar,j+jstep);
	N.Normalize();

	normalList->Append(N.Point());
	pointList->Append(curP);
    }
}

void CGLSurface::ComputeWFPoints()
{
    double fUPar = pSurf->FirstUParameter();
    double lUPar = pSurf->LastUParameter();
    double fVPar = pSurf->FirstVParameter();
    double lVPar = pSurf->LastVParameter();
    double parts;
    if(pSurf->IsOfType(EXTRUDED) || pSurf->IsOfType(RULED))
	parts = 2;
    else
	parts = 3;
    double ustep = fabs(lUPar-fUPar)/parts;
    double vstep = fabs(lVPar-fVPar)/parts;
    double i;

    //Compute U ISO Lines
    ComputeUIso(fVPar);	//Edge
    for(i = fVPar+vstep; i < lVPar; i += vstep)
	ComputeUIso(i);
    ComputeUIso(lVPar);	//Edge

    //Compute V ISO Lines
    ComputeVIso(fUPar);	//Edge
    for(i = fUPar+ustep; i <= lUPar; i += ustep)
	ComputeVIso(i);
    ComputeVIso(lUPar);	//Edge
}

void CGLSurface::ComputeUIso(const double vPar)
{
    double fPar = pSurf->FirstUParameter();
    double lPar = pSurf->LastUParameter();
    double parts = CSMOOTH;
    double step = fabs(lPar-fPar)/parts;
    double i;
    CPoint3D curP;

    for(i = fPar; i <= lPar; i += step)
    {
	curP = pSurf->PointAtPara(i, vPar);
	pointWFUList->Append(curP);
    }
    curP = pSurf->PointAtPara(lPar, vPar);
    pointWFUList->Append(curP);
}

void CGLSurface::ComputeVIso(const double uPar)
{
    double fPar = pSurf->FirstVParameter();
    double lPar = pSurf->LastVParameter();
    double parts = CSMOOTH;
    double step = fabs(lPar-fPar)/parts;
    double i;
    CPoint3D curP;

    for(i = fPar; i <= lPar; i += step)
    {
	curP = pSurf->PointAtPara(uPar, i);
	pointWFVList->Append(curP);
    }
    curP = pSurf->PointAtPara(uPar, lPar);
    pointWFVList->Append(curP);
}

void CGLSurface::DrawUIso()
{
    double fPar = pSurf->FirstUParameter();
    double lPar = pSurf->LastUParameter();
    double parts = CSMOOTH;
    double step = fabs(lPar-fPar)/parts;
    double i;
    CPoint3D curP;

    glColor3ub(isoRed, isoGreen, isoBlue);
    glBegin(GL_LINE_STRIP);
    for(i = fPar; i <= lPar; i += step)
    {
	curP = myListWFUIter.Current();
	glVertex3d(curP.GetX(), curP.GetY(), curP.GetZ());
	myListWFUIter.Next();
    }
    curP = myListWFUIter.Current();
    glVertex3d(curP.GetX(), curP.GetY(), curP.GetZ());
    myListWFUIter.Next();
    glEnd();
}

void CGLSurface::DrawVIso()
{
    double fPar = pSurf->FirstVParameter();
    double lPar = pSurf->LastVParameter();
    double parts = CSMOOTH;
    double step = fabs(lPar-fPar)/parts;
    double i;
    CPoint3D curP;

    glColor3ub(isoRed, isoGreen, isoBlue);
    glBegin(GL_LINE_STRIP);
    for(i = fPar; i <= lPar; i += step)
    {
	curP = myListWFVIter.Current();
	glVertex3d(curP.GetX(), curP.GetY(), curP.GetZ());
	myListWFVIter.Next();
    }
    curP = myListWFVIter.Current();
    glVertex3d(curP.GetX(), curP.GetY(), curP.GetZ());
    myListWFVIter.Next();
    glEnd();
}

void CGLSurface::DrawWired()
{
    myListWFUIter.Init();
    myListWFVIter.Init();
    double fUPar = pSurf->FirstUParameter();
    double lUPar = pSurf->LastUParameter();
    double fVPar = pSurf->FirstVParameter();
    double lVPar = pSurf->LastVParameter();
    double parts;
    if(pSurf->IsOfType(EXTRUDED) || pSurf->IsOfType(RULED))
	parts = 2;
    else
	parts = 3;
    double ustep = fabs(lUPar-fUPar)/parts;
    double vstep = fabs(lVPar-fVPar)/parts;
    double i;

    glLineStipple(factor,pattern);

    //Draw U ISO Lines
    isoRed = itsRed;
    isoGreen = itsGreen;
    isoBlue = itsBlue;
    glDisable(GL_LINE_STIPPLE);
    DrawUIso();	//Edge
    for(i = fVPar+vstep; i < lVPar; i += vstep)
    {
	isoRed = 205;
	isoGreen = 205;
	isoBlue = 255;
	glEnable(GL_LINE_STIPPLE);
	DrawUIso();
    }
    isoRed = itsRed;
    isoGreen = itsGreen;
    isoBlue = itsBlue;
    glDisable(GL_LINE_STIPPLE);
    DrawUIso();	//Edge

    //Draw V ISO Lines
    DrawVIso();	//Edge
    for(i = fUPar+ustep; i <= lUPar; i += ustep)
    {	
	isoRed = 205;
	isoGreen = 205;
	isoBlue = 255;
	glEnable(GL_LINE_STIPPLE);
	DrawVIso();
    }
    isoRed = itsRed;
    isoGreen = itsGreen;
    isoBlue = itsBlue;
    glDisable(GL_LINE_STIPPLE);
    DrawVIso();	//Edge

    glDisable(GL_LINE_STIPPLE);
}

void CGLSurface::DrawShaded()
{
    CPoint3D curP; CPoint3D N;
    double uparts, vparts, istep, jstep;
    double fUPar, lUPar, fVPar, lVPar;

    double i, j;

    fUPar = pSurf->FirstUParameter();
    lUPar = pSurf->LastUParameter();

    fVPar = pSurf->FirstVParameter();
    lVPar = pSurf->LastVParameter();

    uparts = vparts = SMOOTH/15;

    istep = fabs(lUPar-fUPar)/uparts;
    jstep = fabs(lVPar-fVPar)/vparts;

    CListIteratorOfListOfCPoint3D myListIterP(pointList);
    CListIteratorOfListOfCPoint3D myListIterN(normalList);
    myListIterP.Init();
    myListIterN.Init();


    if(displayMode == GLSHADED) 
	glColor3ub(itsShadeRed, itsShadeGreen, itsShadeBlue);
    //else HLR

    glFrontFace(GL_CW);
    for(j = fVPar; j < lVPar; j += jstep)
    {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLE_STRIP);
	for(i = fUPar; i < lUPar; i += istep)
	{
	    curP = myListIterP.Current();
	    N = myListIterN.Current();
	    glNormal3d(N.GetX(), N.GetY(), N.GetZ());
	    glVertex3d(curP.GetX(), curP.GetY(), curP.GetZ());
	    myListIterP.Next();
	    myListIterN.Next();

	    curP = myListIterP.Current();
	    N = myListIterN.Current();
	    glNormal3d(N.GetX(), N.GetY(), N.GetZ());
	    glVertex3d(curP.GetX(), curP.GetY(), curP.GetZ());
	    myListIterP.Next();
	    myListIterN.Next();
	}
	curP = myListIterP.Current();
	N = myListIterN.Current();
	glNormal3d(N.GetX(), N.GetY(), N.GetZ());
	glVertex3d(curP.GetX(), curP.GetY(), curP.GetZ());
	myListIterP.Next();
	myListIterN.Next();

	curP = myListIterP.Current();
	N = myListIterN.Current();
	glNormal3d(N.GetX(), N.GetY(), N.GetZ());
	glVertex3d(curP.GetX(), curP.GetY(), curP.GetZ());
	myListIterP.Next();
	myListIterN.Next();

	glEnd();
    }
    glFrontFace(GL_CCW);
}

void CGLSurface::DrawEdges()
{
    myListWFUIter.Init();
    myListWFVIter.Init();
    double fUPar = pSurf->FirstUParameter();
    double lUPar = pSurf->LastUParameter();
    double fVPar = pSurf->FirstVParameter();
    double lVPar = pSurf->LastVParameter();
    double parts;
    if(pSurf->IsOfType(EXTRUDED) || pSurf->IsOfType(RULED))
	parts = 2;
    else
	parts = 3;
    double ustep = fabs(lUPar-fUPar)/parts;
    double vstep = fabs(lVPar-fVPar)/parts;
    double i;

    isoRed = 25;
    isoGreen = 25;
    isoBlue = 25;
    glDisable(GL_LINE_STIPPLE);
    glLineWidth(2.5);
    //Draw U ISO Lines    	
    DrawUIso();	//Edge
    for(i = fVPar+vstep; i < lVPar; i += vstep)
    {			
	DrawUIso();
    }    
    DrawUIso();	//Edge

    //Draw V ISO Lines
    DrawVIso();	//Edge
    for(i = fUPar+ustep; i <= lUPar; i += ustep)
    {		
	DrawVIso();
    }    	
    DrawVIso();	//Edge

    glLineWidth(1.0);
}

void CGLSurface::InitDisplayList()
{
    //wire list
    wireList = glGenLists(1);
    glNewList(wireList, GL_COMPILE);
    DrawWired();
    glEndList();

    //edges list
    edgeList = glGenLists(1);
    glNewList(edgeList, GL_COMPILE);
    DrawEdges();
    glEndList();

    //shaded list
    shadList = glGenLists(1);
    glNewList(shadList, GL_COMPILE);
    DrawShaded();
    glEndList();
}
