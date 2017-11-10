#include "Header.h"
#include "Util.h"
#include "cMatrix.h"
#include "cVector3.h"

#define PI 3.14159
#define PLUS_U (float)1 / (float)18;
#define PLUS_V (float)1 / (float)36;
#define PLUS_Z 20
#define PLUS_Y 10
#define TEXTURE_NUM 6
#define Z_START 270

enum State
{
	SPOT = 0,
	DIR_POINT
};

cVector3 DirPos = cVector3(0, 3, 0);
GLubyte m_image[64][64][3];

GLuint texID[6];
const char* FilePath[6] = { "Material/wood.raw","Material/marble.raw","Material/donut.raw","Material/check.raw","Material/logo2.raw","Material/grayscale_ornament.raw" };
static float thetaZ;
static float thetaY;
static float CameraTheta;
static float CameraThetaPlus;

vector<cVector3*> vDonutPt[37];  // µµ³Ó Æ÷ÀÎÅÍ ³ÖÀ» º¤ÅÍ
vector<floatUV*> vDonutUV[37];  // µµ³Ó UV °ª ³ÖÀ» º¤ÅÍ
vector<int> Index;  //  µµ³ÓÀÇ »ï°¢Çü ÀÎµ¦½º

Util* util = new Util;   // Åä³¢ ±×¸®´Â°Å °ü·Ã ÇÔ¼ö ¹­À½

int state = 1;

void renderXYZ();
void DonutInit();
void LoadTexture();
void setup_textures();

void timer(int value)
{
	glutTimerFunc(1000 / 90, timer, 1);
	glutPostRedisplay();
	CameraTheta += CameraThetaPlus;
}

void init()
{
	CameraThetaPlus = 0.5f;
	CameraTheta = 0;
	thetaZ = Z_START;
	thetaY = 0;
	glutTimerFunc(1000 / 90, timer, 1);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_TEXTURE_2D);
	setup_textures();
}

void LoadTexture()
{

}

void setup_textures()
{

	int width, height;
	FILE * file;
	BYTE* data[6];
	width = 512;
	height = 512;
	glGenTextures(6, &texID[0]);

	for (int i = 0; i < TEXTURE_NUM; i++)
	{

		fopen_s(&file, FilePath[i], "rb");
		if (file == NULL) return;

		data[i] = (BYTE*)malloc(width * height * 3);

		fread(data[i], width * height * 3, 1, file);
		fclose(file);

		glBindTexture(GL_TEXTURE_2D, texID[i]);
		if( i >= 4 )
		glTexImage2D(GL_TEXTURE_2D, 0,3, 512, 512, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data[i]);
		else glTexImage2D(GL_TEXTURE_2D, 0, 3, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, data[i]);

		
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		free(data[i]);
	}
	return;


}


//  ±×·ÁÁÖ´Â ÇÔ¼ö.
void glDrawDonut()
{
	renderXYZ();

	glShadeModel(GL_SMOOTH);
	for (int j = 0; j < 18; j++)
	for (int i = 0; i < 36; i++)
		{
			glBindTexture(GL_TEXTURE_2D, texID[state]);
			util->SmoothDrawQuad(vDonutPt[i][j]->pt, vDonutPt[i + 1][j]->pt, vDonutPt[i][j + 1]->pt, vDonutPt[i + 1][j + 1]->pt,
		                                 	vDonutUV[i][j], vDonutUV[i+1][j], vDonutUV[i][j + 1], vDonutUV[i+1][j + 1]);
	     }

}

void DirPosLightRenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	cVector3 vec;
	vec.pt[0] = 7;
	vec.pt[1] = 7;
	vec.pt[2] = 7;
	vec = vec.Transform(cMatrix::RotateY((CameraTheta * PI) / 180));

	gluLookAt(vec.pt[0], vec.pt[1], vec.pt[2], 0, 0, 0, 0, 1, 0);

	GLfloat ambient1[4] = { 0.1,0.1,0.1,1 };
	GLfloat diffuse1[4] = { 1,1,1,1 };
	GLfloat specular1[4] = { 1,1,1,1 };


	glPolygonOffset(1.f, 1.f);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	

	glLightfv(GL_LIGHT0, GL_POSITION, DirPos.pt);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular1);

	glDrawDonut();


	glutSwapBuffers();
}
// Flat Shader + Light

//
// Smooth Shader + Light



void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 's':
		state = 0;
		break;
	case 't':
		state = 1;
		break;
	case 'u':
		state = 2;
		break;
	case 'v':
		state = 3;
		break;
	case 'w':
		state = 4;
		break;
	case 'x':
		state = 5;
		break;
	case 'e':
		if (CameraThetaPlus >= 0.4f)
		{
			CameraThetaPlus = 0;
		}
		else
		{
			CameraThetaPlus = 0.5;
		}
		break;
	default:
		break;
	}
}




void SetupRC()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);


}

void ChangeSize(int w, int h)
{
	GLfloat aspectRatio;
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glFrustum(-5, 5, -5, 5, 5, 100);

}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(600, 600);
	glutCreateWindow("CG 2013");

	DonutInit();

	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(keyboard);

	glutDisplayFunc(DirPosLightRenderScene);
	glutTimerFunc(1000/90 , timer, 1);

	init();
	SetupRC();	
	glutMainLoop();

}


void renderXYZ()
{

	point3 Origin;
	Origin[0] = 0;  Origin[1] = -30; Origin[2] = 0;

	point3 xAxis;
	xAxis[0] = 110;  xAxis[1] = 0;  xAxis[2] = 0;
	
	point3 yAxis;
	yAxis[0] = 0;  yAxis[1] = 110;  yAxis[2] = 0;

	point3 zAxis;
	zAxis[0] = 0;  zAxis[1] = 0;  zAxis[2] = 110;

	//util->DrawLine(xAxis, Origin);
	util->DrawLine(yAxis, Origin);
//	util->DrawLine(zAxis, Origin);

}

void DonutInit()
{
	float U = 0;
	float V = 0;

	for (int i = 0; i < 37; i++)
	{
		cVector3* pt = new cVector3[19];
		floatUV* ptUV = new floatUV[19];

		U = 0;
		thetaZ = Z_START;

		for (int j = 0; j < 19; j++)
		{
			pt[j] = pt[j].Transform(cMatrix::RotateZ((thetaZ * PI) / 180));
			pt[j] = pt[j].Transform(cMatrix::RotateY((thetaY * PI) / 180));
			pt[j] = pt[j].Transform(cMatrix::Translation(cosf((thetaY * PI) / 180) *3, 0, -sinf((thetaY * PI) / 180) * 3));		//	pt[j] = pt[j].Transform(cMatrix::Translation(2, 2, 2));

			thetaZ += PLUS_Z;
			vDonutPt[i].push_back(&pt[j]);

			ptUV[j].U = U;
			ptUV[j].V = V;

			U += PLUS_U;
			vDonutUV[i].push_back(&ptUV[j]);
		}

		V += PLUS_V;
		thetaY += PLUS_Y;

	}
}


