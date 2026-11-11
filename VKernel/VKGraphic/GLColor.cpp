
#include "stdafx.h"

#include "GLColor.h"
#include "MMath.h"

CGLColor::CGLColor()
{
    itsRed = 0.5f;
    itsGreen = 0.5f;
    itsBlue = 0.5f;
}

CGLColor::CGLColor(const float& red, const float& green, const float& blue)
{
    itsRed =	(red < 0) ? 0 : ((red > 1) ? 1 : red);
    itsGreen =	(green < 0) ? 0 : ((green > 1) ? 1 : green);
    itsBlue =	(blue < 0) ? 0 : ((blue > 1) ? 1 : blue);
}

CGLColor::~CGLColor()
{
}

void CGLColor::SetValues(const float& red, const float& green, const float& blue)
{
    itsRed =	(red < 0) ? 0 : ((red > 1) ? 1 : red);
    itsGreen =	(green < 0) ? 0 : ((green > 1) ? 1 : green);
    itsBlue =	(blue < 0) ? 0 : ((blue > 1) ? 1 : blue);
}

float CGLColor::GetRed()   const
{
    return itsRed;
}

float CGLColor::GetGreen() const
{
    return itsGreen;
}

float CGLColor::GetBlue()  const
{
    return itsBlue;
}

