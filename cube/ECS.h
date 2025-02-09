#pragma once
#pragma warning(disable : 4996)

#include <Entt/entt.hpp>
#include <iostream>
#include "Math3D.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <strstream>
#include <GL/glew.h>
#include "Perlin_Noise.h"

enum sMove
{
	invariable,
	variable

};
struct Transform
{
	sMove State = variable;

	Vec3f Pos = { 0.0f,0.0f,0.0f };
	Vec3f Vel = { 0.0f,0.0f,0.0f };
	Vec3f Ang = { 0.0f,0.0f,0.0f };
	Vec3f Scale = { 1.0f,1.0f,1.0f };

};

using namespace std;
struct ch3
{
	char Pos[3];
	float Col[3];
};
struct mesh
{

	Vec3f Max = { 0,0,0 };  //mesh boundaries
	Vec3f Min = { 0,0,0 };

	vector<Vec3f> tris;
	//Texture* pTex;
	vector<Vec2f> texcoord;
	vector<Vec3f> color;
	vector<Vec3f> normal;
	Vec3f main_col = { 0.8,0.8,0.9 };
	vector<ch3>mat;
};

struct Mesh
{
	std::string Path;
	mesh MObj;
	unsigned int buf, buf2, buf3;
	GLenum primitive = GL_TRIANGLES;
	bool A_Noise=0;
	Perlin_Noise Noise;
};
struct �amera
{
	int S_W, S_H;
	std::string VShader;
	std::string FShader;
	
	Vec3f LightPos = { 100,100,100 };

};



class ECS
{	
public:
	entt::registry reg;
	void Add�amera(Transform T, �amera CS);
	void AddObj(Transform T, Mesh M);
private:
	
};

