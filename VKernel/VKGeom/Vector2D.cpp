// Vector2D.cpp: implementation of the CVector2D class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "Vector2D.h"
#include "Point2D.h"
#include "Matrix22.h"
#include "OneAxis2D.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//Implementation of class vector
CVector2D::CVector2D()
{
    itsX = 0;
    itsY = 0;
}

CVector2D::CVector2D(double x, double y)
{
    itsX = x;
    itsY = y;
}

CVector2D::CVector2D(const CPoint2D& P)
{
    itsX = P.GetX();
    itsY = P.GetY();
}

CVector2D::CVector2D(const CVector2D &V1, const CVector2D &V2)
{
    CVector2D VC1, VC2, VC3;
    VC1 = V1;
    VC2 = V2;
    VC3 = VC2-VC1;
    itsX = VC3.GetX();
    itsY = VC3.GetY();
}

CVector2D::CVector2D(const CPoint2D &V1, const CPoint2D &V2)
{
    CPoint2D VC1, VC2;
    VC1 = V1;
    VC2 = V2;
    itsX = VC2.GetX()-VC1.GetX();
    itsY = VC2.GetY()-VC1.GetY();
}

CVector2D::~CVector2D()
{
}

double CVector2D::Dot(const CVector2D &V1) const
{
    double result;
    result = V1.GetX()*GetX()+V1.GetY()*GetY();
    return result;
}

double CVector2D::DotCross(const CVector2D &V1, const CVector2D &V2)
{
    CVector2D A = (*this), B = V1, C = V2;
    CVector2D cross = B.Crossed(C);
    double Res = A.Dot(cross);
    return Res;
}

void CVector2D::Cross(const CVector2D& V1)
{
    SetX(GetY()-V1.GetY());
    SetY(V1.GetX()-GetX());
}

CVector2D CVector2D::Crossed(const CVector2D& V1) const
{
    CVector2D result;
    result.SetX(GetY()-V1.GetY());
    result.SetY(V1.GetX()-GetX());
    return result;
}

void CVector2D::CrossCross(const CVector2D &V1, const CVector2D &V2)
{
    CVector2D A = (*this), B = V1, C = V2;
    CVector2D Res = B*((C.Dot(A))) - C*(A.Dot(B));
    (*this) = Res;
}

CVector2D CVector2D::CrossCrossed(const CVector2D &V1, const CVector2D &V2)
{
    CVector2D V = (*this);
    V.CrossCross(V1, V2);
    return V;
}

void CVector2D::Reverse()
{
    this->SetX(-GetX());
    this->SetY(-GetY());
}

CVector2D CVector2D::Reversed()
{
    CVector2D result;
    result.Reverse();
    return result;
}

CVector2D CVector2D::operator + (const CVector2D &V1)
{
    CVector2D result;
    result.SetX(GetX()+V1.GetX());
    result.SetY(GetY()+V1.GetY());
    return result;
}

void CVector2D::operator += (const CVector2D &V1)
{
    SetX(GetX()+V1.GetX());
    SetY(GetY()+V1.GetY());
}

CVector2D CVector2D::operator - (const CVector2D &V1)
{
    CVector2D result;
    result.SetX(GetX()-V1.GetX());
    result.SetY(GetY()-V1.GetY());
    return result;
}

void CVector2D::operator -= (const CVector2D &V1)
{
    SetX(GetX()-V1.GetX());
    SetY(GetY()-V1.GetY());
}

CVector2D CVector2D::operator * (const double &scalar)
{
    CVector2D result;
    result.SetX(GetX()*scalar);
    result.SetY(GetY()*scalar);
    return result;
}

CVector2D CVector2D::operator * (const CMatrix22 &M)
{
   CVector2D V;
   V.SetX(M(0,0)*GetX()+M(0,1)*GetY());
   V.SetY(M(1,0)*GetX()+M(1,1)*GetY());
   return V;
}


void CVector2D::operator *= (const double &scalar)
{
    SetX(GetX()*scalar);
    SetY(GetY()*scalar);
}


void CVector2D::operator *= (const CMatrix22 &M)
{
   SetX(M(0,0)*GetX()+M(0,1)*GetY());
   SetY(M(1,0)*GetX()+M(1,1)*GetY());
}

CVector2D CVector2D::operator / (const double &scalar)
{
    CVector2D result;
    if(!IsNull())
    {
	result.SetX(GetX()/scalar);
	result.SetY(GetY()/scalar);
    }
    else
    {
	result.SetX(0);
	result.SetY(0);
    }
    return result;
}

void CVector2D::operator /= (const double &scalar)
{
    if(!IsNull())
    {
	SetX(GetX()/scalar);
	SetY(GetY()/scalar);
    }
    else
    {
	SetX(0);
	SetY(0);
    }
}

CVector2D CVector2D::operator ^ (const CVector2D &V1)
{
    return this->Crossed(V1);
}

void CVector2D::operator ^= (const CVector2D &V1)
{
    this->Cross(V1);
}

bool CVector2D::operator == (const CVector2D& V) const
{
    return(itsX==V.GetX()&&itsY==V.GetY());
}

CVector2D::operator CPoint2D()
{
    CPoint2D P(itsX, itsY);
    return P;
}

double CVector2D::Magnitude() const
{
    if(!IsNull())
    {
	double result;
	result = sqrt(pow((double)GetX(), 2)+pow((double)GetY(), 2));
	return result;
    }
    else
    {
	return 0;
    }
}

double CVector2D::SqrMagnitude() const
{
    if(!IsNull())
    {
	double result;
	result = pow((double)GetX(), 2)+pow((double)GetY(), 2);
	return result;
    }
    else
	return 0;
}

double CVector2D::CrossMagnitude(const CVector2D &V)
{
    if(!IsNull() && !V.IsNull())
    {
	CVector2D V1 = (*this);
	CVector2D V2 = V;
	V1.Cross(V2);
	double result;
	result = sqrt(pow((double)V1.GetX(), 2)+pow((double)V1.GetY(), 2));
	return result;
    }
    else
    {
	return 0;
    }
}

double CVector2D::CrossSqrMagnitude(const CVector2D &V)
{
    if(!IsNull() && !V.IsNull())
    {
	CVector2D V1 = (*this);
	CVector2D V2 = V;
	V1.Cross(V2);
	double result;
	result = pow((double)V1.GetX(), 2)+pow((double)V1.GetY(), 2);
	return result;
    }
    else
    {
	return 0;
    }
}

double CVector2D::Modulus()
{
    return sqrt(itsX*itsX + itsY*itsY);
}

CVector2D CVector2D::Unit()
{
    CVector2D result;
    double x, y;
    if(!IsNull())
    {
	x = GetX()/Magnitude();
	y = GetY()/Magnitude();
	result.SetX(x);
	result.SetY(y);
	return result;
    }
    else
	return *this;
}

CPoint2D CVector2D::Point()
{
    CPoint2D aPnt;
    aPnt.SetParam(GetX(), GetY());
    return aPnt;
}

double CVector2D::Angle(const CVector2D& Other) const
{
    double theta, a, b, dot;
    CVector2D tempV = Other;
    if(!this->IsNull() && !tempV.IsNull())
    {
	dot = this->Dot(Other);
	a = this->Magnitude();
	b = tempV.Magnitude();
	theta = acos(dot/(a*b));
	return theta;
    }
    else
	return 0;
}

double CVector2D::Angle(const CVector2D& Other, const CVector2D& Dir) const
{
    double theta;
    CVector2D tempV = Other;
    CVector2D D = Dir;
    CVector2D N = this->Crossed(tempV);
    if(!this->IsNull() && !tempV.IsNull())
    {
	theta = Angle(Other);

	if(IsOpposite(tempV))
	{
	    return PI;
	}
	else
	{
	    if(N.Angle(D)<=0.0001)

		return theta;
	    else
		return -theta;
	}
    }
    else
	return 0;
}

bool CVector2D::IsNull() const
{
    if(itsX==0&&itsY==0)
	return true;
    return false;
}

bool CVector2D::IsParallel(const CVector2D& V) const
{
    /*double ang=0;
      CVector2D tempV = V;
      ang = this->Angle(tempV);
      if(ang==0 || ang==(double)2*PI)
      return true;
      else
      return false;*/
    CVector2D N = this->Crossed(V);
    return (N.IsNull());
}

bool CVector2D::IsOpposite(const CVector2D& V) const

{
    double ang=0;
    CVector2D tempV = V;
    ang = this->Angle(tempV);
    if(fabs(ang-PI)<=0.0001)

	return true;
    else
	return false;
}

bool CVector2D::IsNormal(const CVector2D& V) const
{
    double ang=0;
    CVector2D tempV = V;
    ang = this->Angle(tempV);
    if(fabs(ang-PI/2)<=0.0001)
	return true;
    else
	return false;
}

void CVector2D::Print() const
{
    cout<<"X = "<<GetX()<<endl;
    cout<<"Y = "<<GetY()<<endl;
}

void CVector2D::Normalize()
{
    double x, y;
    if(!IsNull())
    {
	x = GetX()/Magnitude();
	y = GetY()/Magnitude();
	SetX(x);
	SetY(y);
    }
    else
	return;
}

void CVector2D::Translate(const COneAxis2D& Ax, const double& amt)
{
    CGeometry2D::Translate(Ax, amt);
}

void CVector2D::Translate(double dx, double dy)
{
    CPoint2D P = (*this);
    P.Translate(dx, dy);
    CVector2D V = P;
    (*this) = V;

}

void CVector2D::Translate(const CVector2D& dV)
{
    CPoint2D P = (*this);
    P.Translate(dV);
    CVector2D V = P;
    (*this) = V;
}

void CVector2D::Translate(const CPoint2D& P1, const CPoint2D& P2)
{
    CPoint2D P = (*this);
    P.Translate(P1, P2);
    CVector2D V = P;
    (*this) = V;
}

void CVector2D::Rotate(const CPoint2D& Pt, double ang)
{
    CPoint2D P1 = Pt;
    CMatrix22 M;
    M.SetRotation(ang);
    CPoint2D tp = *this - (CVector2D)P1;
    CVector2D V = M * (CVector2D)tp;
    V += (CVector2D)P1;
    *this = V;
}

void CVector2D::Scale(const CPoint2D& P1, double fact)
{
    CPoint2D P = (*this);
    P.Scale(P1, fact);
    CVector2D V = P;
    (*this) = V;
}

void CVector2D::Mirror(const CPoint2D& P1)
{
    CPoint2D P = (*this);
    P.Mirror(P1);
    CVector2D V = P;
    (*this) = V;
}

void CVector2D::Mirror(const COneAxis2D& Ax)
{
    CPoint2D P = (*this);
    P.Mirror(Ax);
    CVector2D V = P;
    (*this) = V;
}

CVector2D CVector2D::Origin()
{
    return CVector2D(0,0);
}
