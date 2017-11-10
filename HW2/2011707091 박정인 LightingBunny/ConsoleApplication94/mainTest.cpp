#include "Header.h"
#include "Util.h"
#include "cMatrix.h"
#include "cVector3.h"
#define PLUS 1
#define PI 3.14159
#define MAX_CUTOFF 25
#define MIN_CUTOFF 5
#define MAX_SHINY 100
#define MIN_SHINY 0

enum State
{
	SPOT = 0,
	DIR_POINT
};

cVector3 SpotPos = cVector3( -10,-5,-10 );
cVector3 PointPos = cVector3(5,5,5,1);
cVector3 DirPos = cVector3(5, 5, 5);
//cVector3 SpotPos = cVector3( -1,-10,-10,1 );

static bool isSpot = false;
static bool isPoint = true;
static bool isDirect = false;

//static bool mouseLeftDown;
static int PointNum;
static int VerticeNum;

static float spotShiness = MIN_SHINY; // Spot 각도 
static float spotAngle = MIN_CUTOFF; // Spot 각도 
static float theta; // Light 각도
static float thetaPlus = PLUS; // Light 각도

vector<cVector3*> vBunnyPt;  // 토끼 포인터 넣을 벡터
vector<GLfloat*> vColor;  // 토끼 색깔 넣을 벡터
vector<int> Index;  //  토끼의 삼각형 인덱스

Util* util = new Util;   // 토끼 그리는거 관련 함수 묶음

int state = 1;

static bool spotAngleChange = false;
static bool spotAngleMinus = false;

static bool spotShinessOn = false;
static bool spotShinessMinus = false;

void renderXYZ();
void spotCutOffChange();
void spotShinessChange();
void SpotLightOn();
void FileIO();

void timer(int value)
{
	theta = thetaPlus;
	if (theta >= 360.0)
		theta -= 360.00;

	DirPos = DirPos.Transform(cMatrix::RotateY((theta * PI) / 180));
	PointPos = PointPos.Transform(cMatrix::RotateY((theta * PI) / 180));
	SpotPos = SpotPos.Transform(cMatrix::RotateY((theta * PI) / 180));

	spotCutOffChange();
	spotShinessChange();

	glutTimerFunc(1000 / 90, timer, 1);
	glutPostRedisplay();
}

void init()
{
	theta = thetaPlus;
	glutTimerFunc(1000 / 90, timer, 1);
}

// 바니 그려주는 함수.
void glDrawBunny()
{
	renderXYZ();

	glColor3f(1, 0, 0);
	//glutSolidTeapot(5.0);

	glShadeModel(GL_SMOOTH);
	//glutSolidTeapot(1.0);
	for (int i = 0; i < VerticeNum * 3; i += 3)
	{
		util->SmoothDrawTriangle(vBunnyPt[Index[i] - 1]->pt, vBunnyPt[Index[i + 1] - 1]->pt, vBunnyPt[Index[i + 2] - 1]->pt);
	}

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glPolygonOffset(0.f, 0.f);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBegin(GL_TRIANGLES);
		//quad #1
		for (int i = 0; i < VerticeNum * 3; i++)
		{
			glColor3f(0, 0, 0);
			glVertex3f(vBunnyPt[Index[i] - 1]->pt[0], vBunnyPt[Index[i] - 1]->pt[1], vBunnyPt[Index[i] - 1]->pt[2]);
		}

		glEnd();

		glDisable(GL_POLYGON_OFFSET_LINE);
}

void SpotLightOn()
{
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, SpotPos.pt);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spotAngle);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, spotShiness);

}


void SpotLightRenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);


	GLfloat position[4] = { 5,5,5,1 };
	GLfloat ambient1[4] = { 0.1,0.1,0.1,1 };
	GLfloat diffuse1[4] = { 1,1,1,1 };
	GLfloat specular1[4] = { 1,1,1,1 };


	glPolygonOffset(1.f, 1.f);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT1, GL_POSITION, PointPos.pt);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);

	SpotLightOn();
	//바니 그린다.
	glDrawBunny();


	glutSwapBuffers();
}
void DirPosLightRenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

	GLfloat ambient1[4] = { 0.1,0.1,0.1,1 };
	GLfloat diffuse1[4] = { 1,1,1,1 };
	GLfloat specular1[4] = { 1,1,1,1 };


	glPolygonOffset(1.f, 1.f);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	if (isDirect)
	{
		glLightfv(GL_LIGHT0, GL_POSITION, DirPos.pt);
	}
	if (isPoint)
	{
		glLightfv(GL_LIGHT0, GL_POSITION, PointPos.pt);
	}

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular1);

	//바니 그린다.
	glDrawBunny();


	glutSwapBuffers();
}
// Flat Shader + Light

//
// Smooth Shader + Light



void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{

	case 'p':
		state = DIR_POINT;
		isSpot = false;
		 isPoint = true;
		 isDirect = false;
			break;
	case 's':
		state = SPOT;
		isSpot = true;
		isPoint = false;
		isDirect = false;
		break;
	case 'd':
		state = DIR_POINT;
		isSpot = false;
		isPoint = false;
		isDirect = true;
		break;
	case 'c':
		spotAngleChange = !spotAngleChange;
		break;
	case 'n':
		spotShinessOn = !spotShinessOn;
		break;
	case 'e':
		if (thetaPlus <= PLUS && thetaPlus != 0)
		{
			thetaPlus = 0;
		}
		else if (thetaPlus == 0)
			thetaPlus = PLUS;
		break;
	default:
		break;
	}

	switch (state)
	{
	case SPOT:
		glutDisplayFunc(SpotLightRenderScene);
		break;
	case DIR_POINT:
		glutDisplayFunc(DirPosLightRenderScene);
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

	FileIO();

	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(keyboard);

	glutDisplayFunc(DirPosLightRenderScene);
	glutTimerFunc(1000/90 , timer, 1);

	init();
	SetupRC();	
	glutMainLoop();

}


void spotCutOffChange()
{
	if (spotAngleChange)
	{
		if (spotAngle > MAX_CUTOFF)
		{
			spotAngleMinus = true;
		}
		else if (spotAngle < MIN_CUTOFF)
		{
			spotAngleMinus = false;
		}

		if (spotAngleMinus)
		{
			spotAngle -= 0.5f;
		}
		else
		{
			spotAngle += 0.5f;
		}
	}
}

void spotShinessChange()
{
	if (spotShinessOn)
	{
		if (spotShiness > MAX_SHINY)
		{
			spotShinessMinus = true;
		}
		else if (spotShiness < MIN_SHINY)
		{
			spotShinessMinus = false;
		}

		if (spotShinessMinus)
		{
			spotShiness -= 0.5f;
		}
		else
		{
			spotShiness += 0.5f;
		}
	}
}

void renderXYZ()
{
	point3 LightPos;
	point3 Origin;
	Origin[0] = 0;  Origin[1] = 0; Origin[2] = 0;

	LightPos[0] = PointPos.pt[0];  LightPos[1] = PointPos.pt[1]; LightPos[2] = PointPos.pt[2];

	point3 xAxis;
	xAxis[0] = 110;  xAxis[1] = 0;  xAxis[2] = 0;
	
	point3 yAxis;
	yAxis[0] = 0;  yAxis[1] = 110;  yAxis[2] = 0;

	point3 zAxis;
	zAxis[0] = 0;  zAxis[1] = 0;  zAxis[2] = 110;

	util->DrawLine(LightPos, Origin);
	util->DrawLine(xAxis, Origin);
	util->DrawLine(yAxis, Origin);
	util->DrawLine(zAxis, Origin);

}

void FileIO()
{
	FILE* fp;
	char* Str = NULL;
	Str = new char[255];
	int i = 0;

	// Vertice Num과 Point Num을 파일에서 가져오자!
	if (fopen_s(&fp, "bunny.txt", "r") == NULL)
	{
		fscanf_s(fp, "%d", &PointNum);
		fscanf_s(fp, "%d", &VerticeNum);
	}
	else
	{
		puts("File Open Error");
		exit(0);
	}

	for (int i = 0; i < PointNum; i++)
	{
		GLfloat* pt = new GLfloat[3];
		GLfloat* color = new GLfloat[3];

		for (int j = 0; j < 3; j++)
		{
			fscanf_s(fp, "%f", &pt[j]);
			pt[j] *= 10;
			color[j] = 0.3f;
		}

		cVector3* vec = new cVector3(pt[0], pt[1], pt[2]);
		vBunnyPt.push_back(vec);
		vColor.push_back(color);
	}


	for (int i = 0; i < VerticeNum * 3; i++)
	{
		int tmpIdx;
		fscanf_s(fp, "%d", &tmpIdx);
		Index.push_back(tmpIdx);
	}

	fclose(fp);

}


