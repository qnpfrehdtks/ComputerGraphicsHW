
#include "Util.h"

void Util::DrawTriangle(point3 & a, point3 & b, point3 & c)
{
	point3 n;
	Cross(a, b, c, n);

	glBegin(GL_TRIANGLES);
	  glNormal3fv(n);
	  glVertex3fv(a);
	  glVertex3fv(b);
	  glVertex3fv(c);
	  
	glEnd();

}

void Util::SmoothDrawTriangle(GLfloat* a, GLfloat* b, GLfloat* c)
{
	glColor3f(0, 0, 1);
	glBegin(GL_TRIANGLES);
	glNormal3fv(a);
	glVertex3fv(a);
	glNormal3fv(b);
	glVertex3fv(b);
	glNormal3fv(c);
	glVertex3fv(c);

	glEnd();

}

void Util::DrawPoints(point3 & a)
{

	glBegin(GL_POINTS);
	glColor3f(0, 0, 1);

	glVertex3fv(a);


	glEnd();

}

void Util::DrawLine(point3 & a, point3 & b)
{
	glBegin(GL_LINES);

	glVertex3fv(a);
	glVertex3fv(b);

	glEnd();

}



void Util::DrawFaceNormal(point3 & a, point3 & b, point3 & c)
{
	point3 n;
	Cross(a, b, c, n);

	point3 centerPt;

	centerPt[0] = (a[0] + b[0] + c[0]) / (float)3;
	centerPt[1] = (a[1] + b[1] + c[1]) / (float)3;
	centerPt[2] = (a[2] + b[2] + c[2]) / (float)3;

	n[0] += centerPt[0];
	n[1] += centerPt[1];
	n[2] += centerPt[2];

	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glNormal3fv(n);
	
	glVertex3fv(n);
	glVertex3fv(centerPt);

	glEnd();

}



void Util::Normalize(point3 & p)
{
	double d = pow(p[0], 2) + pow(p[1], 2) + pow(p[2], 2);
	if (d > 0)
	{
		float len = (float)(1.0 / sqrt(d));
		p[0] *= len;
		p[1] *= len;
		p[2] *= len;
	}


}



void Util::Cross(point3 & a, point3 & b, point3 & c, point3 & r)
{
	r[0] = (b[1] - a[1]) * (c[2] - a[2]) - (b[2] - a[2]) * (c[1] - a[1]);
	r[1] = (b[2] - a[2]) * (c[0] - a[0]) - (b[0] - a[0]) * (c[2] - a[2]);
	r[2] = (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]);
	Normalize(r);

}

void Util::CrossProduct(point3 & a, point3 & b, point3 & r)
{

}

GLfloat * Util::CalFaceNormal(point3 & a, point3 & b, point3 & c)
{
	return nullptr;
}

GLfloat * Util::CalPointAvg(point3 & a, point3 & b, point3 & c)
{
	GLfloat point[3];
	point[0] =( a[0] + b[0] + c[0] )/ (float)3;
	point[1] = (a[1] + b[1] + c[1]) / (float)3;
	point[2] = (a[2] + b[2] + c[2]) / (float)3;

	return point;
}

void Util::fltChangeToPt3(GLfloat * pt1, point3 & pt2)
{
	pt2[0] = pt1[0];
	pt2[1] = pt1[1];
	pt2[2] = pt1[2];
}
