#ifndef _LEPUS3D_ASSETS_MODELIMP_
#define _LEPUS3D_ASSETS_MODELIMP_

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "../Vertex.h"
#include "../Mesh.h"

namespace LepusEngine {
	namespace Lepus3D {
		namespace Assets {
			class ModelImporter {
			private:
				std::ifstream* mObjFile; // Wavefront OBJ file handle
				std::ifstream* mMtlFile; // Wavefront OBJ material library file handle
				bool _ReadOBJ(); // read data from .obj file
			public:
				ModelImporter();
				ModelImporter(std::string filename);
				bool Init(std::string filename);
				bool Read();
				Mesh *GetMeshes();
				bool Close();
				~ModelImporter();
			};
		}
	}
}

#endif
