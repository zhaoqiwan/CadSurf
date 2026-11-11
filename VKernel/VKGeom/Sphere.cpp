// Sphere.cpp: implementation of the CSphere class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "Sphere.h"
#include "GeomException.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//
CSphere::CSphere()
{
    sType = SPHERE;
    itsLocation = CAxisSystem(CPoint3D(0,0,0), CVector3D(0,0,1));
    itsRadius = 10.0;
    itsFirstUParameter = 0;
    itsLastUParameter = 2*PI;
    itsFirstVParameter = -PI/2;
    itsLastVParameter = PI/2;
}

CSphere::CSphere(const CAxisSystem& Ax, const double& rad, const double& uStart, const double& uEnd, const double& vStart, const double& vEnd) : itsRadius(rad)
{
    sType = SPHERE;
    itsLocation = Ax;
    itsFirstUParameter = uStart;
    itsLastUParameter = uEnd;
    itsFirstVParameter = vStart;
    itsLastVParameter = vEnd;
    if(itsFirstUParameter < 0 || itsFirstUParameter > 2*PI)
    {
	cout << "Invalid U Range .... CSphere::CSphere(const CAxisSystem& Ax, const double& rad, const double& uStart, const double& uEnd, const double& vStart, const double& vEnd)\n";
	throw CGeomException(InvalidRange);
    }
    if(itsLastUParameter < 0 || itsLastUParameter > 2*PI)
    {
	cout << "Invalid U Range .... CSphere::CSphere(const CAxisSystem& Ax, const double& rad, const double& uStart, const double& uEnd, const double& vStart, const double& vEnd)\n";
	throw CGeomException(InvalidRange);
    }
    if(itsFirstVParameter < -PI/2 || itsFirstVParameter > PI/2)
    {
	cout << "Invalid V Range .... CSphere::CSphere(const CAxisSystem& Ax, const double& rad, const double& uStart, const double& uEnd, const double& vStart, const double& vEnd)\n";
	throw CGeomException(InvalidRange);
    }
    if(itsLastVParameter < -PI/2 || itsLastVParameter > PI/2)
    {
	cout << "Invalid V Range .... CSphere::CSphere(const CAxisSystem& Ax, const double& rad, const double& uStart, const double& uEnd, const double& vStart, const double& vEnd)\n";
	throw CGeomException(InvalidRange);
    }
    if(itsLastUParameter <  itsFirstUParameter)
    {
	cout << "Invalid Range .... CSphere::CSphere(const CAxisSystem& Ax, const double& rad, const double& uStart, const double& uEnd, const double& vStart, const double& vEnd)\n";
	throw CGeomException(InvalidRange);
    }
    if(itsLastVParameter <  itsFirstVParameter)
    {
	cout << "Invalid Range .... CSphere::CSphere(const CAxisSystem& Ax, const double& rad, const double& uStart, const double& uEnd, const double& vStart, const double& vEnd)\n";
	throw CGeomException(InvalidRange);
    }
}

CSphere::~CSphere()
{

}

CPoint3D CSphere::PointAtPara(const double uPar, const double vPar)
{
    CVector3D P, O, XDir, YDir, ZDir;
    O = itsLocation.GetOrigin();
    ZDir = itsLocation.GetDirection();
    XDir = itsLocation.GetXDirection();
    YDir = itsLocation.GetYDirection();
    double cu = cos(uPar);
    double su = sin(uPar);
    double cv = cos(vPar);
    double sv = sin(vPar);
    P = O + (XDir*cu + YDir*su)*itsRadius*cv + ZDir*itsRadius*sv;
    return P.Point();
}

CVector3D CSphere::NormalAt(const double uPar, const double vPar)
{
    CPoint3D O = itsLocation.GetPosition();
    CPoint3D P = PointAtPara(uPar, vPar);
    CVector3D N(O, P);
    N.Normalize();
    return N;
}

bool CSphere::IsUClosed() const
{
    if(itsFirstUParameter == 0 && itsLastUParameter == 2*PI)
	return true;
    return false;
}

bool CSphere::IsVClosed() const
{
    if(itsFirstVParameter == -PI/2 && itsLastVParameter == PI/2)
	return true;
    return false;
}

CSurface* CSphere::Copy() const
{
    CSphere* S = new CSphere(itsLocation, itsRadius, itsFirstUParameter, itsLastUParameter, itsFirstVParameter, itsLastVParameter);
    return S;
}

void CSphere::Translate(const COneAxis& Ax, const double& amt)
{
    CGeometry::Translate(Ax, amt);
}

void CSphere::Translate(double dx, double dy, double dz)
{
    itsLocation.Translate(dx,dy,dz);
}

void CSphere::Translate(const CVector3D& V)
{
    itsLocation.Translate(V);
}

void CSphere::Translate(const CPoint3D& P1, const CPoint3D& P2)
{
    itsLocation.Translate(P1, P2);
}

void CSphere::Rotate(const COneAxis& Ax, double ang)
{
    itsLocation.Rotate(Ax, ang);
}

void CSphere::Scale(const CPoint3D& P, double fact)
{
    itsLocation.Scale(P, fact);
}

void CSphere::Mirror(const CPoint3D& P)
{
    itsLocation.Mirror(P);
}

void CSphere::Mirror(const COneAxis& Ax)
{
    itsLocation.Mirror(Ax);
}

void CSphere::Mirror(const CPlane& Pln)
{
    itsLocation.Mirror(Pln);
}

