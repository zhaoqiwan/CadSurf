/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
/***************************************************************************
 *            Vector2D.h
 *
 *  Thu Jun 29 22:44:35 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _VECTOR2D_H
#define _VECTOR2D_H

#include "VKGeom.h"

#include "Geometry2D.h"

class CPoint2D;
class CMatrix22;
class COneAxis2D;

class VKGEOM_API CVector2D : public CGeometry2D  
{
public:
	CVector2D();
	CVector2D(double x, double y);
	CVector2D(const CPoint2D&);
	CVector2D(const CVector2D&, const CVector2D&);
	CVector2D(const CPoint2D&, const CPoint2D&);
	~CVector2D();
	inline void SetX(double x){itsX = x;} //Set The X Component Value
	inline void SetY(double y){itsY = y;} //Set The Y Component Value
	inline void SetParam(double x, double y){itsX = x; itsY = y;} //Set The X Y and Z Values
	inline double GetX() const {return itsX;}// Returns The X Component
	inline double GetY() const {return itsY;}// Returns The Y Component
	double Dot(const CVector2D &V1) const;//Calculate Dot Product
	double DotCross(const CVector2D &V1, const CVector2D &V2);//Calculate Scalar Triple Product
	void Cross(const CVector2D &V1);//Calculate Cross Product Of' this' And Other
	CVector2D Crossed(const CVector2D &V1) const;//Return The Cross Product Vector
	void CrossCross(const CVector2D &V1, const CVector2D &V2);//Calculate The Vector Triple Product
	CVector2D CrossCrossed(const CVector2D &V1, const CVector2D &V2);//Return Vector Triple Product
	void Reverse();//Reverse The Direction Of 'this'
	CVector2D Reversed();//Return The Reversed Vector Of 'this'
	CVector2D operator + (const CVector2D &V1);//Return Addition Of 'this' And Other
	void operator += (const CVector2D &V1);//Add Other To 'this'
	CVector2D operator - (const CVector2D &V1);//Return Subtraction Of 'this' And Other
	void operator -= (const CVector2D &V1);//Subtract Other From 'this'
	CVector2D operator * (const double &scalar);//Scalar Multiple With Other Vector
	CVector2D operator * (const CMatrix22& M);//Multiply With Matrix
	void operator *= (const double &scalar);//Multiply 'this' With Other
	void operator *= (const CMatrix22 &M);//Multiply 'this With Matrix
	CVector2D operator / (const double &scalar);//Scalar Division
	void operator /= (const double &scalar);//Divide 'this' By Scalar
	CVector2D operator ^ (const CVector2D &V1);//Return The Cross Product
	void operator ^= (const CVector2D &V1);//Calculate Cross Product Of' this' And Other
	double Magnitude() const;// Calculate 'this' Vector's Magnitude
	double SqrMagnitude() const;// Calculate 'this' Vector's Square Magnitude
	double CrossMagnitude(const CVector2D &V);// Calculate 'this' Vector's Cross Magnitude
	double CrossSqrMagnitude(const CVector2D &V1);// Calculate 'this' Vector's Cross Square Magnitude
	double Modulus();// Return Modulus
	CVector2D Unit();//Return Normalized Vector Of 'this'
	CPoint2D Point();//Return Component Triplet As Point
	double Angle(const CVector2D&) const;//Angle Bet'n 'this' And Other
	double Angle(const CVector2D&, const CVector2D&) const;//Angle Bet'n 'this' And Other WRT Direction
	bool IsNull() const;//Check For Null Vector
	bool IsParallel(const CVector2D&) const;//Check If Parallel To Other
	bool IsOpposite(const CVector2D&) const;//Check If Opposite Direction To Other
	bool IsNormal(const CVector2D&) const;//Check If Normal To Other
	bool operator == (const CVector2D& V) const;//Check Equality
	operator CPoint2D();//Convertion Cast To Point;
	void Print() const;//Print Values Of Components
	void Normalize();//Normalize 'this' Vector
	virtual void Translate(const COneAxis2D&, const double&);
	virtual void Translate(double dx, double dy);
	virtual void Translate(const CVector2D&);
	virtual void Translate(const CPoint2D&, const CPoint2D&);
	virtual void Rotate(const CPoint2D&, double);
	virtual void Scale(const CPoint2D&, double);
	virtual void Mirror(const CPoint2D&);
	virtual void Mirror(const COneAxis2D&);
	static CVector2D Origin();
private:
	double itsX;
	double itsY;
};

#endif /* _VECTOR2D_H */
