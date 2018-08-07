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
	string word = "";
	while(*mObjFile >> word)
		cout << word << endl;
	return true;
}

bool ModelImporter::Init(string fn)
{
	// navigate to content folder
	fn = "../../Content/" + fn;
	// filename for the .obj file
	string objFN = fn;
	// filename for the .mtl material library file
	string mtlFN = fn.substr(0, fn.length() - string(".obj").length());

	mObjFile = new ifstream(objFN);
	mMtlFile = new ifstream(mtlFN);

	return true;
}

bool ModelImporter::Read()
{
	return _ReadOBJ();
}

Mesh *ModelImporter::GetMeshes()
{
	return nullptr;
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
