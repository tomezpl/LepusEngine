#pragma once

#include "Mesh.h"
#include "Transform.h"

namespace LepusEngine {
	class PhysicsRigidbody;
	class Physics;

	namespace Lepus3D {
		class Renderable : public Transformable {
		friend class Scene;
		friend class RenderEngine;
		protected:
			Mesh* mMesh;
			::LepusEngine::PhysicsRigidbody* mRigidbody;
		public:
			Renderable();
			Renderable(Mesh mesh, bool enablePhysics = false, Physics* physicsEngine = nullptr);
			::LepusEngine::PhysicsRigidbody* GetPhysicsRigidbody();
			Mesh* GetMesh();
			void SetMesh(Mesh mesh, bool enablePhysics = false, Physics* physicsEngine = nullptr);
			~Renderable();
		};
	}
}
