//############################################################################

//  Author:	Sharjith Nair
//  Email:	sharjith_ssn@hotmail.com

// Program:	OpenGL, C++ MFC Surface Modeler Demo Application

//							Important Notice:

//	This Idea and the Application is Copyright(c) Sharjith Nair 2002, 2003.
//	You can freely use it as long as you credit me for it.

//	No guarantee/warranty is given on this app and I will not be responsible 
//	for any damage to you, your property or any other person from using it.
//	USE IT ON YOUR OWN RISK.

//	Thankyou
//	Sharjith Nair.

//#############################################################################

// GLFont.h: interface for the CGLFont class.
//
// Idea And Part Of Code Taken From
// OpenGL SuperBible 2nd Ed
// OpenGL raster font demo from Chapter 7.
// 
// Original Code Written by Michael Sweet.
//
// Modified By N Sharjith Into C++ Code

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLFONT_H__B9B714C5_7F4D_452D_A20F_B773A9CB0BF3__INCLUDED_)
#define AFX_GLFONT_H__B9B714C5_7F4D_452D_A20F_B773A9CB0BF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VKGraphic.h"

#include "GLObject.h"
#include "Point3D.h"

class CPoint3D;

enum TextAlign { CENTER = 0, LEFT, RIGHT };

class VKGRAPHIC_API CGLFont : public CGLObject  
{
public:
	CGLFont(const string&, const CPoint3D&, const char *tf = "MS Sans Serif", int ht = 16,
	  int wt = 0, DWORD it = 0, const TextAlign& = LEFT);
	virtual ~CGLFont();
	void DrawString();
	virtual CGLObject* Copy() const;
	virtual void DefineDisplay();
	virtual void Display(const GLDisplayMode& = GLWIREFRAME);
	virtual void Hilight(const GLDisplayMode&);
private:
	void ComputeBoundLimits();
private:
	GLuint base;        /* Display list number of first character */
    int    fwidths[256]; /* Width of each character in pixels */
    int    fheight;      /* Height of characters */
	CPoint3D itsPosition;
	TextAlign itsAlignment;
	const char* typeface; // I - Font specification
	int height;           // I - Font height/size in pixels
	int weight;           // I - Weight of font (bold, etc)
	DWORD italic;
	std::string itsStr;
};

#endif // !defined(AFX_GLFONT_H__B9B714C5_7F4D_452D_A20F_B773A9CB0BF3__INCLUDED_)
