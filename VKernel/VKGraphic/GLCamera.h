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
 *            GLCamera.h
 *
 *  Thu Jun 29 22:46:35 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/

/**********************************************************************

//Part of this code is taken from "camera" tutorial written by Philipp Crocoll
//Contact: 
//philipp.crocoll@web.de
//www.codecolony.de
 **********************************************************************/

#ifndef _GLCAMERA_H
#define _GLCAMERA_H

#include "VKGraphic.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <MMath.h>

#define PIdiv180 PI/180.0

/////////////////////////////////
//Note: All angles in degrees  //
/////////////////////////////////

struct VKGRAPHIC_API SF3dVector  //Float 3d-vect, normally used
{
    GLdouble x,y,z;
};
struct VKGRAPHIC_API SF2dVector
{
    GLdouble x,y;
};

class VKGRAPHIC_API CGLCamera
{
    public:
	CGLCamera();			//inits the values (Position: (0|0|0) Target: (0|0|-1) )
	void Render ( void );	//executes some glRotates and a glTranslate command
							//Note: You should call glLoadIdentity before using Render
	void Move ( SF3dVector Direction );
	void RotateX ( GLdouble Angle );
	void RotateY ( GLdouble Angle );
	void RotateZ ( GLdouble Angle );
	void RotateXYZ ( SF3dVector Angles );
	void MoveForwards ( GLdouble Distance );
	void StrafeRight ( GLdouble Distance );

	//Added By N.Sharjith
	void ResetCamera();
	void SetPosition(SF3dVector pos);
	SF3dVector GetPosition() const { return Position; }
	//Added

    private:
	SF3dVector Position;
	SF3dVector ViewDir;		/*Not used for rendering the camera, but for "moveforwards"
					  So it is not necessary to "actualize" it always. It is only
					  actualized when ViewDirChanged is true and moveforwards is called*/
	bool ViewDirChanged;
	GLdouble RotatedX, RotatedY, RotatedZ;	
	void GetViewDir ( void );
};


VKGRAPHIC_API SF3dVector F3dVector ( GLdouble x, GLdouble y, GLdouble z );
VKGRAPHIC_API SF3dVector AddF3dVectors ( SF3dVector * u, SF3dVector * v);
VKGRAPHIC_API void AddF3dVectorToVector ( SF3dVector * Dst, SF3dVector * V2);

#endif /* _GLCAMERA_H */
