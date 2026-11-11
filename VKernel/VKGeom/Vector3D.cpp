// Vector3d.cpp: implementation of the CVector3d class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "Vector3D.h"
#include "Point3D.h"
#include "Matrix33.h"
#include "OneAxis.h"
#include "Plane.h"
#include "MMath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//Implementation of class vector
CVector3D::CVector3D()
{
	itsX = 0;
	itsY = 0;
	itsZ = 0;
}

CVector3D::CVector3D(double x, double y, double z)
{
	itsX = x;
	itsY = y;
	itsZ = z;
}

CVector3D::CVector3D(const CPoint3D& P)
{
	itsX = P.GetX();
	itsY = P.GetY();
	itsZ = P.GetZ();
}

CVector3D::CVector3D(const CVector3D &V1, const CVector3D &V2)
{
	CVector3D VC1, VC2, VC3;
	VC1 = V1;
	VC2 = V2;
	VC3 = VC2-VC1;
	itsX = VC3.GetX();
	itsY = VC3.GetY();
	itsZ = VC3.GetZ();
}

CVector3D::CVector3D(const CPoint3D &V1, const CPoint3D &V2)
{
	CPoint3D VC1, VC2;
	VC1 = V1;
	VC2 = V2;
	itsX = VC2.GetX()-VC1.GetX();
	itsY = VC2.GetY()-VC1.GetY();
	itsZ = VC2.GetZ()-VC1.GetZ();
}

CVector3D::~CVector3D()
{
}

double CVector3D::Dot(const CVector3D &V1) const
{
	double result;
	result = V1.GetX()*GetX()+V1.GetY()*GetY()+V1.GetZ()*GetZ();
	return result;
}

double CVector3D::DotCross(const CVector3D &V1, const CVector3D &V2)
{
	CVector3D A = (*this), B = V1, C = V2;
	CVector3D cross = B.Crossed(C);
	double Res = A.Dot(cross);
	return Res;
}

void CVector3D::Cross(const CVector3D &V1)
{
	SetX(GetY()*V1.GetZ()-GetZ()*V1.GetY());
	SetY(GetZ()*V1.GetX()-GetX()*V1.GetZ());
	SetZ(GetX()*V1.GetY()-GetY()*V1.GetX());
}

CVector3D CVector3D::Crossed(const CVector3D &V1) const
{
	CVector3D result;
	result.SetX(GetY()*V1.GetZ()-GetZ()*V1.GetY());
	result.SetY(GetZ()*V1.GetX()-GetX()*V1.GetZ());
	result.SetZ(GetX()*V1.GetY()-GetY()*V1.GetX());
	return result;
}

void CVector3D::CrossCross(const CVector3D &V1, const CVector3D &V2)
{
	CVector3D A = (*this), B = V1, C = V2;
	CVector3D Res = B*((C.Dot(A))) - C*(A.Dot(B));
	(*this) = Res;
}

CVector3D CVector3D::CrossCrossed(const CVector3D &V1, const CVector3D &V2)
{
	CVector3D V = (*this);
	V.CrossCross(V1, V2);
	return V;
}

void CVector3D::Reverse()
{
	this->SetX(-GetX());
	this->SetY(-GetY());
	this->SetZ(-GetZ());
}

CVector3D CVector3D::Reversed()
{
	CVector3D result;
	result.Reverse();
	return result;
}

CVector3D CVector3D::operator + (const CVector3D &V1)
{
	CVector3D result;
	result.SetX(GetX()+V1.GetX());
	result.SetY(GetY()+V1.GetY());
	result.SetZ(GetZ()+V1.GetZ());
	return result;
}

void CVector3D::operator += (const CVector3D &V1)
{
	SetX(GetX()+V1.GetX());
	SetY(GetY()+V1.GetY());
	SetZ(GetZ()+V1.GetZ());
}

CVector3D CVector3D::operator - (const CVector3D &V1)
{
	CVector3D result;
	result.SetX(GetX()-V1.GetX());
	result.SetY(GetY()-V1.GetY());
	result.SetZ(GetZ()-V1.GetZ());
	return result;
}

void CVector3D::operator -= (const CVector3D &V1)
{
	SetX(GetX()-V1.GetX());
	SetY(GetY()-V1.GetY());
	SetZ(GetZ()-V1.GetZ());
}

CVector3D CVector3D::operator * (const double &scalar)
{
	CVector3D result;
	result.SetX(GetX()*scalar);
	result.SetY(GetY()*scalar);
	result.SetZ(GetZ()*scalar);
	return result;
}

CVector3D CVector3D::operator * (const CMatrix33 &M)
{
	CVector3D V;
	V.SetX(M(0,0)*GetX()+M(0,1)*GetY()+M(0,2)*GetZ());
	V.SetY(M(1,0)*GetX()+M(1,1)*GetY()+M(1,2)*GetZ());
	V.SetZ(M(2,0)*GetX()+M(2,1)*GetY()+M(2,2)*GetZ());
	return V;
}

void CVector3D::operator *= (const double &scalar)
{
	SetX(GetX()*scalar);
	SetY(GetY()*scalar);
	SetZ(GetZ()*scalar);
}

void CVector3D::operator *= (const CMatrix33 &M)
{
	SetX(M(0,0)*GetX()+M(0,1)*GetY()+M(0,2)*GetZ());
	SetY(M(1,0)*GetX()+M(1,1)*GetY()+M(1,2)*GetZ());
	SetZ(M(2,0)*GetX()+M(2,1)*GetY()+M(2,2)*GetZ());
}

CVector3D CVector3D::operator / (const double &scalar)
{
	CVector3D result;
	if(!IsNull())
	{
		result.SetX(GetX()/scalar);
		result.SetY(GetY()/scalar);
		result.SetZ(GetZ()/scalar);
	}
	else
	{
		result.SetX(0);
		result.SetY(0);
		result.SetZ(0);
	}
	return result;
}

void CVector3D::operator /= (const double &scalar)
{
	if(!IsNull())
	{
		SetX(GetX()/scalar);
		SetY(GetY()/scalar);
		SetZ(GetZ()/scalar);
	}
	else
	{
		SetX(0);
		SetY(0);
		SetZ(0);
	}
}

CVector3D CVector3D::operator ^ (const CVector3D &V1)
{
	return this->Crossed(V1);
}

void CVector3D::operator ^= (const CVector3D &V1)
{
	this->Cross(V1);
}

bool CVector3D::operator == (const CVector3D& V) const
{
	return(itsX==V.GetX()&&itsY==V.GetY()&&itsZ==V.GetZ());
}

CVector3D::operator CPoint3D()
{
	CPoint3D P(itsX, itsY, itsZ);
	return P;
}

double CVector3D::Magnitude() const
{
	if(!IsNull())
	{
		double result;
		result = sqrt(pow((double)GetX(), 2)+pow((double)GetY(), 2)+pow((double)GetZ(), 2));
		return result;
	}
	else
	{
		return 0;
	}
}

double CVector3D::SqrMagnitude() const
{
	if(!IsNull())
	{
		double result;
		result = pow((double)GetX(), 2)+pow((double)GetY(), 2)+pow((double)GetZ(), 2);
		return result;
	}
	else
		return 0;
}

double CVector3D::CrossMagnitude(const CVector3D &V)
{
	if(!IsNull() && !V.IsNull())
	{
		CVector3D V1 = (*this);
		CVector3D V2 = V;
		V1.Cross(V2);
		double result;
		result = sqrt(pow((double)V1.GetX(), 2)+pow((double)V1.GetY(), 2)+pow((double)V1.GetZ(), 2));
		return result;
	}
	else
	{
		return 0;
	}
}

double CVector3D::CrossSqrMagnitude(const CVector3D &V)
{
	if(!IsNull() && !V.IsNull())
	{
		CVector3D V1 = (*this);
		CVector3D V2 = V;
		V1.Cross(V2);
		double result;
		result = pow((double)V1.GetX(), 2)+pow((double)V1.GetY(), 2)+pow((double)V1.GetZ(), 2);
		return result;
	}
	else
	{
		return 0;
	}
}

double CVector3D::Modulus()
{
	return sqrt(itsX*itsX + itsY*itsY + itsZ*itsZ);
}

CVector3D CVector3D::Unit()
{
	CVector3D result;
	double x, y, z;
	if(!IsNull())
	{
		x = GetX()/Magnitude();
		y = GetY()/Magnitude();
		z = GetZ()/Magnitude();
		result.SetX(x);
		result.SetY(y);
		result.SetZ(z);
		return result;
	}
	else
		return *this;
}

CPoint3D CVector3D::Point() const
{
	CPoint3D aPnt;
	aPnt.SetParam(GetX(), GetY(), GetZ());
	return aPnt;
}

double CVector3D::Angle(const CVector3D& Other) const
{
	double theta, a, b, dot;
	CVector3D tempV = Other;
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

double CVector3D::Angle(const CVector3D& Other, const CVector3D& Dir) const
{
	double theta;
	CVector3D tempV = Other;
	CVector3D D = Dir;
	CVector3D N = this->Crossed(tempV);
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

bool CVector3D::IsNull() const
{
	if(itsX==0&&itsY==0&&itsZ==0)
		return true;
	else
		return false;
}

bool CVector3D::IsParallel(const CVector3D& V) const
{
	/*double ang=0;
	CVector3D tempV = V;
	ang = this->Angle(tempV);
	if(ang==0 || ang==(double)2*PI)
		return true;
	else
		return false;*/
	CVector3D N = this->Crossed(V);
	return (N.IsNull());
}

bool CVector3D::IsOpposite(const CVector3D& V) const

{
	double ang=0;
	CVector3D tempV = V;
	ang = this->Angle(tempV);
	if(fabs(ang-PI)<=0.0001)

		return true;
	else
		return false;
}

bool CVector3D::IsNormal(const CVector3D& V) const
{
	double ang=0;
	CVector3D tempV = V;
	ang = this->Angle(tempV);
	if(fabs(ang-PI/2)<=0.0001)
		return true;
	else
		return false;
}

void CVector3D::Print() const
{
	cout<<"X = "<<GetX()<<endl;
	cout<<"Y = "<<GetY()<<endl;
	cout<<"Z = "<<GetZ()<<endl;
}

void CVector3D::Normalize()
{
	double x, y, z;
	if(!IsNull())
	{
		x = GetX()/Magnitude();
		y = GetY()/Magnitude();
		z = GetZ()/Magnitude();
		SetX(x);
		SetY(y);
		SetZ(z);
	}
	else
		return;
}

void CVector3D::Translate(const COneAxis& Ax, const double& amt)
{
	CGeometry::Translate(Ax, amt);
}

void CVector3D::Translate(double dx, double dy, double dz)
{
	CPoint3D P = (*this);
	P.Translate(dx, dy, dz);
	CVector3D V = P;
	(*this) = V;

}

void CVector3D::Translate(const CVector3D& dV)
{
	CPoint3D P = (*this);
	P.Translate(dV);
	CVector3D V = P;
	(*this) = V;
}

void CVector3D::Translate(const CPoint3D& P1, const CPoint3D& P2)
{
	CPoint3D P = (*this);
	P.Translate(P1, P2);
	CVector3D V = P;
	(*this) = V;
}

void CVector3D::Rotate(const COneAxis& Ax, double ang)
{
	COneAxis ax = Ax;
	CVector3D P1 = ax.GetOrigin();
	CVector3D V = ax.GetDirection();
	V.Normalize();
	CMatrix33 M; CVector3D PV(*this);
	M.SetRotation(V, ang);
	CVector3D RV = M*(PV-P1);
	RV = RV + P1;
	*this = RV;
}

void CVector3D::Scale(const CPoint3D& P1, double fact)
{
	CPoint3D P = (*this);
	P.Scale(P1, fact);
	CVector3D V = P;
	(*this) = V;
}

void CVector3D::Mirror(const CPoint3D& P1)
{
	CPoint3D P = (*this);
	P.Mirror(P1);
	CVector3D V = P;
	(*this) = V;
}

void CVector3D::Mirror(const COneAxis& Ax)
{
	CPoint3D P = (*this);
	P.Mirror(Ax);
	CVector3D V = P;
	(*this) = V;
}

void CVector3D::Mirror(const CPlane& Pln)
{
	CPoint3D P = (*this);
	P.Mirror(Pln);
	CVector3D V = P;
	(*this) = V;
}

CVector3D CVector3D::Origin()
{
	return CVector3D(0,0,0);
}

std::istream &operator>>(std::istream& input, CVector3D &p)
{
    input>>p.itsX>>p.itsY>>p.itsZ;
    return input;
}

std::ostream &operator<<(std::ostream& output, CVector3D &p)
{
    output<< p.itsX << " " << p.itsY << " " << p.itsZ;
    return output;
}

