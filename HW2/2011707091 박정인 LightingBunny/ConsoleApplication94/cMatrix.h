#pragma once
#include "Header.h"
class cVector3;

class cMatrix
{
private:
	class cRow
	{
	public:
		cRow() {}
		~cRow() {}

		vector<GLfloat> m_vecCol;

		float& operator[] (int nIndex)
		{
			return m_vecCol[nIndex];
		}

		void Resize(int nDimension)
		{
			m_vecCol.resize(nDimension);
		}

		void Print()
		{
			for (size_t i = 0; i < m_vecCol.size(); ++i)
			{
				printf("%6.2f", m_vecCol[i]);
			}
			printf("\n");
		}
	};

private:
	vector<cRow> m_vecRow;

public:
	cMatrix();
	cMatrix(int nDimention);
	~cMatrix();

	cRow& operator[](int nIndex);
	cMatrix operator+(cMatrix& mat);
	cMatrix operator-(cMatrix& mat);
	cMatrix operator*(float f);

	static cMatrix Identity(int nDimension);

	static cMatrix Scaling(GLfloat fScale);
	static cMatrix Translation(GLfloat x,  GLfloat y, GLfloat z);
	static cMatrix RotateY(GLfloat fAngle);
	static cMatrix RotateX(GLfloat fAngle);
	static cMatrix RotateZ(GLfloat fAngle);

private:
	void Resize(int nDimension);
	int Dimension();

};

