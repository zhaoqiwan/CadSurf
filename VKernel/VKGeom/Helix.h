// Helix.h: interface for the CHelix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HELIX_H__8195CAF2_E922_4631_8E35_AD84A07EE6A1__INCLUDED_)
#define AFX_HELIX_H__8195CAF2_E922_4631_8E35_AD84A07EE6A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VKGeom.h"

#include "Circle3D.h"
#include "AxisSystem.h"

class VKGEOM_API CHelix : public CCircle3D 
{
public:
	CHelix(const CAxisSystem& Ax, const double& radius, const double& start, const double& end, const double lead);
	virtual ~CHelix();
	virtual void Reverse();
	virtual CPoint3D PointAtPara(const double& para);
	virtual double FirstParameter() const;
	virtual double LastParameter() const;
	virtual bool IsClosed() const;
	CurveType Type() const;
	bool IsOfType(const CurveType&) const;
	virtual CCurve* Copy() const;
	virtual void Translate(double dx, double dy, double dz);
	virtual void Translate(const CVector3D&);
	virtual void Translate(const CPoint3D&, const CPoint3D&);
	virtual void Rotate(const COneAxis&, double);
	virtual void Scale(const CPoint3D&, double);
	virtual void Mirror(const CPoint3D&);
	virtual void Mirror(const COneAxis&);
	virtual void Mirror(const CPlane&);
	double Lead() const { return itsLead;}
private:
	//CAxisSystem itsLocation;
	//double itsRadius;
	double itsFirstParameter;
	double itsLastParameter;
	double itsLead;
};

#endif // !defined(AFX_HELIX_H__8195CAF2_E922_4631_8E35_AD84A07EE6A1__INCLUDED_)
