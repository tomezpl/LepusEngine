#ifndef _LEPUS3D_RENDERABLE_
#define _LEPUS3D_RENDERABLE_

#include "Mesh.h"
#include "Transform.h"

namespace LepusEngine {
	namespace Lepus3D {
		class Renderable : public Transformable {
		friend class Scene;
		friend class RenderEngine;
		protected:
			Mesh* mMesh;
		public:
			Renderable();
			Renderable(Mesh mesh);
			Mesh* GetMesh();
			void SetMesh(Mesh mesh);
			~Renderable();
		};
	}
}

#endif
