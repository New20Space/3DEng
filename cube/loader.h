#pragma once



#include "ECS.h"
#include "Math3D.h"


#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


class Loader
{
public:
	ECS* M_ECS;
	Loader();
	void Init(ECS* M_ECS1);
	bool LColor(mesh& MObj, string sFilename);
	void SetColor(mesh& MObj);
	bool Load(mesh& MObj, string sFilename);
};
	
	