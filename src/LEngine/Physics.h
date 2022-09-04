#pragma once

//#include <L3D/Scene.h>
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
    private:
        bool m_Ready;

		btCollisionConfiguration* mCollisionConfig;
		btCollisionDispatcher* mCollisionDispatch;
		btBroadphaseInterface* mBroadphase;
		btConstraintSolver* mConstraintSolver;

		// Physics world
		btDiscreteDynamicsWorld* mWorld;
    public:
        Physics();

        void Init(/*Lepus3D::Vector3 gravity = Lepus3D::Vector3(0.0f, LEPUS_PHYSICS_DEFAULT_GRAVITY, 0.0f)*/);

		void AddObject(PhysicsRigidbody& rigidbody);
        
        void Run(float deltaTime);

        void Shutdown();

        ~Physics();
    };
}