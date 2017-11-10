#include "Header.h"


enum STATE
{
	POINT1,
	LINES,
	TRIANGLES,
	LINETRIANGLE
};

static bool mouseLeftDown;
static int PointNum;
static int VerticeNum;
static float point[2][2];
static int Index[3000];
static float theta = 0;
GLfloat vertices[1000][3];
GLfloat colors[1000][3];
int state = POINT1;



void timer(int value)
{
	theta += 2.0f;
	if (theta >= 360.0)
		theta -= 360.00;

	glutPostRedisplay();
	glutTimerFunc(1000 / 30, timer, 1);
}





void init()
{
	theta = 0.0f;

	point[0][0] = 0; 
	point[0][1] = 0;
	point[1][0] = 0;
	point[1][1] = 0;
	glutTimerFunc(1000 / 30, timer, 1);
	
}



//only point
void pointRenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

	glBegin(GL_POINTS);

	for (int i = 0; i < VerticeNum * 3; i++)
	{
		glColor3f(0, 0, 0);    glVertex3fv(vertices[Index[i] - 1]);

	}
	glEnd();

	glutSwapBuffers();
}


//only Line
void lineRenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

	
	for (int i = 0; i < VerticeNum * 3; i+=3)
	{
		glBegin(GL_LINE_LOOP);
		glColor3f(0, 0, 0);    glVertex3f(vertices[Index[i] - 1][0], vertices[Index[i] - 1][1], vertices[Index[i] - 1][2]);
		glColor3f(0, 0, 0);    glVertex3f(vertices[Index[i+1] - 1][0], vertices[Index[i+1] - 1][1], vertices[Index[i+1] - 1][2]);
		glColor3f(0, 0, 0);    glVertex3f(vertices[Index[i+2] - 1][0], vertices[Index[i+2] - 1][1], vertices[Index[i+2] - 1][2]);
		glEnd();
	}

	glutSwapBuffers();
}

//Only Triangle
void triangleRenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);
	//glPolygonOffset 사용법 출처 :
	////http://neoplanetz.tistory.com/entry/openGL-%ED%85%8D%EC%8A%A4%EC%B3%90%EC%97%90-%EC%99%B8%EA%B3%BD%EC%84%A0-%EA%B7%B8%EB%A6%AC%EA%B8%B0Draw-outline-on-texture-using-glPolygonOffSet
	glPolygonOffset(1.f, 1.f);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBegin(GL_TRIANGLES);


	for (int i = 0; i < VerticeNum * 3; i++)
	{
		glColor3fv(colors[Index[i] - 1]);    glVertex3fv(vertices[Index[i] - 1]);

	}
	glEnd();

	glPolygonOffset(0.f, 0.f);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glLineStipple(3, 0xcccc);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_TRIANGLES);
	//quad #1
	for (int i = 0; i < VerticeNum * 3; i++)
	{
		glColor3f(0, 0, 0);    glVertex3fv(vertices[Index[i] - 1]);
	}

	glEnd();
	glDisable(GL_POLYGON_OFFSET_LINE);

	glutSwapBuffers();
}

// Triangle + Line
void triangleLineRenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

	glPolygonOffset(1.f, 1.f);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBegin(GL_TRIANGLES);

	for (int i = 0; i < VerticeNum * 3; i++)
	{
		glColor3fv(colors[Index[i] - 1]);    glVertex3fv(vertices[Index[i] - 1]);

	}
	glEnd();

	glPolygonOffset(0.f, 0.f);
	glEnable(GL_POLYGON_OFFSET_LINE );
	
	glLineStipple(0.5f, 0xcccc);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (int i = 0; i < VerticeNum * 3; i += 3)
	{
		glBegin(GL_LINE_LOOP);
		glColor3f(0, 0, 0);    glVertex3f(vertices[Index[i] - 1][0], vertices[Index[i] - 1][1], vertices[Index[i] - 1][2]);
		glColor3f(0, 0, 0);    glVertex3f(vertices[Index[i + 1] - 1][0], vertices[Index[i + 1] - 1][1], vertices[Index[i + 1] - 1][2]);
		glColor3f(0, 0, 0);    glVertex3f(vertices[Index[i + 2] - 1][0], vertices[Index[i + 2] - 1][1], vertices[Index[i + 2] - 1][2]);
		glEnd();
	}

	glDisable(GL_POLYGON_OFFSET_LINE);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a' :
		state = 0;
		break;
	case 'b':
		state = 1;
		break;
	case 'c':
		state = 2;
		break;
	case 'd':
		state = 3;
		break;
	default:
		break;
	}

	switch (state)
	{
	case 0:
		glutDisplayFunc(pointRenderScene);
		break;
	case 1:
		glutDisplayFunc(lineRenderScene);
		break;
	case 2:
		glutDisplayFunc(triangleRenderScene);
		break;
	case 3:
		glutDisplayFunc(triangleLineRenderScene);
		break;
	}
}





void FileIO()
{
	FILE* fp;

	char* Str =  NULL;
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
		for (int j = 0; j < 3; j++)
		{
			fscanf_s(fp, "%f", &vertices[i][j]);
			vertices[i][j] *= 10;
			colors[i][0] = 0.3f;
			colors[i][1] = 0.3f;
			colors[i][2] = 0.3f;
		}

	}
	for (int i = 0; i < VerticeNum * 3; i++)
	{
		fscanf_s(fp, "%d", &Index[i]);
	}

	fclose(fp);
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

	//aspectRatio = (GLfloat)w / (GLfloat)h;

	//if (w <= h)
	//{
	///*	float windowWidth = 100;
	//	float windowHeight = 100 / aspectRatio;
	//	glOrtho(-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);*/

	//	//gluOrtho2D(0, 500, 0, 500 * (float)h / (float)w);

	//	glOrtho(-2.0f, 2.0, -2.0 * (float)h / (float)w, 2.0* (float)h / (float)w, -10.0, 10.0);
	//}
	//else
	//{
	//	/*float windowWidth = 100 * aspectRatio;
	//	float windowHeight = 100;
	//	glOrtho(-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);*/
	//	//gluOrtho2D(0, 500 * (float)2 / (float)h, 0, 500);

	//	glOrtho(-2.0f * (float)w / (float)h, 2.0f * (float)w / (float)h,-2.0f, 2.0f, -10.0f, 10.0f);
	//}


}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	FileIO();
	
	glutInitWindowSize(600, 600);
	glutCreateWindow("CG 2013");

	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(keyboard);

	glutDisplayFunc(pointRenderScene);


	init();
	SetupRC();
	glutMainLoop();
}