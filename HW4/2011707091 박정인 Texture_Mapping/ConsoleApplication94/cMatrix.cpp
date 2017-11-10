
#include "cMatrix.h"
#include "cVector3.h"

cMatrix::cMatrix()
{
}

cMatrix::cMatrix(int nDimension)
{
	Resize(nDimension);
}

cMatrix::~cMatrix(void)
{
}

//���ڸ� �޾Ƽ� ���Ϳ� ����
cMatrix::cRow& cMatrix::operator[](int nIndex)
{
	return m_vecRow[nIndex];
}

//��Ʈ������ ������ ũ�⸸ŭ ������¡
void cMatrix::Resize(int nDimension)
{
	m_vecRow.resize(nDimension);
	for (int i = 0; i < nDimension; ++i)
	{
		m_vecRow[i].Resize(nDimension);
	}
}

//ũ�⸦ �޾� Row�� ��ȯ
int cMatrix::Dimension()
{
	return m_vecRow.size();
}

//������ ũ�⸸�� ��Ʈ������ �����
//Identity��Ʈ������ �ʱ�ȭ
cMatrix cMatrix::Identity(int nDimension)
{
	cMatrix ret(nDimension);
	for (int i = 0; i < nDimension; ++i)
	{
		for (int j = 0; j < nDimension; ++j)
		{
			ret[i][j] = (i == j) ? 1.f : 0.f;
		}
	}
	return ret;
}

cMatrix cMatrix::Scaling(GLfloat fScale)
{
	cMatrix matRet = cMatrix::Identity(4);

	matRet[0][0] *= fScale;
	matRet[1][1] *= fScale;
	matRet[2][2] *= fScale;

	return matRet;

}



cMatrix cMatrix::operator+(cMatrix& mat)
{
	cMatrix ret(Dimension());
	for (int i = 0; i < Dimension(); ++i)
	{
		for (int j = 0; j < Dimension(); ++j)
		{
			ret[i][j] = (*this)[i][j] + mat[i][j];
		}
	}
	return ret;
}


cMatrix cMatrix::operator-(cMatrix& mat)
{
	cMatrix ret(Dimension());
	for (int i = 0; i < Dimension(); ++i)
	{
		for (int j = 0; j < Dimension(); ++j)
		{
			ret[i][j] = (*this)[i][j] - mat[i][j];
		}
	}
	return ret;
}

//��Į��*����
//������ ���ڿ� ��Į�� ���� ����
cMatrix cMatrix::operator*(float f)
{
	cMatrix ret(Dimension());
	for (int i = 0; i < Dimension(); ++i)
	{
		for (int j = 0; j < Dimension(); ++j)
		{
			ret[i][j] = (*this)[i][j] * f;
		}
	}
	return ret;
}



cMatrix cMatrix::Translation(GLfloat x, GLfloat y, GLfloat z)
{
	cMatrix mat = cMatrix::Identity(4);
	mat[0][3] = x;
	mat[1][3] = y;
	mat[2][3] = z;

	return mat;
}

cMatrix cMatrix::RotateY(GLfloat fAngle)
{
	cMatrix matRet = cMatrix::Identity(4);
	matRet[0][0] = cosf(fAngle);
	matRet[0][2] = sinf(fAngle);
	matRet[2][0] = -sinf(fAngle);
	matRet[2][2] = cosf(fAngle);
	return matRet;
}

cMatrix cMatrix::RotateX(GLfloat fAngle)
{
	cMatrix matRet = cMatrix::Identity(4);
	matRet[1][1] = cosf(fAngle);
	matRet[1][2] = -sinf(fAngle);
	matRet[2][1] = sinf(fAngle);
	matRet[2][2] = cosf(fAngle);
	return matRet;
}

cMatrix cMatrix::RotateZ(GLfloat fAngle)
{
	cMatrix matRet = cMatrix::Identity(4);
	matRet[0][0] = cosf(fAngle);
	matRet[0][1] = -sinf(fAngle);
	matRet[1][0] = sinf(fAngle);
	matRet[1][1] = cosf(fAngle);
	return matRet;
}