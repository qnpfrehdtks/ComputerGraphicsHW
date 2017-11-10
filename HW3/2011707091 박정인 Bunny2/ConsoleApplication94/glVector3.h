#pragma once
#include "Header.h"

class glVector3
{
public:
	point3 point;


public:
	glVector3(GLfloat _x = 0, GLfloat _y = 0, GLfloat _z = 0);
	glVector3(glVector3& Vector);
	~glVector3();

	bool operator==(glVector3& vec);
	bool operator!=(glVector3& vec);
	glVector3 operator+(glVector3& vec);
	glVector3 operator-(glVector3& vec);
	glVector3 operator*(float f);
	float Length();

	glVector3 Normalize();

	static float Dot(glVector3& v1, glVector3& v2);

	static glVector3 Cross(glVector3& v1, glVector3& v2);
	static glVector3 calFaceNormalVector(glVector3& v1, glVector3& v2, glVector3& v3);

};

