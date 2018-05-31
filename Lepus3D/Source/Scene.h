#pragma once

#include "Renderable.h"
#include "Transform.h"
#include <vector>>

namespace LepusEngine {
	namespace Lepus3D {
		class Scene {
		friend class RenderEngine;
		private:
			std::vector<Renderable*> m_ObjArr;
		public:
			Scene();

			// Returns the index of the newly added mesh
			int AddMesh(Mesh mesh, Transform t = Transform(Vector3::Zero(), Vector3::Zero(), Vector3::Create(1.f, 1.f, 1.f)));
			int AddMesh(Renderable* renderable);
			// Returns the length of the scene's MeshPack
			int GetSize();
			// Removes a mesh from the scene at a specified index
			void RemoveMesh(int index);
		};
	}
}
