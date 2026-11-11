// Point3D.cpp: implementation of the CPoint3D class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "Point3D.h"
#include "Vector3D.h"
#include "OneAxis.h"
#include "Line3D.h"
#include "Plane.h"
#include "Matrix33.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPoint3D::CPoint3D()
{
    SetParam(0,0,0);
    geomType = CPOINT;
}

CPoint3D::CPoint3D(double x, double y, double z)
{
    SetParam(x, y, z);
    geomType = CPOINT;
}

CPoint3D::~CPoint3D()
{

}

void CPoint3D::SetParam(double x, double y, double z)
{
    itsX=x;
    itsY=y;
    itsZ=z;
}

CPoint3D CPoint3D::operator+(const CPoint3D &newPoint)
{
    return CPoint3D(itsX+newPoint.GetX(),itsY+newPoint.GetY(),itsZ+newPoint.GetZ());
}

CPoint3D CPoint3D::operator+(const double & inc)
{
    return CPoint3D(itsX+inc,itsY+inc,itsZ+inc);
}

CPoint3D CPoint3D::operator-(const CPoint3D &newPoint)
{
    return CPoint3D(itsX-newPoint.GetX(),itsY-newPoint.GetY(),itsZ-newPoint.GetZ());
}

CPoint3D CPoint3D::operator-(const double & dec)
{
    return CPoint3D(itsX-dec,itsY-dec,itsZ-dec);
}

CPoint3D CPoint3D::operator*(const double scale)
{
    return CPoint3D(itsX*scale,itsY*scale,itsZ*scale);
}

CPoint3D CPoint3D::operator/(const double scale)
{
    return CPoint3D(itsX/scale,itsY/scale,itsZ/scale);
}

void CPoint3D::operator+=(const CPoint3D& P)
{
    (*this) = (*this) + P;
}

void CPoint3D::operator+=(const double& inc)
{
    (*this) = (*this) + inc;
}

void CPoint3D::operator-=(const CPoint3D& P)
{
    (*this) = (*this) - P;
}

void CPoint3D::operator-=(const double& dec)
{
    (*this) = (*this) - dec;
}

void CPoint3D::operator*=(const double scale)
{
    (*this) = (*this) * scale;
}

void CPoint3D::operator/=(const double scale)
{
    (*this) = (*this) / scale;
}

CPoint3D CPoint3D::operator = (const CPoint3D& p)
{
    if(this == &p)
	return *this;
    itsX = p.itsX;
    itsY = p.itsY;
    itsZ = p.itsZ;
    return *this;
}

bool CPoint3D::operator == (const CPoint3D& p) const
{
    return (itsX==p.GetX() && itsY==p.GetY() && itsZ==p.GetZ());
}

CPoint3D::operator CVector3D()
{
    CVector3D V((*this));
    return V;
}

double CPoint3D::Distance(const CPoint3D& P) const
{

    double sqad, root;
    double x,y,z,tx,ty,tz;
    x=P.GetX(); y=P.GetY(); z=P.GetZ();
    tx = fabs(this->GetX()-x);
    ty = fabs(this->GetY()-y);
    tz = fabs(this->GetZ()-z);
    sqad=((pow(tx,2))+(pow(ty,2))+(pow(tz,2)));
    root=sqrt(sqad);

    return root;
}

double CPoint3D::Distance(const CPlane& P) const
{

    CPoint3D p = P.GetPosition();
    CPoint3D dp = (*this);
    CVector3D V1 = P.GetDirection();
    CVector3D V2(dp);
    double D = V1.Dot(V2) + P.GetConstant();
    return D;
}

double CPoint3D::PolarAngle(const CPoint3D& P) const
{
    double x = itsX - P.GetX();
    double y = itsY - P.GetY();
    if((x == 0.0) && (y == 0.0))
	return -1;
    if(x == 0.0)
	return ((y > 0.0) ? 90 : 270);
    double theta = atan(y / x);
    theta *= 360.0 / (2 * PI);
    if(x > 0.0)
	return ((y >= 0.0) ? theta : 360 + theta);
    else
	return (180.0 + theta);
}

void CPoint3D::Translate(const COneAxis& Ax, const double& amt)
{
    CGeometry::Translate(Ax, amt);
}

void CPoint3D::Translate(double dx, double dy, double dz)
{
    itsX+=dx;
    itsY+=dy;
    itsZ+=dz;
}

void CPoint3D::Translate(const CVector3D& VDir)
{
    CVector3D V = VDir;
    itsX+=V.GetX();
    itsY+=V.GetY();
    itsZ+=V.GetZ();
}

void CPoint3D::Translate(const CPoint3D& P1, const CPoint3D& P2)
{
    CVector3D V(P1, P2);
    Translate(V);
}

void CPoint3D::Rotate(const COneAxis& Ax, double ang)
{
    COneAxis ax = Ax;
    CVector3D P1 = ax.GetOrigin();
    CVector3D V = ax.GetDirection();
    V.Normalize();
    CMatrix33 M; CVector3D PV(*this);
    M.SetRotation(V, ang);
    CVector3D RV = M*(PV-P1);
    RV = RV + P1;
    *this = RV.Point();
}

void CPoint3D::Scale(const CPoint3D& P, double fact)
{	
    CPoint3D p = P;
    p = p * (1.0 - fact);
    (*this) = (*this)*(fact);
    (*this) = (*this) + p;
}

void CPoint3D::Mirror(const CPoint3D& P)
{
    CPoint3D p = P;
    (*this) *= -1;
    p *= (2.0);
    (*this) += p;
}

void CPoint3D::Mirror(const COneAxis& Ax)
{
    CPoint3D P1 = Ax.GetPosition();
    CPoint3D P = (*this);
    CVector3D N = Ax.GetDirection();
    CLine3D L(P1, N);
    double D = L.Distance(P);
    CVector3D Q = L.NormalThrough(P) * D * (-2.0);
    CVector3D PV((*this));
    CVector3D R = PV + Q;
    this->SetParam(R.GetX(), R.GetY(), R.GetZ());
}

void CPoint3D::Mirror(const CPlane& Pln)
{
    CPlane pln = Pln;
    CPoint3D P = (*this);
    double D = P.Distance(pln);
    CVector3D N = pln.GetDirection().Unit();
    CVector3D Q = N * D * (-2.0);
    CPoint3D QP = Q.Point();
    (*this) = P + QP;
}

CPoint3D CPoint3D::Origin()
{
    return  CPoint3D(0,0,0);
}

std::istream &operator>>(std::istream& input, CPoint3D &p)
{
    input>>p.itsX>>p.itsY>>p.itsZ;
    return input;
}

std::ostream &operator<<(std::ostream& output, CPoint3D &p)
{
    output<< p.itsX << " " << p.itsY << " " << p.itsZ;
    return output;
}
