//############################################################################

//  Author:	Sharjith Nair
//  Email:	sharjith_ssn@hotmail.com

// Program:	OpenGL, MFC C++ 

//							Important Notice:

//	This Idea and the Application is Copyright(c) Sharjith Nair 2002, 2003.
//	You can freely use it as long as you credit me for it.

//	No guarantee/warranty is given on this app and I will not be responsible 
//	for any damage to you, your property or any other person from using it.
//	USE IT ON YOUR OWN RISK.

//	Thankyou
//	Sharjith Nair.

//#############################################################################

// GLTrihedron.h: interface for the CGLTrihedron class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_GLTRIHEDRON_H__F1DE074D_EC62_4F52_9890_7612816C931B__INCLUDED_)
#define _GLTRIHEDRON_H__F1DE074D_EC62_4F52_9890_7612816C931B__INCLUDED_

#include "VKGraphic.h"

#include "GLObject.h"
#include "font.h"

class VKGRAPHIC_API CGLTrihedron : public CGLObject  
{
public:
	CGLTrihedron();
	CGLTrihedron(GLfloat size);
	virtual ~CGLTrihedron();
	virtual CGLObject* Copy() const;
	virtual void DefineDisplay();
	virtual void Display(const GLDisplayMode& = GLWIREFRAME);
	virtual void Hilight(const GLDisplayMode&);
	void SetSize(GLfloat size); 
private:
	GLfloat csSize;
	static GLuint list;
	GLUquadricObj* coneObj;
	GLuint base;            // base display list for the font set.
	GLuint triList;
	bool firstTime;
	_GLFONT *Font;
private:
	GLvoid BuildFont(GLvoid);
	void DrawText(char* string);
	void ComputeBoundLimits();
	void InitDisplayLists();

};

#endif // !defined(_GLTRIHEDRON_H__F1DE074D_EC62_4F52_9890_7612816C931B__INCLUDED_)
