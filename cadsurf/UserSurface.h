// UserSurface.h: interface for the CUserSurface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERSURFACE_H__799994D7_512A_4200_877D_F7AD0B3DF66E__INCLUDED_)
#define AFX_USERSURFACE_H__799994D7_512A_4200_877D_F7AD0B3DF66E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <BasicSurface.h>

class CUserSurface : public CBasicSurface  
{
public:
	CUserSurface();
	virtual ~CUserSurface();
	virtual CPoint3D PointAtPara(const double uPar, const double vPar);
	virtual double FirstUParameter() const;
	virtual double LastUParameter() const;
	virtual double FirstVParameter() const;
	virtual double LastVParameter() const;
	virtual bool IsUClosed() const;
	virtual bool IsVClosed() const;
	virtual CSurface* Copy() const;
	virtual void Translate(double dx, double dy, double dz);
	virtual void Translate(const CVector3D&);
	virtual void Translate(const CPoint3D&, const CPoint3D&);
	virtual void Rotate(const COneAxis&, double);
	virtual void Scale(const CPoint3D&, double);
	virtual void Mirror(const CPoint3D&);
	virtual void Mirror(const COneAxis&);
	virtual void Mirror(const CPlane&) ;
};

#endif // !defined(AFX_USERSURFACE_H__799994D7_512A_4200_877D_F7AD0B3DF66E__INCLUDED_)
