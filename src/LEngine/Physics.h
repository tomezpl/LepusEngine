#pragma once

#include <L3D/Scene.h>
#include <bullet/btBulletDynamicsCommon.h>

#define LEPUS_PHYSICS_DEFAULT_GRAVITY -9.81f

#ifndef _DEBUG
#define NDEBUG
#endif

namespace LepusEngine 
{
    class Physics 
    {
		friend class PhysicsRigidbody;
		friend class Lepus3D::Scene;
    private:
        // TODO: Delete. Keep references on each object in the scene instead - easier than syncing with the Lepus3D scene.
        Lepus3D::Scene* m_Scene;

        bool m_Ready;


		btCollisionConfiguration* mCollisionConfig;
		btCollisionDispatcher* mCollisionDispatch;
		btBroadphaseInterface* mBroadphase;
		btConstraintSolver* mConstraintSolver;

		// Physics world
		btDiscreteDynamicsWorld* mWorld;
    public:
        // Initialise pointers with null
		Physics() { m_Ready = false; m_Scene = nullptr; }

        // Map a Lepus3D scene to the physics simulation
        Physics(Lepus3D::Scene& scene);

        void Init(Lepus3D::Vector3 gravity = Lepus3D::Vector3(0.0f, LEPUS_PHYSICS_DEFAULT_GRAVITY, 0.0f));

		void AddObject(PhysicsRigidbody& rigidbody);
        
        void Run(float deltaTime);

        void Shutdown();

        ~Physics();
    };
}