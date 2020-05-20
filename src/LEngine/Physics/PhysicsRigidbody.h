#pragma once

#include "../Physics.h"
#include <L3D/Vertex.h>

namespace LepusEngine
{
	typedef btRigidBody RigidDynamic;
        class PhysicsRigidbody {
			friend class Physics;
        private:
			// TODO:
			// physic material?
			// geometry?
			// rigidbody object?

			// TODO:
			//PhysicsMaterial mMaterial;
			
			btCollisionShape* mBtCollider;
			btRigidBody* mBtRigidbody;
			btMotionState* mBtMotionState;
            
			bool m_Active;
			Physics* m_PhysicsEngine;
        public:
			PhysicsRigidbody() {
				// Initialise material, geometry and rigidbody 
				m_Active = false; m_PhysicsEngine = nullptr; mBtCollider = nullptr; mBtRigidbody = nullptr; mBtMotionState = nullptr;
			}
            PhysicsRigidbody(Physics& physicsEngine, Lepus3D::Mesh& geometry, Lepus3D::Transform& transform);
            void InitCollider(Physics& physicsEngine, Lepus3D::Mesh& geometry, Lepus3D::Transform& transform);
			RigidDynamic* const GetDynamic();
			Lepus3D::Transform GetTransform();
			void SetActive(bool active);
			bool IsActive();
			~PhysicsRigidbody();
        };
}