
#include "cVector3.h"
#include "cMatrix.h"


cVector3::cVector3(GLfloat _x , GLfloat _y , GLfloat _z , GLfloat _w)
{
	pt[0] = _x;
	pt[1] = _y;
	pt[2] = _z;
	pt[3] = _w;
}
cVector3::~cVector3()
{

}

cVector3 cVector3::operator+(cVector3& vec)
{
	return cVector3(pt[0] + vec.pt[0], pt[1] + vec.pt[1], pt[2] + vec.pt[2]);
}

cVector3 cVector3::operator-(cVector3& vec)
{
	return cVector3(pt[0] - vec.pt[0], pt[1] - vec.pt[1], pt[2] - vec.pt[2]);
}

cVector3 cVector3::operator*(float f)
{
	return cVector3(pt[0] * f, pt[1] * f, pt[2] * f);
}

float cVector3::Length()
{
	return sqrtf(pt[0] * pt[0] + pt[1] * pt[1] + pt[2] * pt[2]);
}

//¡§±‘»≠
cVector3 cVector3::Normalize()
{
	float l = Length();
	return cVector3(pt[0] / l, pt[1] / l, pt[2] / l);
}


cVector3 cVector3::Transform(cMatrix& mat)
{
	cVector3 v;
	v.pt[0] = mat[0][0] * pt[0] + mat[0][1] * pt[1] + mat[0][2] * pt[2] + mat[0][3];
	v.pt[1] = mat[1][0] * pt[0] + mat[1][1] * pt[1] + mat[1][2] * pt[2] + mat[1][3];
	v.pt[2] = mat[2][0] * pt[0] + mat[2][1] * pt[1] + mat[2][2] * pt[2] + mat[2][3];
	v.pt[3] = pt[3];
	return cVector3(v.pt[0], v.pt[1], v.pt[2], v.pt[3]);
}

