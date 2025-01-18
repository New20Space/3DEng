#pragma once

class Vec2f
{
public:
	float x, y;
	Vec2f();
	Vec2f(float a0, float a1);
	Vec2f(float ang);
	void L_I(float x, Vec2f a, Vec2f b);

	inline Vec2f operator*(const float & Mag) const
	{
		Vec2f Ret;

		Ret.x = x * Mag;
		Ret.y = y * Mag;
		
		return Ret;
	}

	inline float operator*(const Vec2f& Right) const
	{
		//dot product
		float z = x * Right.x  + y * Right.y;
		return z;
	}
};
class Vec3f
{
public:
	float x, y, z;
	Vec3f();
	Vec3f(float a0, float a1, float a2);
	void Normalize();
	
	inline Vec3f operator+(const Vec3f& Right) const
	{
		Vec3f Ret;

		Ret.x = x + Right.x;
		Ret.y = y + Right.y;
		Ret.z = z + Right.z;


		return Ret;
	}
	inline Vec3f operator-(const Vec3f& Right) const
	{
		Vec3f Ret;

		Ret.x = x - Right.x;
		Ret.y = y - Right.y;
		Ret.z = z - Right.z;


		return Ret;
	}
	inline Vec3f operator*(const Vec3f& Right) const	
	{	
		//cross product
		Vec3f Ret;

		Ret.x = y * Right.z - z * Right.y;
		Ret.y = z * Right.x - x * Right.z;
		Ret.z = x * Right.y - y * Right.x;
		return Ret;
	}

private:

};

class Mat4f
{
public:
	float m[4][4];

	Mat4f();

	Mat4f(float a00, float a01, float a02, float a03,
		float a10, float a11, float a12, float a13,
		float a20, float a21, float a22, float a23,
		float a30, float a31, float a32, float a33);

	inline Mat4f operator*(const Mat4f& Right) const
	{
		Mat4f Ret;

		for (unsigned int i = 0; i < 4; i++) {
			for (unsigned int j = 0; j < 4; j++) {
				Ret.m[i][j] = m[i][0] * Right.m[0][j] +
					m[i][1] * Right.m[1][j] +
					m[i][2] * Right.m[2][j] +
					m[i][3] * Right.m[3][j];
			}
		}

		return Ret;
	}
};

