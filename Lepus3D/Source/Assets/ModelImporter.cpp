#include "ModelImporter.h"

using namespace LepusEngine;
using namespace Lepus3D;
using namespace Assets;
using namespace std;

ModelImporter::ModelImporter()
{
	mObjFile = nullptr;
	mMtlFile = nullptr;
}

ModelImporter::ModelImporter(string fn)
{
	bool success = false;
	mObjFile = nullptr;
	mMtlFile = nullptr;
	success = Init(fn);
}

bool ModelImporter::_ReadOBJ()
{
	string line = "";
	enum OBJImportState { ObjectSearch, BuildObject };
	OBJImportState state = ObjectSearch;
	VertexArray verts, normals, finalVerts; // obj indexes normals and assigns them to vertices, so we'll need to load them into a separate array. also separate array for final vertices as we don't use indexing (we'll recalculate normals)
	vector<unsigned int> indices, finalIndices; // finalIndices will just be an array of indices with the size of n total vertices
	int indexCounter = 0;
	while(mObjFile->good())
	{
		getline(*mObjFile, line);
		if(line[0] == '#') // ignore comments
			continue;
		string keyword = ""; // current keyword, like "o" for object, "v" for vertex, etc.
		string data = ""; // current data, like coordinates when keyword is "v", or material name when keyword is "usemtl"
		int firstSpace = line.find(" "); // first whitespace character in the line
		keyword = line.substr(0, firstSpace);
		if(keyword == "o")
		{
			if(state == ObjectSearch)
			{
				if (verts.size() > 0)
				{
					Mesh loadedMesh(finalVerts, true);
					loadedMesh.SetIndices(finalIndices);
					mGeometry.push_back(loadedMesh);
				}
				verts.clear();
				normals.clear();
				indices.clear();
				finalVerts.clear();
				finalIndices.clear();
				indexCounter = 0;
				state = BuildObject;
			}
			else
				state = ObjectSearch;
		}
		else if(keyword == "v" || keyword == "vn")
		{
			Vertex v;
			data = line.substr(firstSpace+1);
			string xyz[3] = { "", "", "" };
			for(int i = 0; i < 3; i++)
			{
				xyz[i] = data.substr(0, data.find(" "));
				data = data.substr(data.find(" ")+1);
			}
			if(keyword == "v")
			{
				v.x = stof(xyz[0]);
				v.y = stof(xyz[1]);
				v.z = stof(xyz[2]);
				verts.push_back(v);
			}
			else
			{
				v.nX = stof(xyz[0]);
				v.nY = stof(xyz[1]);
				v.nZ = stof(xyz[2]);
				normals.push_back(v);
			}
		}
		else if(keyword == "f")
		{
			data = line;
			for(int i = 0; i < 3; i++)
			{
				data = data.substr(data.find(" ")+1);
				string vertexIndex = data.substr(0, data.find("/"));
				data = data.substr(data.find("/")+1);
				// next up should be the vertex UV coord index, which might be empty
				// loading in UVs is still TODO
				string uvIndex = "";
				if(data.find("/") != 0)
				{
					uvIndex = data.substr(data.find("/"));
					data = data.substr(data.find("/")+1);
				}
				else
					data = data.substr(1);
				string normalIndex = data.substr(0, data.find(" "));
				unsigned int vIdx = stoi(vertexIndex);
				if(uvIndex != "")
					unsigned int uvIdx = stoi(uvIndex);
				unsigned int nIdx = stoi(normalIndex);
				indices.push_back(indexCounter++);
				finalVerts.push_back(verts[vIdx-1]);
				finalVerts[indexCounter-1].nX = normals[nIdx-1].nX;
				finalVerts[indexCounter-1].nY = normals[nIdx-1].nY;
				finalVerts[indexCounter-1].nZ = normals[nIdx-1].nZ;
			}
		}
	}
	Mesh loadedMesh(finalVerts, true);
	loadedMesh.SetIndices(indices);
	mGeometry.push_back(loadedMesh);
	verts.clear();
	normals.clear();
	indices.clear();
	return true;
}

bool ModelImporter::Init(string fn)
{
	// navigate to content folder
	fn = "../../Content/" + fn;
	// filename for the .obj file
	string objFN = fn;
	// filename for the .mtl material library file
	string mtlFN = fn.substr(0, fn.length() - string(".obj").length()) + ".mtl";

	mObjFile = new ifstream(objFN);
	mMtlFile = new ifstream(mtlFN);

	return true;
}

bool ModelImporter::Read()
{
	return _ReadOBJ();
}

Mesh ModelImporter::GetSubMesh(int idx)
{
	return mGeometry[idx];
}

bool ModelImporter::Close()
{
	// close file handles
	mObjFile->close();
	mMtlFile->close();

	// delete pointers
	delete mObjFile;
	delete mMtlFile;

	// reinit pointers
	mObjFile = nullptr;
	mMtlFile = nullptr;

	return true;
}

ModelImporter::~ModelImporter()
{
	if(mObjFile != nullptr && mMtlFile != nullptr)
		Close();
}
