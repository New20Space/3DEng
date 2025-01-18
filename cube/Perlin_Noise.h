#pragma once
#include "Math3D.h"
#include <vector>

class Perlin_Noise
{
public:
	float Width, Height;
	Perlin_Noise();
	void Init(float WA, float HA);
	void Animate(float rate);
	float Get_Noise(float x, float y);
	float Cell(Vec2f pos, float r1, float r2, float r3, float r4);
private:
	std::vector<float> Arr;
};

