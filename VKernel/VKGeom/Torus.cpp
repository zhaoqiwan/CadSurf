// Torus.cpp: implementation of the CTorus class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "Torus.h"
#include "Circle3D.h"
#include "GeomException.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTorus::CTorus()
{
    sType = TORUS;
    itsLocation = CAxisSystem(CPoint3D(0,0,0), CVector3D(0,0,1));
    majorRadius = 10.0;
    minorRadius = 5.0;
    itsFirstUParameter = 0;
    itsLastUParameter = 2*PI;
    itsFirstVParameter = 0;
    itsLastVParameter = 2*PI;
}

CTorus::CTorus(const CAxisSystem& Ax, const double& maj, const double& min, const double& uStart, const double& uEnd, const double& vStart, const double& vEnd) : majorRadius(maj), minorRadius(min)
{

    sType = TORUS;
    itsLocation = Ax;
    itsFirstUParameter = uStart;
    itsLastUParameter = uEnd;
    itsFirstVParameter = vStart;
    itsLastVParameter = vEnd;
    if(itsFirstUParameter < 0 || itsFirstUParameter > 2*PI)
    {
	cout << "Invalid U Range .... CTorus::CTorus(const CAxisSystem& Ax, const double& maj, const double& min, const double& uStart, const double& uEnd, const double& vStart, const double& vEnd)\n";
	throw CGeomException(InvalidRange);
    }
    if(itsLastUParameter < 0 || itsLastUParameter > 2*PI)
    {
	cout << "Invalid U Range .... CTorus::CTorus(const CAxisSystem& Ax, const double& maj, const double& min, const double& uStart, const double& uEnd, const double& vStart, const double& vEnd)\n";
	throw CGeomException(InvalidRange);
    }
    if(itsFirstVParameter < 0 || itsFirstVParameter > 2*PI)
    {
	cout << "Invalid V Range .... CTorus::CTorus(const CAxisSystem& Ax, const double& maj, const double& min, const double& uStart, const double& uEnd, const double& vStart, const double& vEnd)\n";
	throw CGeomException(InvalidRange);
    }
    if(itsLastVParameter < 0 || itsLastVParameter > 2*PI)
    {
	cout << "Invalid V Range .... CTorus::CTorus(const CAxisSystem& Ax, const double& maj, const double& min, const double& uStart, const double& uEnd, const double& vStart, const double& vEnd)\n";
	throw CGeomException(InvalidRange);
    }
    if(itsLastUParameter <  itsFirstUParameter)
    {
	cout << "Invalid Range .... CTorus::CTorus(const CAxisSystem& Ax, const double& maj, const double& min, const double& uStart, const double& uEnd, const double& vStart, const double& vEnd)\n";
	throw CGeomException(InvalidRange);
    }
    if(itsLastVParameter <  itsFirstVParameter)
    {
	cout << "Invalid Range .... CTorus::CTorus(const CAxisSystem& Ax, const double& maj, const double& min, const double& uStart, const double& uEnd, const double& vStart, const double& vEnd)\n";
	throw CGeomException(InvalidRange);
    }
}

CTorus::~CTorus()
{

}

CPoint3D CTorus::PointAtPara(const double uPar, const double vPar)
{
    CVector3D P, O, XDir, YDir, ZDir;
    double r = minorRadius, R = majorRadius;
    O = itsLocation.GetOrigin();
    ZDir = itsLocation.GetDirection();
    XDir = itsLocation.GetXDirection();
    YDir = itsLocation.GetYDirection();
    double cu = cos(uPar);
    double su = sin(uPar);
    double cv = cos(vPar);
    double sv = sin(vPar);
    P = O + (((XDir*cu + YDir*su) * (R + r*cv)) ) + ZDir*r*sv;
    return P.Point();
}

CVector3D CTorus::NormalAt(const double uPar, const double vPar)
{
    CCircle3D C(itsLocation, majorRadius);
    CPoint3D O = C.PointAtPara(uPar);
    CPoint3D P = PointAtPara(uPar, vPar);
    CVector3D N(O, P);
    return N;
}

bool CTorus::IsUClosed() const
{
    if(itsFirstUParameter == 0 && itsLastUParameter == 2*PI)
	return true;
    return false;
}

bool CTorus::IsVClosed() const
{
    if(itsFirstVParameter == 0 && itsLastVParameter == 2*PI)
	return true;
    return false;
}

CSurface* CTorus::Copy() const
{
    CTorus* T = new CTorus(itsLocation, majorRadius, minorRadius, itsFirstUParameter, itsLastUParameter, itsFirstVParameter, itsLastVParameter);
    return T;
}

void CTorus::Translate(const COneAxis& Ax, const double& amt)
{
    CGeometry::Translate(Ax, amt);
}

void CTorus::Translate(double dx, double dy, double dz)
{
    itsLocation.Translate(dx,dy,dz);
}

void CTorus::Translate(const CVector3D& V)
{
    itsLocation.Translate(V);
}

void CTorus::Translate(const CPoint3D& P1, const CPoint3D& P2)
{
    itsLocation.Translate(P1, P2);
}

void CTorus::Rotate(const COneAxis& Ax, double ang)
{
    itsLocation.Rotate(Ax, ang);
}

void CTorus::Scale(const CPoint3D& P, double fact)
{
    itsLocation.Scale(P, fact);
}

void CTorus::Mirror(const CPoint3D& P)
{
    itsLocation.Mirror(P);
}

void CTorus::Mirror(const COneAxis& Ax)
{
    itsLocation.Mirror(Ax);
}

void CTorus::Mirror(const CPlane& Pln)
{
    itsLocation.Mirror(Pln);
}
