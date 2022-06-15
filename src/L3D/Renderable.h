#pragma once

#include <L3D/Transform.h>

namespace LepusEngine {

	namespace Lepus3D {
		class Mesh;

		class Renderable : public Transformable {
		friend class Scene;
		friend class RenderEngine;
		protected:
			Mesh* mMesh;
		public:
			Renderable();
			Renderable(Mesh& mesh);
			Mesh* GetMesh();
			void SetMesh(Mesh& mesh);
			~Renderable();
		};
	}
}
