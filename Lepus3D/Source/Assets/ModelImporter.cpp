#include "ModelImporter.h"

using namespace LepusEngine;
using namespace Lepus3D;
using namespace Assets;
using namespace std;

// Default constructor
ModelImporter::ModelImporter()
{
	// initialise member variables
	mObjFile = nullptr;
	mMtlFile = nullptr;
}

// Opens file after constructor
ModelImporter::ModelImporter(string fn, string dir)
{
	// initialise member variables
	bool success = false;
	mObjFile = nullptr;
	mMtlFile = nullptr;
	success = Init(fn, dir);
}

// Opens model file
bool ModelImporter::Init(string fn, string dir)
{
	// navigate to content folder
	fn = dir + "/" + fn;
	// filename for the .obj file
	string objFN = fn;
	// filename for the .mtl material library file
	string mtlFN = fn.substr(0, fn.length() - string(".obj").length()) + ".mtl";

	mGeometry.clear(); // remove previous geometry loaded via this importer

	// initialise filestream pointers
	mObjFile = new ifstream(objFN);
	mMtlFile = new ifstream(mtlFN);

	return true;
}

// Actual .obj parsing
bool ModelImporter::_ReadOBJ()
{
	string line = ""; // currently read line
	enum OBJImportState { ObjectSearch, BuildObject }; // states are needed as multiple objects/meshes can be found in a model file (TODO: actually this looks unnecessary and could use a rewrite)
	OBJImportState state = ObjectSearch; // ObjectSearch is when the parser is looking for an object/mesh, then it jumps to BuildObject to load vertices for that object/mesh
	VertexArray verts, normals, uvs, finalVerts; // obj indexes normals and assigns them to vertices, so we'll need to load them into a separate array. also separate array for final vertices as we don't use indexing (we'll recalculate normals)
	vector<unsigned long long> indices, finalIndices; // finalIndices will just be an array of indices with the size of n total vertices
	int indexCounter = 0; // TODO: an incrementing index to write for each vertex (as in, ignoring indexing altogether and just writing an index for each vertex). Might consider getting rid of drawing elements altogether and just draw arrays instead.
	while(mObjFile->good()) // check if file is still readable
	{
		getline(*mObjFile, line); // read next line
		if(line[0] == '#') // ignore comments
			continue;

		string keyword = ""; // current keyword, like "o" for object, "v" for vertex, etc.
		string data = ""; // current data, like coordinates when keyword is "v", or material name when keyword is "usemtl"
		int firstSpace = line.find(" "); // first whitespace character in the line
		keyword = line.substr(0, firstSpace); // skip till first whitespace
		if(keyword == "o") // object has been found
		{
			if(state == ObjectSearch) // if the previous state was searching for an object, then prepare for the buildobject state
			{
				// Add last read mesh to model geometry
				if (verts.size() > 0)
				{
					Mesh loadedMesh(finalVerts, true); // temporary mesh, don't index vertices
					loadedMesh.SetIndices(finalIndices); // pass vertex indices (TODO: these don't actually perform index so could remove this to save memory)
					mGeometry.push_back(loadedMesh);
				}

				// Reset data & state to prepare for next mesh
				verts.clear();
				normals.clear();
				indices.clear();
				finalVerts.clear();
				finalIndices.clear();
				indexCounter = 0;
				state = BuildObject;
			}
			else
				state = ObjectSearch; // TODO: this logic seems to make no sense... rewrite?
		}
		else if(keyword == "v" || keyword == "vn" || keyword == "vt") // a vertex coordinate (position, normal or texture uv) is found
		{
			Vertex v;
			data = line.substr(firstSpace+1); // skip the keyword and first whitespace
			string xyz[3] = { "", "", "" }; // XYZ (or XY if keyword is vt) coordinates from file
			for(int i = 0; i < 3 || (keyword == "vt" && i < 2); i++) // read all 3 (or 2 if vt) coordinates from the line
			{
				xyz[i] = data.substr(0, data.find(" "));
				data = data.substr(data.find(" ")+1);
			}
			// Assign coordinates to the Vertex in accordance to coordinate type (pos/normal/uv)
			// TODO: Wasting memory here. In case of vt, there are 6 unused floats - that's 24 bytes per vertex. Should get rid of separate vectors too.
			if(keyword == "v")
			{
				v.x = stof(xyz[0]);
				v.y = stof(xyz[1]);
				v.z = stof(xyz[2]);
				verts.push_back(v);
			}
			else if(keyword == "vn")
			{
				v.nX = stof(xyz[0]);
				v.nY = stof(xyz[1]);
				v.nZ = stof(xyz[2]);
				normals.push_back(v);
			}
			else if(keyword == "vt")
			{
				v.s = stof(xyz[0]);
				v.t = stof(xyz[1]);
				uvs.push_back(v);
			}
		}
		else if(keyword == "f") // a face (triangle) has been found, this is where indices are taken from
		{
			data = line + " "; // current line + extra whitespace (a quick hack that will be used for loading normals further below, search "normalIndex")
			for(int i = 0; i < 3; i++) // The .obj file should be storing faces as triangles (NOT quads) for this to work
			{
				data = data.substr(data.find(" ")+1); // skips "f " to start reading the integers
				string vertexIndex = data.substr(0, data.find("/")); // copy position index
				data = data.substr(data.find("/")+1); // skips till after slash to read next index type
				// next up should be the vertex UV coord index, which might be empty
				string uvIndex = "";
				if(data.find("/") != 0) // UV coord index is not empty (e.g. `f 1/1/1`)
				{
					uvIndex = data.substr(0, data.find("/")); // copy uv index
					data = data.substr(data.find("/")+1); // skips to next index type
				}
				else // UV coord index is empty (e.g. `f 1//1`)
					data = data.substr(1); // skips to next index type
				// next up is the vertex normal index, which is the last one for this vertex. The first two vertices for this face will be separated by a whitespace, while the last one ends with a newline, so that's why there was an extra whitespace added to data earlier.
				string normalIndex = data.substr(0, data.find(" "));

				// Convert all indices from strings to integers
				unsigned long long vIdx = stoll(vertexIndex);
				unsigned long long uvIdx = 0;
				if(uvIndex != "") // check if uv index was even found before converting
					uvIdx = stoll(uvIndex);
				unsigned long long nIdx = stoll(normalIndex);

				indices.push_back(indexCounter++); // add incrementing index

				// finalVerts will store each vertex (no matter if it's duplicated - it ignores indexing)
				// the indices are zero-based (hence the v/n/uvIdx - 1). indexCounter was just incremented hence -1
				// The reason why indexing code is even here is to find out which vertex is the normal/UV coordinate is associated with
				// the .obj file stores vertex positions, normals and uv separately yet they're linked together by indices
				// TODO: this is not the most efficient way of doing things (and certainly not an elegant one) but it works for now
				finalVerts.push_back(verts[vIdx-1]);
				finalVerts[indexCounter-1].nX = normals[nIdx-1].nX;
				finalVerts[indexCounter-1].nY = normals[nIdx-1].nY;
				finalVerts[indexCounter-1].nZ = normals[nIdx-1].nZ;
				if(uvIdx != 0)
				{
					finalVerts[indexCounter-1].s = uvs[uvIdx-1].s;
					finalVerts[indexCounter-1].t = uvs[uvIdx-1].t;
				}
			}
		}
	}

	// Add last mesh found in the file (as there is no "o" keyword afterwards to trigger the adding code)
	Mesh loadedMesh(finalVerts, true);
	loadedMesh.SetIndices(indices);
	mGeometry.push_back(loadedMesh);

	// Remove all data from vectors
	verts.clear();
	normals.clear();
	uvs.clear();
	indices.clear();
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
