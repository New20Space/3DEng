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



float a = 0;
float x = PI / 2 ;
float y = PI / 2;
float z = 0 ;


void MImGui() {

	

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	
	ImGui::SliderFloat("A", &a, 0.0f, PI*2);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::SliderFloat("x", &x, 0.0f, PI);
	ImGui::SliderFloat("y", &y, 0.0f, PI*2);            
	ImGui::SliderFloat("z", &z, -PI / 2, PI/2);


	ImGui::End();
}
float  ApplyNoise(Vec3f Pos,Vec3f Max, Vec3f Min, Perlin_Noise& Noise) {//to a plane
	float MW = Max.x - Min.x;
	float MH = Max.z - Min.z;

	float r_w = MW / Noise.Width; //ratio 
	float r_h = MH / Noise.Height;

	float s_w = Min.x;//shift
	float s_h = Min.z;

	float x = (Pos.x - s_w) / r_w;
	float y = (Pos.z - s_h) / r_h;

	return Noise.Get_Noise(x, y);

}

void Start() {
	Perlin_Noise* Map;

	srand(860);
	Perlin_Noise Col;
	Col.Init(15.1, 15.1);

	M_ECS.reg.view<Mesh>().each([&](auto entity, Mesh& MP) {
		if (MP.A_Noise) {
			
			for (int num_vertex = 0; num_vertex < MP.MObj.color.size(); num_vertex++)
			{	
				
				float N= ApplyNoise(MP.MObj.tris[num_vertex], MP.MObj.Max,MP.MObj.Min,MP.Noise);
				
				MP.MObj.tris[num_vertex].y = N;

				if (((num_vertex + 1) % 3) == 0)
				{

					Vec3f  curnorm, line1, line2;
					line1 = MP.MObj.tris[num_vertex - 1] - MP.MObj.tris[num_vertex - 2];
					line2 = MP.MObj.tris[num_vertex] - MP.MObj.tris[num_vertex - 2];


					curnorm = line1 * line2;
					curnorm.Normalize();
					MP.MObj.normal[num_vertex - 2] = curnorm;
					MP.MObj.normal[num_vertex - 1] = curnorm;
					MP.MObj.normal[num_vertex] = curnorm;
				}
				float C = ApplyNoise(MP.MObj.tris[num_vertex], MP.MObj.Max, MP.MObj.Min, Col);
				float red   = C;
				float green = 1;
				float blue  = C;
				MP.MObj.color[num_vertex] = { red,green,blue };

			}

			Map = &MP.Noise;

		}

		});

}
void main_dis() {
	if (T.fr1 - T.fr2 >= 50)                        //only draw 20 frames/second
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGLUT_NewFrame();
		ImGui::NewFrame();

		M_R.InitCamera();
		M_R.MovePlayer();

		M_R.Render();

		T.fr2 = T.fr1;


		MImGui();




		M_ECS.reg.view<Mesh,Transform>().each([&](auto entity, Mesh& MP,Transform& T) {

			if (T.State==variable)
			{
			if (MP.A_Noise!=1) {
				
				T.Ang = { 0,a, 0};
			}

			}
			else if(T.State == invariable) {
				M_ECS.reg.view<Ñamera, Transform>().each([&](auto entity, Ñamera& Ñam, Transform& Tr) {
					
					T.Pos = { 50 * sin(z), 50 * sin(x) *cos(z),50 * cos(x) };

					Ñam.LightPos = T.Pos;
					});

			}
			

			});





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
	OTP.Scale = { 5.0,5.0,5.0 };
	//OTP.Ang = { 0.0,PI / 4,0.0 };
	Mesh MP;
	MP.Path = "Model/plane.obj";

	srand(306);
	MP.A_Noise = 1;
	MP.Noise.Init(10.1, 10.1);
	
	M_ECS.AddObj(OTP, MP);



	Transform OT;
	OT.Ang = { 0.0,PI/4,0.0 };
	Mesh M;
	M.Path = "Model/din.obj";

	M_ECS.AddObj(OT,M);

	Transform LT;
	LT.Pos = { 100,100,100 };
	LT.State = invariable;
	Mesh ML;
	ML.Path = "Model/Suz.obj";
	M_ECS.AddObj(LT, ML);

	


	//Transform PImg;
	//Mesh MImg;

	//int imgW, imgH, n;
	//int num_ch = 3;
	//unsigned char* data = stbi_load("Img/test.png", &imgW, &imgH, &n, num_ch);


	//MImg.primitive = GL_POINTS;

	//int idx = 0;
	//for (int y = 0; y < imgH; y++) {
	//	for (int x = 0; x < imgW; x++) {
	//		float red = data[idx * num_ch] / 255.0;
	//		float green = data[idx * num_ch + 1] / 255.0;
	//		float blue = data[idx * num_ch + 2] / 255.0;
	//		idx++;

	//		MImg.MObj.tris.push_back({ red * 30,green * 30,blue * 30 });
	//		MImg.MObj.color.push_back({ red,green,blue });
	//		MImg.MObj.normal.push_back({ 0,0,1 });

	//	}
	//}

	//M_ECS.AddObj(PImg, MImg);

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
