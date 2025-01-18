#pragma once
#define ToRadian(x) (float)(((x) * 3.14 / 180.0f))

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glut.h"
#include "Imgui/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "ECS.h"
#include "loader.h"
#include "ETexture.h"
#include "Math3D.h"

class render
{
public:
	ECS* M_ECS;
	void Init(ECS *M_ECS1);
	void MovePlayer();
	void InitCamera();
	
	bool ReadF(const char* path, std::string& outFile);
	void Render();
	void DelProg();


private:
	
	Vec3f* Tpos;
	Vec3f* Ang;

	int SW, SH;
	Mat4f FinalMatrix;
	std::vector<unsigned int> prog;
	unsigned int CompileShader(unsigned int type, const std::string& sCode);
	unsigned int CreateShader(const std::string& vsh, const std::string& fsh);
};

