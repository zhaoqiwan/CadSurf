// Cone.cpp: implementation of the CCone class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Cone.h"
#include "Circle3D.h"
#include "MMath.h"
#include "GeomException.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCone::CCone(const CCircle3D& C, const double& ht) : itsHeight(ht)
{
    sType = CONE;
    itsLocation = C.GetLocation();
    baseRadius = C.GetRadius();
    topRadius = 0;
    itsFirstUParameter = 0;
    itsLastUParameter = 2*PI;
    itsFirstVParameter = 0;
    itsLastVParameter = itsHeight;
}

CCone::CCone(const CAxisSystem& Ax, const double& ht, const double& rad1, const double& rad2, const double& uStart, const double& uEnd) : itsHeight(ht), baseRadius(rad1), topRadius(rad2)
{
    sType = CONE;
    itsLocation = Ax;
    itsFirstUParameter = uStart;
    itsLastUParameter = uEnd;
    itsFirstVParameter = 0;
    itsLastVParameter = itsHeight;
    if(itsFirstUParameter < 0 || itsFirstUParameter > 2*PI)
    {
	cout << "Invalid Range .... CCone::CCone(const CAxisSystem& Ax, const double& ht, const double& rad1, const double& rad2, const double& uStart, const double& uEnd)\n";
	throw CGeomException(InvalidRange);
    }
    if(itsLastUParameter < 0 || itsLastUParameter > 2*PI)
    {
	cout << "Invalid Range .... CCone::CCone(const CAxisSystem& Ax, const double& ht, const double& rad1, const double& rad2, const double& uStart, const double& uEnd)\n";
	throw CGeomException(InvalidRange);
    }
    if(itsLastUParameter <  itsFirstUParameter)
    {
	cout << "Invalid Range .... CCone::CCone(const CAxisSystem& Ax, const double& ht, const double& rad1, const double& rad2, const double& uStart, const double& uEnd)\n";
	throw CGeomException(InvalidRange);
    }
}

CCone::~CCone()
{

}

CPoint3D CCone::PointAtPara(const double uPar, const double vPar)
{
    CVector3D P, O, XDir, YDir, ZDir;
    double Ang = atan((baseRadius-topRadius)/itsHeight);
    O = itsLocation.GetOrigin();
    ZDir = itsLocation.GetDirection();
    XDir = itsLocation.GetXDirection();
    YDir = itsLocation.GetYDirection();
    double cu = cos(uPar);
    double su = sin(uPar);
    P = O + (((XDir*cu + YDir*su) * (baseRadius - vPar*tan(Ang))) ) + ZDir*vPar;
    return P.Point();
}

bool CCone::IsUClosed() const
{
    if(itsFirstUParameter == 0 && itsLastUParameter == 2*PI)
	return true;
    return false;
}

bool CCone::IsVClosed() const
{
    return false;
}

CSurface* CCone::Copy() const
{
    CCone* R = new CCone(itsLocation, itsHeight, baseRadius, topRadius, itsFirstUParameter, itsLastUParameter);;
    return R;
}

void CCone::Translate(const COneAxis& Ax, const double& amt)
{
    CGeometry::Translate(Ax, amt);
}

void CCone::Translate(double dx, double dy, double dz)
{
    itsLocation.Translate(dx,dy,dz);
}

void CCone::Translate(const CVector3D& V)
{
    itsLocation.Translate(V);
}

void CCone::Translate(const CPoint3D& P1, const CPoint3D& P2)
{
    itsLocation.Translate(P1, P2);
}

void CCone::Rotate(const COneAxis& Ax, double ang)
{
    itsLocation.Rotate(Ax, ang);
}

void CCone::Scale(const CPoint3D& P, double fact)
{
    itsLocation.Scale(P, fact);
}

void CCone::Mirror(const CPoint3D& P)
{
    itsLocation.Mirror(P);
}

void CCone::Mirror(const COneAxis& Ax)
{
    itsLocation.Mirror(Ax);
}

void CCone::Mirror(const CPlane& Pln)
{
    itsLocation.Mirror(Pln);
}
