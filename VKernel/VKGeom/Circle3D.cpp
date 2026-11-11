// Circle3D.cpp: implementation of the CCircle3D class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "Circle3D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCircle3D::CCircle3D()
{
    cType = CIRCLE;
}

CCircle3D::CCircle3D(const CPoint3D& cp, const double rad) : itsRadius(rad)
{
    CAxisSystem ax(cp, CVector3D(0,0,1), CVector3D(1,0,0));
    itsLocation = ax;
    cType = CIRCLE;
}

CCircle3D::CCircle3D(const CAxisSystem& ax, const double rad) : itsRadius(rad)
{
    itsLocation = ax;
    cType = CIRCLE;
}

CCircle3D::CCircle3D(const CPoint3D& P1, const CPoint3D& P2, const CPoint3D& P3)
{
    CPoint3D p1=P1, p2=P2, p3=P3;
    double x1, x2, x3, y1, y2, y3, z1, z2, z3;
    x1 = p1.GetX(); y1 = p1.GetY(); z1 = p1.GetZ();
    x2 = p2.GetX(); y2 = p2.GetY(); z2 = p2.GetZ();
    x3 = p3.GetX(); y3 = p3.GetY(); z3 = p3.GetZ();

    CVector3D pv1(p1);
    CVector3D pv2(p2);
    CVector3D pv3(p3);

    CVector3D n1(pv1, pv2); 
    CVector3D n2(pv2, pv3);
    CVector3D n3(pv3, pv1);

    double Row1[3]={n1.GetX(), n1.GetY(), n1.GetZ()};
    double Row2[3]={n2.GetX(), n2.GetY(), n2.GetZ()};
    double Row3[3]={n3.GetX(), n3.GetY(), n3.GetZ()};

    double b1, b2, b3;
    b1 = ((p1.Distance(p2))/2) + (x1*n1.GetX() + y1*n1.GetY() + z1*n1.GetZ());
    b2 = ((p2.Distance(p3))/2) + (x2*n2.GetX() + y2*n2.GetY() + z2*n2.GetZ());
    b3 = ((p3.Distance(p1))/2) + (x3*n3.GetX() + y3*n3.GetY() + z3*n3.GetZ());

    CVector3D b(b1, b2, b3);
    CMatrix33 M(Row1, Row2, Row3);

    M.Invert();

    CVector3D cen = M*b;
    CVector3D dir = n1.Crossed(n2);
    CVector3D xdir = dir.Crossed(n1);

    CAxisSystem ax(CPoint3D(cen.GetX(), cen.GetY(), cen.GetZ()), 
	    dir, xdir);
    itsLocation = ax;

    itsRadius = p1.Distance(cen.Point());

    cType = CIRCLE;
}

CCircle3D::CCircle3D(const CCircle3D& C) : CConic()
{
    cType = CIRCLE;
    itsLocation = C.itsLocation;
    itsRadius = C.itsRadius;
}

CCircle3D::~CCircle3D()
{

}

void CCircle3D::Reverse()
{
    cReversed = !cReversed;
}

CCircle3D CCircle3D::Reversed() const
{
    CCircle3D C = (*this);
    C.Reverse();
    return C;
}

CPoint3D CCircle3D::PointAtPara(const double& Par)
{
    CPoint3D P; CVector3D V;
    double para = Par;

    if(cReversed)
	para = 2*PI - para;

    double c = cos(para);
    double s = sin(para);
    V = itsLocation.GetOrigin()+(itsLocation.GetXDirection()*c+itsLocation.GetYDirection()*s)
	*GetRadius();
    P.SetParam(V.GetX(), V.GetY(), V.GetZ());

    return P;
}

bool CCircle3D::IsClosed() const
{
    return true;
}

bool CCircle3D::operator == (const CCircle3D& C) const
{
    bool eq = (itsLocation == C.GetLocation() && itsRadius == C.GetRadius());
    return eq;
}

CCircle3D CCircle3D::operator = (const CCircle3D& C)
{
    if(this == &C)
	return *this;
    cType = CIRCLE;
    itsLocation = C.GetLocation();
    itsRadius = C.GetRadius();
    return *this;
}

void CCircle3D::Translate(double dx, double dy, double dz)
{
    itsLocation.Translate(dx,dy,dz);
}

CCurve* CCircle3D::Copy() const
{
    CCircle3D* C = new CCircle3D(itsLocation, itsRadius);
    return C;
}

void CCircle3D::Translate(const COneAxis& Ax, const double& amt)
{
    CGeometry::Translate(Ax, amt);
}

void CCircle3D::Translate(const CVector3D& V)
{
    itsLocation.Translate(V);
}

void CCircle3D::Translate(const CPoint3D& P1, const CPoint3D& P2)
{
    itsLocation.Translate(P1, P2);
}

void CCircle3D::Rotate(const COneAxis& Ax, double fact)
{
    itsLocation.Rotate(Ax, fact);
}

void CCircle3D::Scale(const CPoint3D& P, double fact)
{
    itsLocation.Scale(P, fact);
    itsRadius *= fact;
    if(itsRadius < 0)
	itsRadius = -itsRadius;
}

void CCircle3D::Mirror(const CPoint3D& P)
{
    itsLocation.Mirror(P);
}

void CCircle3D::Mirror(const COneAxis& Ax)
{
    itsLocation.Mirror(Ax);
}

void CCircle3D::Mirror(const CPlane& Pln)
{
    itsLocation.Mirror(Pln);
}
