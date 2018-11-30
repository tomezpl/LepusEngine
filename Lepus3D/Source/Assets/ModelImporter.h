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
				std::vector<Mesh> mGeometry;
				bool _ReadOBJ(); // read data from .obj file
			public:
				ModelImporter();
				ModelImporter(std::string filename, std::string directory = "../../Content");
				bool Init(std::string filename, std::string directory = "../../Content");
				bool Read();
				Mesh GetSubMesh(int meshIndex = 0);
				bool Close();
				~ModelImporter();
			};
		}
	}
}

#endif
