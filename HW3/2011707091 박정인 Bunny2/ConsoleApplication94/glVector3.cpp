#include "glVector3.h"

#define EPSILON 0.0001f

glVector3::glVector3(GLfloat _x , GLfloat _y , GLfloat _z)
{
	point[0] = _x;
	point[1] = _y;
	point[2] = _z;

}
glVector3::glVector3(glVector3 & Vector)
{
	point[0] = Vector.point[0];
	point[1] = Vector.point[1];
	point[2] = Vector.point[2];
}
glVector3::~glVector3()
{

}

bool glVector3::operator==(glVector3& vec)
{
	if (fabs(point[0] - vec.point[0]) < EPSILON) return false;
	if (fabs(point[1] - vec.point[1]) < EPSILON) return false;
	if (fabs(point[2] - vec.point[2]) < EPSILON) return false;

	return true;
}

bool glVector3::operator!=(glVector3& vec)
{
	return !(*this == vec);
}

glVector3 glVector3::operator+(glVector3& vec)
{
	return glVector3(point[0] + vec.point[0], point[1] + vec.point[1], point[2] + vec.point[2]);
}

glVector3 glVector3::operator-(glVector3& vec)
{
	return glVector3(point[0] - vec.point[0], point[1] - vec.point[1], point[2] - vec.point[2]);
}

glVector3 glVector3::operator*(float f)
{
	return glVector3(point[0] * f, point[1] * f, point[2] * f);
}

GLfloat glVector3::Length()
{
	return sqrtf(point[0] * point[0] + point[1] * point[1] + point[2] * point[2]);
}

//정규화
glVector3 glVector3::Normalize()
{
	float l = Length();
	return glVector3(point[0] / l, point[1] / l, point[2] / l);
}

//내적
GLfloat glVector3::Dot(glVector3& v1, glVector3& v2)
{
	return v1.point[0] * v2.point[0] + v1.point[1] * v2.point[1] + v1.point[2] * v2.point[2];
}
//외적
glVector3 glVector3::Cross(glVector3& v1, glVector3& v2)
{
	return glVector3(
		v1.point[1] * v2.point[2] - v1.point[2] * v2.point[1],
		v1.point[0] * v2.point[2] - v1.point[2] * v2.point[0],
		v1.point[0] * v2.point[1] - v1.point[1] * v2.point[0]);
}

glVector3 glVector3::calFaceNormalVector(glVector3 & v1, glVector3 & v2, glVector3 & v3)
{
	glVector3 DifVector1 = v2 - v1;
	glVector3  DifVector2 = v3 - v1;

	return Cross(DifVector1, DifVector2).Normalize();
}
