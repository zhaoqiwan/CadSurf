// Point2D.cpp: implementation of the CPoint2D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Point2D.h"
#include "Vector2D.h"
#include "OneAxis2D.h"
#include "Line2D.h"
#include "Matrix22.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPoint2D::CPoint2D()
{
    SetParam(0,0);
    geomType = CPOINT2D;
}

CPoint2D::CPoint2D(double x, double y)
{
    SetParam(x, y);
    geomType = CPOINT2D;
}

CPoint2D::~CPoint2D()
{

}

void CPoint2D::SetParam(double x, double y)
{
    itsX=x;
    itsY=y;
}

CPoint2D CPoint2D::operator+(const CPoint2D &newPoint)
{
    return CPoint2D(itsX+newPoint.GetX(),itsY+newPoint.GetY());
}

CPoint2D CPoint2D::operator+(const double & inc)
{
    return CPoint2D(itsX+inc,itsY+inc);
}

CPoint2D CPoint2D::operator-(const CPoint2D &newPoint)
{
    return CPoint2D(itsX-newPoint.GetX(),itsY-newPoint.GetY());
}

CPoint2D CPoint2D::operator-(const double & dec)
{
    return CPoint2D(itsX-dec,itsY-dec);
}

CPoint2D CPoint2D::operator*(const double scale)
{
    return CPoint2D(itsX*scale,itsY*scale);
}

CPoint2D CPoint2D::operator/(const double scale)
{
    return CPoint2D(itsX/scale,itsY/scale);
}

void CPoint2D::operator+=(const CPoint2D& P)
{
    (*this) = (*this) + P;
}

void CPoint2D::operator+=(const double& inc)
{
    (*this) = (*this) + inc;
}

void CPoint2D::operator-=(const CPoint2D& P)
{
    (*this) = (*this) - P;
}

void CPoint2D::operator-=(const double& dec)
{
    (*this) = (*this) - dec;
}

void CPoint2D::operator*=(const double scale)
{
    (*this) = (*this) * scale;
}

void CPoint2D::operator/=(const double scale)
{
    (*this) = (*this) / scale;
}

CPoint2D CPoint2D::operator = (const CPoint2D& p)
{
    if(this == &p)
	return *this;
    itsX = p.itsX;
    itsY = p.itsY;
    return *this;
}

bool CPoint2D::operator == (const CPoint2D& p) const
{
    return (itsX==p.GetX() && itsY==p.GetY());
}

ORIENTATION CPoint2D::Orientation(const CPoint2D& P1, const CPoint2D& P2)
{
    CPoint2D tmp1 = P1, tmp2 = P2;
    CPoint2D a = tmp2 - *this;
    CPoint2D b = tmp1 - *this;
    double sa = a.GetX() * b.GetY() - b.GetX() * a.GetY();
    if(sa > 0.0)
	return ORIENTED_CCW;
    if(sa < 0.0)
	return ORIENTED_CW;
    
    return ORIENTED_COLLINEAR;
}

CLASSIFICATION CPoint2D::Classification(const CPoint2D& P1, const CPoint2D& P2)
{
    CPoint2D p0 = P1, p1 = P2;
    CPoint2D p2 = *this;
    CPoint2D a = p1 - p0;
    CPoint2D b = p2 - p0;
    double sa = a.GetX() * b.GetY() - b.GetX() * a.GetY();
    if(sa > 0.0)
	return CLASSIFIED_LEFT;
    if(sa < 0.0)
	return CLASSIFIED_RIGHT;
    if((a.GetX() * b.GetX() < 0.0) || (a.GetY() * b.GetY() < 0.0))
	return CLASSIFIED_BEHIND;
    if(a.Distance(CPoint2D(0,0)) < b.Distance(CPoint2D(0,0)))
	return CLASSIFIED_BEYOND;
    if(p0 == p2)
	return CLASSIFIED_ORIGIN;
    if(p1 == p2)
	return CLASSIFIED_DESTINATION;
    return CLASSIFIED_BETWEEN;
}

CPoint2D::operator CVector2D()
{
    CVector2D V((*this));
    return V;
}


double CPoint2D::Distance(const CPoint2D& P)
{

    double sqad, root;
    double x,y,tx,ty;
    x=P.GetX(); y=P.GetY();
    tx = fabs(this->GetX()-x);
    ty = fabs(this->GetY()-y);
    sqad=((pow(tx,2))+(pow(ty,2)));
    root=sqrt(sqad);

    return root;
}

double CPoint2D::PolarAngle(const CPoint2D& P) const
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

void CPoint2D::Translate(const COneAxis2D& Ax, const double& amt)
{
    CGeometry2D::Translate(Ax, amt);
}

void CPoint2D::Translate(double dx, double dy)
{
    itsX+=dx;
    itsY+=dy;
}

void CPoint2D::Translate(const CVector2D& VDir)
{
    CVector2D V = VDir;
    itsX+=V.GetX();
    itsY+=V.GetY();
}

void CPoint2D::Translate(const CPoint2D& P1, const CPoint2D& P2)
{
    CVector2D V(P1, P2);
    Translate(V);
}

void CPoint2D::Rotate(const CPoint2D& Pt, double ang)
{
    CPoint2D P1 = Pt;
    CMatrix22 M;
    M.SetRotation(ang);
    CPoint2D tp = *this - P1;
    CVector2D V = M * CVector2D(tp);
    V += CVector2D(P1);
    *this = V.Point();
}

void CPoint2D::Scale(const CPoint2D& P, double fact)
{	
    CPoint2D p = P;
    p = p * (1.0 - fact);
    (*this) = (*this)*(fact);
    (*this) = (*this) + p;
}

void CPoint2D::Mirror(const CPoint2D& P)
{
    CPoint2D p = P;
    (*this) *= -1;
    p *= (2.0);
    (*this) += p;
}

void CPoint2D::Mirror(const COneAxis2D& Ax)
{
    CPoint2D P1 = Ax.GetPosition();
    CPoint2D P = (*this);
    CVector2D N = Ax.GetDirection();
    CLine2D L(P1, N);
    double D = L.Distance(P);
    CVector2D Q = L.NormalThrough(P) * D * (-2.0);
    CVector2D PV((*this));
    CVector2D R = PV + Q;
    this->SetParam(R.GetX(), R.GetY());
}

CPoint2D CPoint2D::Origin()
{
    return  CPoint2D(0,0);
}

std::istream &operator>>(std::istream& input, CPoint2D &p)
{
    input>>p.itsX>>p.itsY;
    return input;
}

std::ostream &operator<<(std::ostream& output, CPoint2D &p)
{
    output<<p.itsX<<p.itsY;
    return output;
}
