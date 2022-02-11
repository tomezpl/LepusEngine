#pragma once

class btCollisionShape;
class btRigidBody;
class btMotionState;

namespace LepusEngine
{
	namespace Lepus3D
	{
		class Transform;
		class Mesh;
	}

	class Physics;


        class PhysicsRigidbody {
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
			PhysicsRigidbody();
            PhysicsRigidbody(Physics& physicsEngine, Lepus3D::Mesh& geometry, Lepus3D::Transform& transform, float mass = 1.f);
            void InitCollider(Physics& physicsEngine, Lepus3D::Mesh& geometry, Lepus3D::Transform& transform);

			// Returns the underlying Bullet3 rigidbody data
			btRigidBody* const GetBtRigidbody();

			Lepus3D::Transform GetTransform();
			void SetActive(bool active);
			bool IsActive();

			void SetPhysicsEngine(Physics* physicsEngine);

			~PhysicsRigidbody();
        };
}