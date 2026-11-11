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
 *            GLColor.h
 *
 *  Thu Jun 29 22:49:26 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _GLCOLOR_H
#define _GLCOLOR_H

#include "VKGraphic.h"

class VKGRAPHIC_API CGLColor
{
    public:
	CGLColor();
	CGLColor(const float& red, const float& green, const float& blue);
	~CGLColor();
	void SetValues(const float& red, const float& green, const float& blue);
	float GetRed()   const;
	float GetGreen() const;
	float GetBlue()  const;
    private:
	float itsRed;
	float itsGreen;
	float itsBlue;
};

#endif /* _GLCOLOR_H */
