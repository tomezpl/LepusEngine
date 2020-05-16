#pragma once

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
				std::vector<Mesh> mGeometry; // Geometry set
				bool _ReadOBJ(); // reads data from .obj file
			public:
				// Creates a .obj model file importer instance
				ModelImporter();

				// Creates a .obj model file importer instance, and initialises it.
				ModelImporter(std::string filename, std::string directory = "../../Content");

				// Initialises the instance, and prepares file handles for reading.
				// Returns true on success, false on error.
				bool Init(std::string filename, std::string directory = "../../Content");

				// Performs model file parsing & reading. Returns true on success, false on error.
				// Instance must be initialised first.
				bool Read();

				// Returns copy of submesh from the geometry list at specified index (default: 0).
				// Use the default index if there is only one mesh.
				Mesh GetSubMesh(int meshIndex = 0);

				// Closes file handles and frees their memory.
				// To reuse this instance after calling this, it needs to be initialised again.
				bool Close();

				// Default destructor
				~ModelImporter();
			};
		}
	}
}