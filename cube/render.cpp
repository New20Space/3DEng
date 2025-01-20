#include "render.h"


void render::MovePlayer()
{

	if (GetKeyState('A') >= 0) {
		(*Ang).y += 0.1f;
	}

	if (GetKeyState('D') >= 0) {
		(*Ang).y -= 0.1f;
	}
	if (GetKeyState('Z') >= 0) {
		(*Ang).x += 0.1f;
	}

	if (GetKeyState('C') >= 0) {
		(*Ang).x -= 0.1f;
	}
	if (GetKeyState('S') >= 0) {
		(*Tpos).z -= 1;
	}

	if (GetKeyState('W') >= 0) {
		(*Tpos).z += 1;
	}
	if (GetKeyState('R') >= 0) {
		(*Tpos).x += 1;
	}

	if (GetKeyState('F') >= 0) {
		(*Tpos).x -= 1;
	}
	if (GetKeyState('Q') >= 0) {
		(*Tpos).y += 1;
	}

	if (GetKeyState('E') >= 0) {
		(*Tpos).y -= 1;
	}


}




void render::InitCamera()
{


	float FOV = 90.0f;
	float NearZ = 1.0f;
	float FarZ = 300.0f;
	float zRange = NearZ - FarZ;

	float tanHalfFOV = tanf(ToRadian(FOV / 2.0f));
	float F = 1 / tanHalfFOV;
	float A = (-FarZ - NearZ) / zRange;
	float B = 2.0f * FarZ * NearZ / zRange;


	Mat4f Rotationx(1.0f, 0.0f,			0.0f,		   0.0f,
					0.0f, cosf((*Ang).x), -sinf((*Ang).x), 0.0f,
					0.0f, sinf((*Ang).x), cosf((*Ang).x),  0.0f,
					0.0f, 0.0f,		    0.0f,		   1.0f);
	Mat4f Rotationy(cosf((*Ang).y), 0.0f, -sinf((*Ang).y), 0.0f,
					0.0f,		  1.0f, 0.0f,		   0.0f,
					sinf((*Ang).y), 0.0f, cosf((*Ang).y), 0.0f,
					0.0f,		  0.0f, 0.0f,		  1.0f);
	Mat4f Rotationz(cosf((*Ang).z),-sinf((*Ang).z),0.0f, 0.0f,
					sinf((*Ang).z),cosf((*Ang).z), 0.0f, 0.0f,
					0.0f,		 0.0f,		   1.0f, 0.0f,
					0.0f,        0.0f,		   0.0f, 1.0f);
	Mat4f Translation(1.0f, 0.0f, 0.0f, (*Tpos).x,
					  0.0f, 1.0f, 0.0f, (*Tpos).y,
					  0.0f, 0.0f, 1.0f, (*Tpos).z,
					  0.0f, 0.0f, 0.0f, 1.0f);


	Mat4f Projection(F, 0.0f, 0.0f, 0.0f,
		0.0f, F, 0.0f, 0.0f,
		0.0f, 0.0f, A, B,
		0.0f, 0.0f, 1.0f, 0.0f);

	/*Mat4f Projection(1, 0.0f, 0.0f, 0.0f,
		0.0f, 1, 0.0f, 0.0f,
		0.0f, 0.0f, A, B,
		0.0f, 0.0f, 1.0f, 0.0f);*/



	FinalMatrix = Projection* Translation  *Rotationx* Rotationy* Rotationz;

}


void render::Init(ECS* M_ECS1) {

 
	M_ECS = M_ECS1;
	
	


	
	std::string vF, fF;
	(*M_ECS).reg.view<Ñamera, Transform>().each([&](auto entity, Ñamera& Set, Transform& T) {
		
		Ang = &(T.Ang);
		Tpos = &(T.Pos);
		SW = Set.S_W;
		SH = Set.S_H;
		ReadF(Set.VShader.c_str(), vF);
		ReadF(Set.FShader.c_str(),fF);
		
	});
	
	prog.push_back(CreateShader(vF, fF));

}

bool render::ReadF(const char* path, std::string& outFile) {
	using namespace std;
	ifstream fin(path);

	if (fin.is_open()) {
		string line;
		while (getline(fin, line)) {
			outFile.append(line);
			outFile.append("\n");
		}
		fin.close();
		return true;
	}
	else {
		return false;
	}

}
void render::Render()
{	
	glUseProgram(prog[0]);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	(*M_ECS).reg.view<Mesh, Transform>().each([&](auto entity, Mesh& Obj, Transform& T) {
		
		Mat4f Rotationx(1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, cosf(T.Ang.x), -sinf(T.Ang.x), 0.0f,
						0.0f, sinf(T.Ang.x), cosf(T.Ang.x), 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);
		Mat4f Rotationy(cosf(T.Ang.y), 0.0f, -sinf(T.Ang.y), 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						sinf(T.Ang.y), 0.0f, cosf(T.Ang.y), 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);
		Mat4f Rotationz(cosf(T.Ang.z), -sinf(T.Ang.z), 0.0f, 0.0f,
						sinf(T.Ang.z), cosf(T.Ang.z), 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);

		Mat4f Translation(T.Scale.x, 0.0f, 0.0f, T.Pos.x,
						  0.0f, T.Scale.y, 0.0f, T.Pos.y,
						  0.0f, 0.0f, T.Scale.z, T.Pos.z,
						  0.0f, 0.0f, 0.0f, 1.0f);


		FinalMatrix = FinalMatrix * Translation * Rotationx * Rotationy * Rotationz;


		int SizeArr = Obj.MObj.tris.size();
		int gWorldLocation = glGetUniformLocation(prog[0], "gWorld");
		glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &FinalMatrix.m[0][0]);

		int numv;
		switch (Obj.primitive)
		{
		case GL_POINTS:
			numv = 1;
		case GL_LINES:
			numv = 1;
		case GL_TRIANGLES:
			numv = 3;

		}

		glBindBuffer(GL_ARRAY_BUFFER, Obj.buf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Obj.MObj.tris[0]) * Obj.MObj.tris.size(), &Obj.MObj.tris[0], GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, numv, GL_FLOAT, GL_FALSE, sizeof(float) * numv, 0);



		glBindBuffer(GL_ARRAY_BUFFER, Obj.buf2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Obj.MObj.color[0]) * Obj.MObj.color.size(), &Obj.MObj.color[0], GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, numv, GL_FLOAT, GL_FALSE, sizeof(float) * numv, 0);


		glBindBuffer(GL_ARRAY_BUFFER, Obj.buf3);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Obj.MObj.normal[0]) * Obj.MObj.normal.size(), &Obj.MObj.normal[0], GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, numv, GL_FLOAT, GL_FALSE, sizeof(float) * numv, 0);

		glDrawArrays(Obj.primitive, 0, SizeArr);

		});


}

unsigned int render::CompileShader(unsigned int type, const std::string& sCode) {
	unsigned int id = glCreateShader(type);
	const char* src = sCode.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int len;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
		char* mes = (char*)alloca(len * sizeof(char));

		glGetShaderInfoLog(id, len, &len, mes);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader." << std::endl;
		std::cout << mes << std::endl;

		glDeleteShader(id);
		return 0;
	}
	return id;

}
unsigned int render::CreateShader(const std::string& vsh, const std::string& fsh) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vsh);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fsh);


	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);


	return program;
}



void render::DelProg()
{
	for (size_t i = 0; i < prog.size(); i++)
	{
		glDeleteProgram(prog[i]);
	}
}