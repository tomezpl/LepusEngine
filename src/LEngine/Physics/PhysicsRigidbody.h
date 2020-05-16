#pragma once

#include "../Physics.h"
#include <L3D/Vertex.h>

namespace LepusEngine
{
	typedef void RigidDynamic;
        class PhysicsRigidbody {
			friend class Physics;
        private:
			// TODO:
			// physic material?
			// geometry?
			// rigidbody object?
            
			bool m_Active;
			Physics* m_PhysicsEngine;
        public:
			PhysicsRigidbody() {
				// Initialise material, geometry and rigidbody 
				m_Active = false; m_PhysicsEngine = nullptr;
			}
            PhysicsRigidbody(Physics& physicsEngine, Lepus3D::Mesh& geometry, Lepus3D::Transform& transform);
            void InitCollider(Physics& physicsEngine, Lepus3D::Mesh& geometry, Lepus3D::Transform& transform);
			RigidDynamic* const GetDynamic();
			void SetActive(bool active);
			bool IsActive();
			~PhysicsRigidbody();
        };
}