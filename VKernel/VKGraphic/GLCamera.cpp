// GLCamera.cpp: implementation of the CGLCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "GLCamera.h"
#include <MMath.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SF3dVector F3dVector ( GLdouble x, GLdouble y, GLdouble z )
{
    SF3dVector tmp;
    tmp.x = x;
    tmp.y = y;
    tmp.z = z;
    return tmp;
}

SF3dVector AddF3dVectors (SF3dVector* u, SF3dVector* v)
{
    SF3dVector result;
    result.x = u->x + v->x;
    result.y = u->y + v->y;
    result.z = u->z + v->z;
    return result;
}

void AddF3dVectorToVector ( SF3dVector * Dst, SF3dVector * V2)
{
    Dst->x += V2->x;
    Dst->y += V2->y;
    Dst->z += V2->z;
}


/***************************************************************************************/

CGLCamera::CGLCamera()
{
    //Init with standard OGL values:
    Position = F3dVector (	0.0, 
	    0.0,
	    0.0);
    ViewDir = F3dVector(	0.0,
	    0.0,
	    -1.0);
    ViewDirChanged = false;
    //Only to be sure:
    RotatedX = RotatedY = RotatedZ = 0.0;
}

void CGLCamera::GetViewDir( void )
{
    SF3dVector Step1, Step2;
    //Rotate around Y-axis:
    Step1.x = cos( (RotatedY + 90.0) * PIdiv180);
    Step1.z = -sin( (RotatedY + 90.0) * PIdiv180);
    //Rotate around X-axis:
    double cosX = cos (RotatedX * PIdiv180);
    Step2.x = Step1.x * cosX;
    Step2.z = Step1.z * cosX;
    Step2.y = sin(RotatedX * PIdiv180);
    //Rotation around Z-axis not yet implemented, so:
    ViewDir = Step2;
}

void CGLCamera::Move (SF3dVector Direction)
{
    AddF3dVectorToVector(&Position, &Direction );
}

void CGLCamera::RotateY (GLdouble Angle)
{
    if( RotatedY > 360 || RotatedY < -360 )
	RotatedY = 0;
    RotatedY += Angle;
    ViewDirChanged = true;
}

void CGLCamera::RotateX (GLdouble Angle)
{
    if( RotatedY > 360 || RotatedY < -360 )
	RotatedX = 0;
    RotatedX += Angle;
    ViewDirChanged = true;
}

void CGLCamera::Render( void )
{
    ::glRotated(-RotatedX , 1.0, 0.0, 0.0);
    ::glRotated(-RotatedY , 0.0, 1.0, 0.0);
    ::glRotated(-RotatedZ , 0.0, 0.0, 1.0);
    ::glTranslated( -Position.x, -Position.y, -Position.z );
}

void CGLCamera::MoveForwards( GLdouble Distance )
{
    if (ViewDirChanged) GetViewDir();
    SF3dVector MoveVector;
    MoveVector.x = ViewDir.x * -Distance;
    MoveVector.y = ViewDir.y * -Distance;
    MoveVector.z = ViewDir.z * -Distance;
    AddF3dVectorToVector(&Position, &MoveVector );
}

void CGLCamera::StrafeRight ( GLdouble Distance )
{
    if (ViewDirChanged) GetViewDir();
    SF3dVector MoveVector;
    MoveVector.z = -ViewDir.x * -Distance;
    MoveVector.y = 0.0;
    MoveVector.x = ViewDir.z * -Distance;
    AddF3dVectorToVector(&Position, &MoveVector );
}

void CGLCamera::ResetCamera()
{
    Position = F3dVector(0.0, 0.0,	0.0);
    ViewDir = F3dVector(0.0, 0.0, -1.0);
    ViewDirChanged = false;
    RotatedX = RotatedY = RotatedZ = 0.0;
}

void CGLCamera::SetPosition(SF3dVector pos)
{
    Position.x = pos.x;
    Position.y = pos.y;
    Position.z = pos.z;
}
