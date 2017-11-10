#pragma once
#include "Header.h"
class cMatrix;

class cVector3
{
public:
	cVector3(GLfloat _x = 0.f, GLfloat _y = 0.f, GLfloat _z = 0.f, GLfloat _w = 0.f);
	~cVector3();

	GLfloat pt[4];

	cVector3 operator+(cVector3& vec);
	cVector3 operator-(cVector3& vec);
	cVector3 operator*(float f);
	float Length();

	//¡§±‘»≠
	 cVector3 Normalize();
	cVector3 Transform(cMatrix& mat);
};