#include "loader.h"

void Loader::Init(ECS* M_ECS1) {
	M_ECS = M_ECS1;
	(*M_ECS).reg.view<Mesh>().each([&](auto entity, Mesh& Set) {
		Load(Set.MObj,Set.Path);
		
		glGenBuffers(1, &(Set.buf ));
		glGenBuffers(1, &(Set.buf2));
		glGenBuffers(1, &(Set.buf3));
	});
}
Loader::Loader()
{	
}

bool Loader::LColor(mesh& MObj,string sFilename)
{
	ifstream f(sFilename);
	if (!f.is_open())
		return false;
	while (!f.eof())
	{
		char line[228];
		f.getline(line, 228);

		strstream s;
		s << line;

		char junk;

		if ((line[0] == 'n') && (line[1] == 'e') && (line[2] == 'w'))
		{
			ch3 v{ line[16], line[17], line[18] };
			MObj.mat.push_back(v);
		}
		if ((line[0] == 'K') && (line[1] == 'd') && (line[2] == ' '))
		{
			line[0] = ' ';
			line[1] = ' ';
			line[2] = ' ';
			sscanf_s(line, "%f %f %f", &MObj.mat[MObj.mat.size() - 1].Col[0], &MObj.mat[MObj.mat.size() - 1].Col[1], &MObj.mat[MObj.mat.size() - 1].Col[2]);
		}
	}
	return true;
}
void Loader::SetColor(mesh& MObj) {
	MObj.color.clear();
	for (size_t i = 0; i < MObj.tris.size(); i++)
	{
		MObj.color.push_back(MObj.main_col);
	}
}
bool Loader::Load(mesh& MObj, string sFilename)
{
	ifstream f(sFilename);
	if (!f.is_open())
		return false;


	bool col = LColor(MObj,sFilename.substr(0, sFilename.size() - 4) + ".mtl");

	vector<Vec3f> verts;// Local cache of verts
	Vec3f out;
	Vec3f curnorm;
	Vec3f curcol;
	curcol = MObj.main_col;
	while (!f.eof())
	{
		char line[228];
		f.getline(line, 228);

		strstream s;
		s << line;

		char junk;


		if ((line[0] == 'v') && (line[1] == ' '))
		{
			Vec3f v{ 0,0,0 };
			line[0] = ' ';
			line[1] = ' ';
			s >> junk >> v.x >> v.y >> v.z;

			verts.push_back(v);
		}
		if ((line[0] == 'u') && (line[1] == 's') && (line[2] == 'e'))
		{
			for (int i = 0; i < MObj.mat.size(); i++)
			{
				if ((line[16 + 0] == MObj.mat[i].Pos[0]) && (line[16 + 1] == MObj.mat[i].Pos[1]) && (line[16 + 2] == MObj.mat[i].Pos[2]))
				{
					curcol.x = MObj.mat[i].Col[0];
					curcol.y = MObj.mat[i].Col[1];
					curcol.z = MObj.mat[i].Col[2];
				}
			}


		}
		if (line[0] == 'f')
		{
			int f1[3];
			line[0] = ' ';
			s >> junk >> f1[0] >> f1[1] >> f1[2];


			Vec3f  line1, line2;
			line1 = verts[f1[1] - 1] - verts[f1[0] - 1];
			line2 = verts[f1[2] - 1] - verts[f1[0] - 1];
			

			curnorm = line1 * line2;
			curnorm.Normalize();

			for (size_t i = 0; i < 3; i++)
			{
				out = verts[f1[i] - 1];
				
				MObj.tris.push_back(out);
				MObj.color.push_back(curcol);
				MObj.normal.push_back(curnorm);
			}

			//cout << line <<"\t " << f1[0] << " " << f1[1] << " " << f1[2] << endl;
			/*tris.push_back(verts[f1[0] - 1]);
			tris.push_back(verts[f1[1] - 1]);
			tris.push_back(verts[f1[2] - 1]);*/
		}

	}


	return true;
}