#define NOMINMAX
#include <windows.h>
#include <thread>
#include <math.h>

#include <iostream>
#include <fstream>

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glut.h"
#include "Imgui/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GL/freeglut.h>





#include <string.h>

#include <fstream>
#include "ECS.h"



#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "render.h"
#include "Perlin_Noise.h"



#define SW         800                  //screen width
#define SH         800                  //screen height
#define pixelScale 5                    //OpenGL pixel scale
#define GLSW       (SW)					//OpenGL window width
#define GLSH       (SH)					//OpenGL window height
#define GLW        (GetSystemMetrics(SM_CXSCREEN) - GLSW)
#define GLH        0 
#define PI			3.14158 


//------------------------------------------------------------------------------
typedef struct
{
	int fr1, fr2;           //frame 1 frame 2, to create constant frame rate
}time1; time1 T;



//------------------------------------------------------------------------------
ECS M_ECS;
render M_R;
Loader M_L;



void Start() {
	
M_ECS.reg.view<Mesh>().each([&](auto entity, Mesh& MP) {
			if (MP.A_Noise) {
				MP.MObj.tris.clear();
				MP.MObj.color.clear();
				MP.MObj.normal.clear();

				//MP.Noise.Animate(0.1);
				for (float y = 0; y < MP.Noise.Height; y += 0.01) {
					for (float x = 0; x < MP.Noise.Width; x += 0.01) {
						float N = MP.Noise.Get_Noise(x, y);

						float red =   1 - N;
						float green = 1 - N;
						float blue =  1;



						MP.MObj.tris.push_back({ x * 20 - 25 , y * 20 - 5,0 });//(N*2-1) * 50
						MP.MObj.color.push_back({ red,green,blue });
						MP.MObj.normal.push_back({ 0,0,1 });

					}
				}

			}
			});

}
float a = PI / 4 + PI;
float b = PI / 4 + PI / 2 + PI;
float c = PI / 4;
float d = PI / 4 + PI / 2;

void MImGui() {

	

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::SliderFloat("A", &a, 0.0f, PI*2);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::SliderFloat("B", &b, 0.0f, PI*2);            
	ImGui::SliderFloat("C", &c, 0.0f, PI*2);            
	ImGui::SliderFloat("D", &d, 0.0f, PI*2);            


	ImGui::End();
}

void main_dis() {
	if (T.fr1 - T.fr2 >= 50)                        //only draw 20 frames/second
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGLUT_NewFrame();
		ImGui::NewFrame();

		//M_ECS.reg.view<Mesh>().each([&](auto entity, Mesh& MP) {
		//	if (MP.A_Noise) {
		//		MP.MObj.tris.clear();
		//		MP.MObj.color.clear();
		//		MP.MObj.normal.clear();

		//		for (float y = 0; y < 1; y += 0.01) {
		//			for (float x = 0; x < 1; x += 0.01) {
		//				float N = MP.Noise.Cell(Vec2f(x, y), a, b, c, d);

		//				float red = N;
		//				float green = N;
		//				float blue = N ;



		//				MP.MObj.tris.push_back({ x * 20 - 25 , y * 20 - 5 , 0 });//(N ) * 50
		//				MP.MObj.color.push_back({ red,green,blue });
		//				MP.MObj.normal.push_back({ 0,0,1 });
		//			}
		//		}
		//		cout<< MP.Noise.Cell(Vec2f(0.5, 0.5), a, b, c, d)<<endl;
		//	}
		//	});

		M_R.InitCamera();
		M_R.MovePlayer();

		M_R.Render();

		T.fr2 = T.fr1;


		MImGui();

		// Rendering
		ImGui::Render();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glutSwapBuffers();
	}
	T.fr1 = glutGet(GLUT_ELAPSED_TIME);      
	glutPostRedisplay();
}

//------------------------------------------------------------------------------
void ECS_Init()
{
	Transform CT;
	CT.Pos = { 0.0,-20.0,30.0 };
	CT.Ang = { 0.0,0,0.0 };
	Ñamera CS;
	CS.S_W = SW;
	CS.S_H = SH;
	CS.VShader = "Shader/vshader_N.vs";
	CS.FShader = "Shader/fshader.fs";

	M_ECS.AddÑamera(CT,CS);


	Transform OTP;
	Mesh MP;
	MP.primitive = GL_POINTS;

	srand(306);
	MP.A_Noise = 1;
	MP.Noise.Init(20.1, 20.1);

	M_ECS.AddObj(OTP, MP);


	Transform OT;
	Mesh M;
	M.Path = "din.obj";

	M_ECS.AddObj(OT,M);

	


	//Transform OTP;
	//Mesh MP;

	//int imgW, imgH, n;
	//int num_ch = 3;
	//unsigned char* data = stbi_load("test.png", &imgW, &imgH, &n, num_ch);


	//MP.primitive = GL_POINTS;

	//int idx = 0;
	//for (int y = 0; y < imgH; y++) {
	//	for (int x = 0; x < imgW; x++) {
	//		float red = data[idx * num_ch] / 255.0;
	//		float green = data[idx * num_ch + 1] / 255.0;
	//		float blue = data[idx * num_ch + 2] / 255.0;
	//		idx++;

	//		MP.MObj.tris.push_back({ red * 30,green * 30,blue * 30 });
	//		MP.MObj.color.push_back({ red,green,blue });
	//		MP.MObj.normal.push_back({ 0,0,1 });

	//	}
	//}

	//M_ECS.AddObj(OTP, MP);

	//stbi_image_free(data);



}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(GLW, GLH);
	glutInitWindowSize(GLSW, GLSH);
	glutCreateWindow("Test");
	glewInit();

	glPointSize(pixelScale); 
	
	glutDisplayFunc(main_dis);

	ECS_Init();

	M_R.Init(&M_ECS);
	M_L.Init(&M_ECS);

	Start();



	IMGUI_CHECKVERSION();
	ImGui::CreateContext();


	ImGui::StyleColorsDark();

	ImGui_ImplGLUT_Init();
	ImGui_ImplGLUT_InstallFuncs();
	ImGui_ImplOpenGL3_Init();


	glutMainLoop();

	M_R.DelProg();

	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();
	return 0;
}
