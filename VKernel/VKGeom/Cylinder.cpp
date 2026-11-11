// Cylinder.cpp: implementation of the CCylinder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Cylinder.h"
#include "Circle3D.h"
#include "AxisSystem.h"
#include "MMath.h"
#include "GeomException.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCylinder::CCylinder(const CCircle3D& C, const double& ht) : itsHeight(ht)
{
    sType = CYLINDER;
    itsLocation = C.GetLocation();
    itsRadius = C.GetRadius();
    itsFirstUParameter = 0;
    itsLastUParameter = 2*PI;
    itsFirstVParameter = 0;
    itsLastVParameter = itsHeight;
}

CCylinder::CCylinder(const CAxisSystem& Ax, const double& ht, const double& rad, const double& uStart, const double& uEnd) : itsHeight(ht), itsRadius(rad)
{
    itsLocation = Ax;
    itsFirstUParameter = uStart;
    itsLastUParameter = uEnd;
    itsFirstVParameter = 0;
    itsLastVParameter = itsHeight;
    if(itsFirstUParameter < 0 || itsFirstUParameter > 2*PI)
    {
	cout << "Invalid Range .... CCylinder::CCylinder(const CAxisSystem& Ax, const double& ht, const double& rad, const double& uStart, const double& uEnd)\n";
	throw CGeomException(InvalidRange);
    }
    if(itsLastUParameter < 0 || itsLastUParameter > 2*PI)
    {
	cout << "Invalid Range .... CCylinder::CCylinder(const CAxisSystem& Ax, const double& ht, const double& rad, const double& uStart, const double& uEnd)\n";
	throw CGeomException(InvalidRange);
    }
    if(itsLastUParameter <  itsFirstUParameter)
    {
	cout << "Invalid Range .... CCylinder::CCylinder(const CAxisSystem& Ax, const double& ht, const double& rad, const double& uStart, const double& uEnd)\n";
	throw CGeomException(InvalidRange);
    }
}

CCylinder::~CCylinder()
{

}

CPoint3D CCylinder::PointAtPara(const double uPar, const double vPar)
{
    CVector3D P, O, XDir, YDir, ZDir;
    O = itsLocation.GetOrigin();
    ZDir = itsLocation.GetDirection();
    XDir = itsLocation.GetXDirection();
    YDir = itsLocation.GetYDirection();
    double cu = cos(uPar);
    double su = sin(uPar);
    P = O + (XDir*cu + YDir*su)*itsRadius + ZDir*vPar;
    return P.Point();
}

bool CCylinder::IsUClosed() const
{
    if(itsFirstUParameter == 0 && itsLastUParameter == 2*PI)
	return true;
    return false;
}

bool CCylinder::IsVClosed() const
{
    return false;
}

CSurface* CCylinder::Copy() const
{
    CCylinder* R = new CCylinder(itsLocation, itsHeight, itsRadius, itsFirstUParameter, itsLastUParameter);
    return R;
}

void CCylinder::Translate(const COneAxis& Ax, const double& amt)
{
    CGeometry::Translate(Ax, amt);
}

void CCylinder::Translate(double dx, double dy, double dz)
{
    itsLocation.Translate(dx,dy,dz);
}

void CCylinder::Translate(const CVector3D& V)
{
    itsLocation.Translate(V);
}

void CCylinder::Translate(const CPoint3D& P1, const CPoint3D& P2)
{
    itsLocation.Translate(P1, P2);
}

void CCylinder::Rotate(const COneAxis& Ax, double ang)
{
    itsLocation.Rotate(Ax, ang);
}

void CCylinder::Scale(const CPoint3D& P, double fact)
{
    itsLocation.Scale(P, fact);
}

void CCylinder::Mirror(const CPoint3D& P)
{
    itsLocation.Mirror(P);
}

void CCylinder::Mirror(const COneAxis& Ax)
{
    itsLocation.Mirror(Ax);
}

void CCylinder::Mirror(const CPlane& Pln)
{
    itsLocation.Mirror(Pln);
}
