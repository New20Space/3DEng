#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <string.h>
#include <vector>
#include <fstream>
#include <strstream>
#include <algorithm>


#include <GL/freeglut.h>




#define SW         300                  //screen width
#define SH         300                  //screen height
#define SW2        (SW/2)                   //half of screen width
#define SH2        (SH/2)                   //half of screen height
#define pixelScale 2                    //OpenGL pixel scale
#define GLSW       (SW*pixelScale)          //OpenGL window width
#define GLSH       (SH*pixelScale)          //OpenGL window height
//------------------------------------------------------------------------------
typedef struct
{
	int fr1, fr2;           //frame 1 frame 2, to create constant frame rate
}time1; time1 T;

typedef struct
{
	int w, s, a, d;           //move up, down, left, right
	int sl, sr;             //strafe left, right 
	int m;                 //move up, down, look up, down
}keys; keys K;
//------------------------------------------------------------------------------

void pixel(int x, int y, int c)                  //draw a pixel at x/y with rgb
{
	int rgb[3];
	if (c == 0) { rgb[0] = 255; rgb[1] = 255; rgb[2] = 0; } //Yellow	
	if (c == 1) { rgb[0] = 160; rgb[1] = 160; rgb[2] = 0; } //Yellow darker	
	if (c == 2) { rgb[0] = 0; rgb[1] = 255; rgb[2] = 0; } //Green	
	if (c == 3) { rgb[0] = 0; rgb[1] = 160; rgb[2] = 0; } //Green darker	
	if (c == 4) { rgb[0] = 0; rgb[1] = 255; rgb[2] = 255; } //Cyan	
	if (c == 5) { rgb[0] = 0; rgb[1] = 160; rgb[2] = 160; } //Cyan darker
	if (c == 6) { rgb[0] = 160; rgb[1] = 100; rgb[2] = 0; } //brown	
	if (c == 7) { rgb[0] = 110; rgb[1] = 50; rgb[2] = 0; } //brown darker
	if (c == 8) { rgb[0] = 0; rgb[1] = 60; rgb[2] = 130; } //background 
	glColor3ub(rgb[0], rgb[1], rgb[2]);
	glBegin(GL_POINTS);
	glVertex2i(x * pixelScale + 2, y * pixelScale + 2);
	glEnd();
}

using namespace std;


struct vec3d
{
	float x, y, z;
};

struct triangle
{
	vec3d p[3];
	float light;
};

struct mesh
{
	vector<triangle> tris;
	bool Load(string sFilename)
	{
		ifstream f(sFilename);
		if (!f.is_open())
			return false;

		// Local cache of verts
		vector<vec3d> verts;

		while (!f.eof())
		{
			char line[228];
			f.getline(line, 228);

			strstream s;
			s << line;

			char junk;
			int junk1;


			if ((line[0] == 'v')&&(line[1] == ' '))
			{
				vec3d v{0,0,0};
				line[0] = ' ';
				line[1] = ' ';
				s >> junk >> v.x >> v.y >> v.z;
				
				verts.push_back(v);
			}
			
			if (line[0] == 'f')
			{
				int f1[4];
				line[0] = ' ';
				s >> junk >> f1[0] >> f1[1] >> f1[2];

				//sscanf_s(line, "%i/%i/%i %i/%i/%i %i/%i/%i",
				//	// номер точки номер нормали   номер текстуры
				//	  &f1[0], &junk1, &junk1   //p1
				//	, &f1[1], &junk1, &junk1   //p2
				//	, &f1[2], &junk1, &junk1   //p3
				//);
			
				//cout << line <<"\t " << f1[0] << " " << f1[1] << " " << f1[2] << endl;
				tris.push_back({ verts[f1[0] - 1], verts[f1[1] - 1], verts[f1[2] - 1] });
			}
			
		}

		
		return true;
	}
};
struct mat3x3
{
	float m[3][3] = { 0 };
};
float f = 0.0;
float fThetax = 0.0f, fThetaz = 0.0f;
mat3x3 matRotZ, matRotX;
void movePlayer()
{


	//move up, down, left, right
	if (K.a == 1 && K.m == 0) { fThetax+= 0.1f; printf("left\n"); }
	if (K.d == 1 && K.m == 0) { fThetax -= 0.1f; printf("right\n"); }
	if (K.w == 1 && K.m == 0) { fThetaz += 0.1f; printf("up\n"); }
	if (K.s == 1 && K.m == 0) { fThetaz -= 0.1f; printf("down\n"); }
	//strafe left, right
	if (K.sr == 1) { f += 1; printf("strafe left\n"); }
	if (K.sl == 1) { f -= 1; printf("strafe right\n"); }
	//move up, down, look up, look down
	if (K.a == 1 && K.m == 1) { printf("look up\n"); }
	if (K.d == 1 && K.m == 1) { printf("look down\n"); }
	if (K.w == 1 && K.m == 1) { printf("move up\n"); }
	if (K.s == 1 && K.m == 1) { printf("move down\n"); }



	// Rotation Z
	matRotZ.m[0][0] = cosf(	fThetaz);
	matRotZ.m[0][1] = sinf(	fThetaz);
	matRotZ.m[1][0] = -sinf(fThetaz);
	matRotZ.m[1][1] = cosf(	fThetaz);
	matRotZ.m[2][2] = 1;

	// Rotation X
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cosf( fThetax * 0.5f);
	matRotX.m[1][2] = sinf( fThetax * 0.5f);
	matRotX.m[2][1] = -sinf(fThetax * 0.5f);
	matRotX.m[2][2] = cosf( fThetax * 0.5f);
}

mesh meshCube;
float fNear = 1.0f;
float fFar = 1000.0f;
float fFov = 90.0f;
float fFovRad = tanf(fFov * 0.5f / 180.0f * 3.14159f);

void MultiplyMatrixVector(vec3d& i, vec3d& o, mat3x3& m)
{
	o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] ;
	o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] ;
	o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] ;
}

void draw3D()
{

	vec3d vCamera = { 0.0,0.0,0.0 };
	vec3d vLight = { 0.0,1.0,0.0 +f};
	vec3d vLightd = { 0.0,0.0,1.0 };
	float l = sqrtf(vLightd.x * vLightd.x + vLightd.y * vLightd.y + vLightd.z * vLightd.z);
	vLightd.x /= l; vLightd.y /= l; vLightd.z /= l;

	vector<triangle> vecTri;
	for (auto tri : meshCube.tris)
	{
		triangle triP,triRot;
		

		MultiplyMatrixVector(tri.p[0], triRot.p[0], matRotZ);
		MultiplyMatrixVector(tri.p[1], triRot.p[1], matRotZ);
		MultiplyMatrixVector(tri.p[2], triRot.p[2], matRotZ);

	
		MultiplyMatrixVector(triRot.p[0], tri.p[0], matRotX);
		MultiplyMatrixVector(triRot.p[1], tri.p[1], matRotX);
		MultiplyMatrixVector(triRot.p[2], tri.p[2], matRotX);

		tri.p[0].z = tri.p[0].z + 3.0f;
		tri.p[1].z = tri.p[1].z + 3.0f;
		tri.p[2].z = tri.p[2].z + 3.0f;

		vec3d normal, line1, line2;
		line1.x = tri.p[1].x - tri.p[0].x;
		line1.y = tri.p[1].y - tri.p[0].y;
		line1.z = tri.p[1].z - tri.p[0].z;

		line2.x = tri.p[2].x - tri.p[0].x;
		line2.y = tri.p[2].y - tri.p[0].y;
		line2.z = tri.p[2].z - tri.p[0].z;

		normal.x = line1.y * line2.z - line1.z * line2.y;
		normal.y = line1.z * line2.x - line1.x * line2.z;
		normal.z = line1.x * line2.y - line1.y * line2.x;

	
		float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
		normal.x /= l; normal.y /= l; normal.z /= l;


		if (normal.x * (tri.p[0].x - vCamera.x) +
			normal.y * (tri.p[0].y - vCamera.y) +
			normal.z * (tri.p[0].z - vCamera.z) < 0.0f) {

			float light = (normal.x * (tri.p[0].x - vLight.x+vLightd.x) +
			normal.y * (tri.p[0].y - vLight.y + vLightd.y) +
			normal.z * (tri.p[0].z - vLight.z + vLightd.z))*-0.1;

			for (size_t i = 0; i < 3; i++)
			{
				triP.p[i].x = tri.p[i].x * 500 * fNear / tri.p[i].z;
				triP.p[i].y = tri.p[i].y * 500 * fNear / tri.p[i].z;
				triP.p[i].z = tri.p[i].z;
			}
			triP.light = light;
			vecTri.push_back(triP);
			
		}
	}
	sort(vecTri.begin(), vecTri.end(), [](triangle& t1, triangle& t2)
		{
			float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
			float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
			return z1 > z2;
		});

	for (auto& triP1 : vecTri)
	{
		
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(triP1.light, triP1.light, triP1.light);
		glVertex2f(triP1.p[0].x + 300, triP1.p[0].y + 300);

		glVertex2f(triP1.p[1].x + 300, triP1.p[1].y + 300);

		glVertex2f(triP1.p[2].x + 300, triP1.p[2].y + 300);
		glEnd();
		if (0) {
			glLineWidth(1);
			glBegin(GL_LINE_LOOP);
			glColor3ub(255, 0, 0);
			glVertex2f(triP1.p[0].x + 300, triP1.p[0].y + 300);

			glVertex2f(triP1.p[1].x + 300, triP1.p[1].y + 300);

			glVertex2f(triP1.p[2].x + 300, triP1.p[2].y + 300);
			glEnd();
		}
	}
}

void display()
{
	int x, y;
	if (T.fr1 - T.fr2 >= 50)                        //only draw 20 frames/second
	{
		glClearColor(0.0f,0.0f,0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		movePlayer();
		draw3D();

		T.fr2 = T.fr1;
		glutSwapBuffers();
		glutReshapeWindow(GLSW, GLSH);             //prevent window scaling
	}

	T.fr1 = glutGet(GLUT_ELAPSED_TIME);          //1000 Milliseconds per second
	glutPostRedisplay();
}

void KeysDown(unsigned char key, int x, int y)
{
	if (key == 'w' == 1 or key == 'ц') { K.w = 1; }
	if (key == 's' == 1) { K.s = 1; }
	if (key == 'a' == 1) { K.a = 1; }
	if (key == 'd' == 1) { K.d = 1; }
	if (key == 'm' == 1) { K.m = 1; }
	if (key == ',' == 1) { K.sr = 1; }
	if (key == '.' == 1) { K.sl = 1; }
}
void KeysUp(unsigned char key, int x, int y)
{
	if (key == 'w' == 1) { K.w = 0; }
	if (key == 's' == 1) { K.s = 0; }
	if (key == 'a' == 1) { K.a = 0; }
	if (key == 'd' == 1) { K.d = 0; }
	if (key == 'm' == 1) { K.m = 0; }
	if (key == ',' == 1) { K.sr = 0; }
	if (key == '.' == 1) { K.sl = 0; }
}

void init()
{
	meshCube.Load("test.obj");
	//meshCube.tris = {

	//	// SOUTH
	//	{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
	//	{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

	//	// EAST                                                      
	//	{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
	//	{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

	//	// NORTH                                                     
	//	{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
	//	{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

	//	// WEST                                                      
	//	{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
	//	{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

	//	// TOP                                                       
	//	{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
	//	{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

	//	// BOTTOM                                                    
	//	{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
	//	{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

	//};

}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(GLSW / 2, GLSH / 2);
	glutInitWindowSize(GLSW, GLSH);
	glutCreateWindow("");
	glPointSize(pixelScale);                        //pixel size
	gluOrtho2D(0, GLSW, 0, GLSH);                      //origin bottom left
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(KeysDown);
	glutKeyboardUpFunc(KeysUp);
	glutMainLoop();
	return 0;
}
