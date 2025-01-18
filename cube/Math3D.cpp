#include "Math3D.h"
#include <math.h>

void Vec2f::L_I(float x, Vec2f a, Vec2f b) {
	// Linear interpolation
	y = (a.y * (b.x - x) + b.y * (x - a.x)) / (b.x - a.x);
}

Vec2f::Vec2f() {}
Vec2f::Vec2f(float a0, float a1) : x(a0), y(a1) {}
Vec2f::Vec2f(float ang) : x(cos(ang)), y(sin(ang)) {}


Vec3f::Vec3f() {}
Vec3f::Vec3f(float a0, float a1, float a2) : x(a0), y(a1) ,z(a2){}

void Vec3f::Normalize() {
	float l = sqrtf(x * x + y * y + z * z);
	x /= l; y /= l; z /= l;
}


Mat4f::Mat4f() {}
Mat4f::Mat4f(float a00, float a01, float a02, float a03, float a10, float a11, float a12, float a13, float a20, float a21, float a22, float a23, float a30, float a31, float a32, float a33)
{
	m[0][0] = a00; m[0][1] = a01; m[0][2] = a02; m[0][3] = a03;
	m[1][0] = a10; m[1][1] = a11; m[1][2] = a12; m[1][3] = a13;
	m[2][0] = a20; m[2][1] = a21; m[2][2] = a22; m[2][3] = a23;
	m[3][0] = a30; m[3][1] = a31; m[3][2] = a32; m[3][3] = a33;
}
