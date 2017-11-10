#pragma once
#include "Header.h"

class Util
{
private:
	point3* Vertex;
	int* pIndex;

	point3 faceNormal[3000];

	int NormalNum;


public:
	
	
	void SetVertexInfo(point3 *Point, int *Index)
	{
		Vertex = Point;
		pIndex = Index;
	}

	//삼각형 그리기 용도
	void DrawTriangle(point3 &a, point3 &b, point3 &c);
	// Smooth 한 삼각형 그리기
	void SmoothDrawTriangle(point3 & a, point3 & b, point3 & c);

	//선그리기
	void DrawLine(point3 & a, point3 & b);
	//점 그리기
	void DrawPoints(point3 & a);

	//Face 노말 그려주는  함수
	void DrawFaceNormal(point3 & a, point3 & b, point3 & c);

	void Normalize(point3 &p);

	void Cross(point3 &a, point3 &b, point3 &c, point3 &r);
	void CrossProduct(point3 &a, point3 &b, point3 &r);

	GLfloat* CalFaceNormal(point3 &a, point3 &b, point3 &c);
	GLfloat* CalPointAvg(point3 &a, point3 &b, point3 &c);

	void fltChangeToPt3(GLfloat* pt1, point3& pt2);
	



};