#include "Perlin_Noise.h"

#define PI	3.14158 
Perlin_Noise::Perlin_Noise() {}
void Perlin_Noise::Init(float WA,float HA) {
	Width = WA;
	Height = HA;
	for (size_t x = 0; x < WA; x++)
	{
		for (size_t y = 0; y < HA; y++)
		{
			//Arr[x* Height +y] = (rand() % 1000) * 0.001 * PI * 2;
			Arr.push_back((rand() % 1000) * 0.001 * PI * 2);

		}
	}
}
void Perlin_Noise::Animate(float rate) {
		for (size_t i = 0; i < Arr.size(); i++)
		{
			Arr[i] += rate;
		}
}

float Perlin_Noise::Get_Noise(float x, float y) {
	Vec2f iv((int)x, (int)y);
	Vec2f fv(x-(int)x, y-(int)y);

	float a, b, c, d;
	a = Arr[(iv.x	 )* Height +iv.y];
	b = Arr[(iv.x + 1)* Height +iv.y];
	c = Arr[(iv.x + 1)* Height +iv.y + 1];
	d = Arr[(iv.x	 )* Height +iv.y + 1];
	float N = Cell(fv, a, b, c, d);

	return N;
}
float Perlin_Noise::Cell(Vec2f pos, float r1, float r2, float r3, float r4) {

	Vec2f cor1(r1); 	// 4* * * *3      out_y2
	Vec2f cor2(r2);		//  *     *
	Vec2f cor3(r3);  	//  *     *
	Vec2f cor4(r4);		// 1* * * *2      out_y1

	//pos = pos * (1/(sqrtf(0.5)));

	Vec2f a(-pos.x, 0);
	Vec2f b(0, -pos.y);
	Vec2f a1(1 - pos.x, 0);
	Vec2f b1(0, 1 - pos.y);

	float f1 = cor1 * a + cor1 * b;
	float f2 = cor2 * a1 + cor2 * b;
	float f3 = cor3 * a1 + cor3 * b1;
	float f4 = cor4 * a + cor4 * b1;

	Vec2f out_y1, out_y2, out;
	out_y1.L_I(pos.x, Vec2f(0, f1), Vec2f(1, f2));
	out_y2.L_I(pos.x, Vec2f(0, f4), Vec2f(1, f3));

	//floor(out_y1.y * 10000) / 10000)
	out.L_I(pos.y, Vec2f(0, out_y1.y), Vec2f(1, out_y2.y));

	return (out.y ) ; 
}