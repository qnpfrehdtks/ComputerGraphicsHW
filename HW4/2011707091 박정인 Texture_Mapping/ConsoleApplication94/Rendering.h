#pragma once
#include "Header.h"
#include "glVector3.h"

class Rendering
{
	point3 Vertex;
	int pIndex;

public:
	Rendering();
	~Rendering();

	void drawFaceNormal(glVector3 a, glVector3 b, glVector3 c);



};

