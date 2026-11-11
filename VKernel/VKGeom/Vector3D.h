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
 *            Vector3D.h
 *
 *  Thu Jun 29 22:45:19 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _VECTOR3D_H
#define _VECTOR3D_H

#include "VKGeom.h"

#include <iostream>
using namespace std;

#include "Geometry.h"

class CPoint3D;
class CMatrix33;
class COneAxis;
class CPlane;

class VKGEOM_API CVector3D : public CGeometry  
{
    friend std::istream &operator>>(std::istream& , CVector3D&);
    friend std::ostream &operator<<(std::ostream& , CVector3D&);
public:
	CVector3D();
	CVector3D(double x, double y, double z);
	CVector3D(const CPoint3D&);
	CVector3D(const CVector3D&, const CVector3D&);
	CVector3D(const CPoint3D&, const CPoint3D&);
	~CVector3D();
	inline void SetX(double x){itsX = x;} ///Set The X Component Value
	inline void SetY(double y){itsY = y;} ///Set The Y Component Value
	inline void SetZ(double z){itsZ = z;} ///Set The Z Component Value
	inline void SetParam(double x, double y, double z){itsX = x; itsY = y;itsZ = z;} ///Set The X Y and Z Values
	inline double GetX() const {return itsX;}/// Returns The X Component
	inline double GetY() const {return itsY;}/// Returns The Y Component
	inline double GetZ() const {return itsZ;}/// Returns The Z Component
	double Dot(const CVector3D &V1) const;///Calculate Dot Product
	double DotCross(const CVector3D &V1, const CVector3D &V2);///Calculate Scalar Triple Product
	void Cross(const CVector3D &V1);///Calculate Cross Product Of' this' And Other
	CVector3D Crossed(const CVector3D &V1) const;///Return The Cross Product Vector
	void CrossCross(const CVector3D &V1, const CVector3D &V2);///Calculate The Vector Triple Product
	CVector3D CrossCrossed(const CVector3D &V1, const CVector3D &V2);///Return Vector Triple Product
	void Reverse();///Reverse The Direction Of 'this'
	CVector3D Reversed();///Return The Reversed Vector Of 'this'
	CVector3D operator + (const CVector3D &V1);///Return Addition Of 'this' And Other
	void operator += (const CVector3D &V1);///Add Other To 'this'
	CVector3D operator - (const CVector3D &V1);///Return Subtraction Of 'this' And Other
	void operator -= (const CVector3D &V1);///Subtract Other From 'this'
	CVector3D operator * (const double &scalar);///Scalar Multiple With Other Vector
	CVector3D operator * (const CMatrix33& M);///Multiply With Matrix
	void operator *= (const double &scalar);///Multiply 'this' With Other
	void operator *= (const CMatrix33 &M);///Multiply 'this With Matrix
	CVector3D operator / (const double &scalar);///Scalar Division
	void operator /= (const double &scalar);///Divide 'this' By Scalar
	CVector3D operator ^ (const CVector3D &V1);///Return The Cross Product
	void operator ^= (const CVector3D &V1);///Calculate Cross Product Of' this' And Other
	double Magnitude() const;/// Calculate 'this' Vector's Magnitude
	double SqrMagnitude() const;/// Calculate 'this' Vector's Square Magnitude
	double CrossMagnitude(const CVector3D &V);/// Calculate 'this' Vector's Cross Magnitude
	double CrossSqrMagnitude(const CVector3D &V1);/// Calculate 'this' Vector's Cross Square Magnitude
	double Modulus();/// Return Modulus
	CVector3D Unit();///Return Normalized Vector Of 'this'
	CPoint3D Point() const;///Return Component Triplet As Point
	double Angle(const CVector3D&) const;///Angle Bet'n 'this' And Other
	double Angle(const CVector3D&, const CVector3D&) const;///Angle Bet'n 'this' And Other WRT Direction
	bool IsNull() const;///Check For Null Vector
	bool IsParallel(const CVector3D&) const;///Check If Parallel To Other
	bool IsOpposite(const CVector3D&) const;///Check If Opposite Direction To Other
	bool IsNormal(const CVector3D&) const;///Check If Normal To Other
	bool operator == (const CVector3D& V) const;///Check Equality
	operator CPoint3D();///Convertion Cast To Point;
	void Print() const;///Print Values Of Components
	void Normalize();///Normalize 'this' Vector
	virtual void Translate(const COneAxis&, const double&);
	virtual void Translate(double dx, double dy, double dz);
	virtual void Translate(const CVector3D&);
	virtual void Translate(const CPoint3D&, const CPoint3D&);
	virtual void Rotate(const COneAxis&, double);
	virtual void Scale(const CPoint3D&, double);
	virtual void Mirror(const CPoint3D&);
	virtual void Mirror(const COneAxis&);
	virtual void Mirror(const CPlane&);
	static CVector3D Origin();
private:
	double itsX;
	double itsY;
	double itsZ;
};

#endif /* _VECTOR3D_H */
