#include "Header.h"
#include "Util.h"


enum STATE
{
	POINT1,
	LINES,
	TRIANGLES,
	LINETRIANGLE
};
bool isWireOn;
static bool mouseLeftDown;
static int PointNum;
static int VerticeNum;
static float point[2][2];
static int Index[3000];
static float theta = 0;

//중복되는 검색을 허용하는 멀티맵을 이용하여 구현하고자 함.
multimap<int, int*> mm;
multimap<int, int*>::iterator iter;

multimap<int, GLfloat*> FNM;
multimap<int, GLfloat*>::iterator FNiM;

vector<GLfloat*> vVertexNormal;


static point3 bunnyPoint[1000];
static point3 bunnyFaceNormal[3000];


point3 bunnyVertexNormal[3000];


GLfloat colors[1000][3];
//int* TriIndex;
Util util;

int state = POINT1;


void InitVertexMap()
{
	for (int i = 0; i < PointNum; i++)
	{
		pair<map<int, GLfloat*>::iterator, map<int, GLfloat*>::iterator> iter_pair;

		iter_pair = FNM.equal_range(i);

		int num = 0;
		point3 tempPt = { 0,0,0 };
		for (FNiM = iter_pair.first; FNiM != iter_pair.second; ++FNiM)
		{
			point3 pt;
			pt[0] = FNiM->second[0];
			pt[1] = FNiM->second[1];
			pt[2] = FNiM->second[2];

			tempPt[0] += pt[0];
			tempPt[1] += pt[1];
			tempPt[2] += pt[2];

			num++;
		}
		tempPt[0] /= num;
		tempPt[1] /= num;
		tempPt[2] /= num;

		util.Normalize(tempPt);

		tempPt[0] += bunnyPoint[i][0];
		tempPt[1] += bunnyPoint[i][1];
		tempPt[2] += bunnyPoint[i][2];

		GLfloat* VectorPt = new GLfloat[3];

		VectorPt[0] = tempPt[0];
		VectorPt[1] = tempPt[1];
		VectorPt[2] = tempPt[2];

		vVertexNormal.push_back(VectorPt);
		//util.DrawLine(tempPt, bunnyPoint[i]);

	}
}


void IndexInsert(int* Index)
{
	int* TriIndex = new int[3];
	TriIndex[0] = Index[0];
	TriIndex[1] = Index[1];
	TriIndex[2] = Index[2];

	mm.insert(make_pair(Index[0], TriIndex));
	mm.insert(make_pair(Index[1], TriIndex));
	mm.insert(make_pair(Index[2], TriIndex));
}

void NormalIndexInsert(int* Index, GLfloat* xyz)
{
	GLfloat* TriIndex = new GLfloat[3];

	TriIndex[0] = Index[0];
	TriIndex[1] = Index[1];
	TriIndex[2] = Index[2];

	FNM.insert(make_pair(Index[0], xyz));
	FNM.insert(make_pair(Index[1], xyz));
	FNM.insert(make_pair(Index[2], xyz));
}


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

	isWireOn = true;
	
}

void glLightinit(GLfloat* position, GLfloat* ambient, GLfloat* diff, GLfloat* spec)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	int n = 3;

	glLightfv(GL_LIGHT0, GL_POSITION, position);

}

void glDrawBunny()
{

//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (isWireOn)
	{
		glBegin(GL_TRIANGLES);
		//quad #1
		for (int i = 0; i < VerticeNum * 3; i++)
		{
			glColor3f(0, 0, 0);
			glVertex3fv(bunnyPoint[Index[i] - 1]);
		}

		glEnd();
	}
//	glDisable(GL_POLYGON_OFFSET_LINE);
}


////


// Flat Shader + Light
void FlatLightRenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(7, 7, 7, 0, 0, 0, 0, 1, 0);

	GLfloat position1[4] = { 10,10,10,1 };
	GLfloat ambient1[4] = { 0.1,0.1,0.1,1 };
	GLfloat diffuse1[4] = { 1,1,1,1 };
	GLfloat specular1[4] = { 1,1,1,1 };

	glPolygonOffset(1.f, 1.f);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_NORMALIZE);
	glLightinit(position1, ambient1, diffuse1, specular1);

	//	glShadeModel(GL_SMOOTH);
	//glutSolidTeapot(1.0);
	for (int i = 0; i < VerticeNum * 3; i += 3)
	{
		util.SmoothDrawTriangle(bunnyPoint[Index[i] - 1], bunnyPoint[Index[i + 1] - 1], bunnyPoint[Index[i + 2] - 1]);

	}
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glPolygonOffset(0.f, 0.f);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (int i = 0; i < VerticeNum * 3; i += 3)
	{
		util.DrawFaceNormal(bunnyPoint[Index[i] - 1], bunnyPoint[Index[i + 1] - 1], bunnyPoint[Index[i + 2] - 1]);
	}

	glDrawBunny();

	glDisable(GL_POLYGON_OFFSET_LINE);
	glutSwapBuffers();
}
//
// Smooth Shader + Light
void SmoothLightRenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(7, 7, 7, 0, 0, 0, 0, 1, 0);

	GLfloat position1[4] = { 10,10,10,1 };
	GLfloat ambient1[4] = { 0.1,0.1,0.1,1 };
	GLfloat diffuse1[4] = { 1,1,1,1 };
	GLfloat specular1[4] = { 1,1,1,1 };

	glPolygonOffset(1.f, 1.f);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_NORMALIZE);
	glLightinit(position1, ambient1, diffuse1, specular1);

//	glShadeModel(GL_SMOOTH);
	//glutSolidTeapot(1.0);
	for (int i = 0; i < VerticeNum * 3; i += 3)
	{
		util.SmoothDrawTriangle(bunnyPoint[Index[i] - 1], bunnyPoint[Index[i + 1] - 1], bunnyPoint[Index[i + 2] - 1]);
		
	}

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	
	glPolygonOffset(0.f, 0.f);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
	/*for (int i = 0; i < PointNum; i++)
	{
		pair<map<int, GLfloat*>::iterator, map<int, GLfloat*>::iterator> iter_pair;

		iter_pair = FNM.equal_range(i);

		int num = 0;
		point3 tempPt = { 0,0,0 };
		for (FNiM = iter_pair.first; FNiM != iter_pair.second; ++FNiM)
		{
			point3 pt;
			pt[0] = FNiM->second[0];
			pt[1] = FNiM->second[1];
			pt[2] = FNiM->second[2];
			util.DrawLine(pt, bunnyPoint[i]);
		}
	}*/

	for (int i = 0; i < PointNum; i++)
	{
		point3 pt;
		pt[0] = vVertexNormal[i][0];
		pt[1] = vVertexNormal[i][1];
		pt[2] = vVertexNormal[i][2];

		util.DrawLine(pt, bunnyPoint[i]);

	}
	glDrawBunny();

	glDisable(GL_POLYGON_OFFSET_LINE);

		glutSwapBuffers();

}


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'f' :
		state = 0;
		break;
	case 'v':
		state = 1;
		break;
	case 'w':
		isWireOn = !isWireOn;
		break;
	default:
		break;
	}

	switch (state)
	{
	case 0:
		glutDisplayFunc(FlatLightRenderScene);
		break;
	case 1:
		glutDisplayFunc(SmoothLightRenderScene);
		break;
	default:
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
			fscanf_s(fp, "%f", &bunnyPoint[i][j]);
			bunnyPoint[i][j] *= 10;
			
			colors[i][0] = 0.3f;
			colors[i][1] = 0.3f;
			colors[i][2] = 0.3f;
		}		
	}

	int FaceNumIdx = 0;
	for (int i = 0; i < VerticeNum * 3; i++)
	{
		fscanf_s(fp, "%d", &Index[i]);

		
		// 삼각형 마다 Face Normal Vector를 계산하고 저장하자.
		if (i % 3 == 0 && i != 0)
		{
			// 삼각형의 인덱스를 옮겨 담는다.
			int TriIndex[3];
			TriIndex[0] = Index[i - 3] - 1;
			TriIndex[1] = Index[i - 2] - 1;
			TriIndex[2] = Index[i - 1] - 1;

			IndexInsert(TriIndex);

			//삼각형 3점을 Cross 하여 Bunny의 Face Normal값을 저장함.
			util.Cross(bunnyPoint[Index[i - 3] - 1], bunnyPoint[Index[i - 2] - 1], bunnyPoint[Index[i - 1] - 1], bunnyFaceNormal[FaceNumIdx]);
			
			// 삼각형 3점의 가운데를 찾기 위해 pt 변수를 만들고 저장한다.
			GLfloat* pt = util.CalPointAvg(bunnyPoint[Index[i - 3] - 1], bunnyPoint[Index[i - 2] - 1], bunnyPoint[Index[i - 1] - 1]);

			// Bunny의 표면 법선 벡터에 삼각형의 위치를 더하여 원래 자리로 돌아가도록 한다.
			bunnyFaceNormal[FaceNumIdx][0] += pt[0];
			bunnyFaceNormal[FaceNumIdx][1] += pt[1];
			bunnyFaceNormal[FaceNumIdx][2] += pt[2];

			// 이제 위에서 구한 bunnyFaceNormal를 MultiMap 컨테이너에 넣어서 인접한 삼각형 순서로 되게 저장한다.
			NormalIndexInsert(TriIndex, bunnyFaceNormal[FaceNumIdx]);

			FaceNumIdx++;
		}
	}

	InitVertexMap();

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

}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(600, 600);
	glutCreateWindow("CG 2013");

	FileIO();
	util.SetVertexInfo(bunnyPoint, Index);


	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(keyboard);

	glutDisplayFunc(FlatLightRenderScene);
		


	init();
	SetupRC();	
	glutMainLoop();

}

