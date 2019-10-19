#pragma once

#include "PxPhysicsAPI.h"
#include "extensions/PxDefaultAllocator.h"
#include <L3D/Scene.h>

#define LEPUS_PHYSICS_DEFAULT_GRAVITY -9.81f

#ifndef _DEBUG
#define NDEBUG
#endif

namespace LepusEngine 
{
	// Forward declaration to mark them as friend classes
	class PhysicsRigidbody;
	class Physics;

	// PhysX API class. Contains core objects needed for Physics classes to instantiate.
	class APIPhysX
	{
		friend class Physics;
		friend class PhysicsRigidbody;
	private:
		physx::PxDefaultAllocator m_Allocator;
		physx::PxDefaultErrorCallback m_ErrCb;
		physx::PxFoundation* m_Foundation;
		physx::PxPhysics* m_PxPhysics;
		physx::PxCooking* m_PxCooking;
	public:
		APIPhysX();
		~APIPhysX();
	};

    class Physics 
    {
		friend class PhysicsRigidbody;
		friend class Lepus3D::Scene;
    private:
        physx::PxDefaultCpuDispatcher* m_Dispatcher;
        physx::PxScene* m_PxScene;
        
        Lepus3D::Scene* m_Scene;

        bool m_Ready;

		APIPhysX* m_API;
    public:
        // Initialise pointers with null
		Physics() { m_Ready = false; m_API = nullptr; m_Dispatcher = nullptr; m_PxScene = nullptr; m_Scene = nullptr; }

        // Map a Lepus3D scene to the physics simulation
        Physics(APIPhysX& api, Lepus3D::Scene& scene);

        void Init(Lepus3D::Vector3 gravity = Lepus3D::Vector3(0.0f, LEPUS_PHYSICS_DEFAULT_GRAVITY, 0.0f));

		void AddObject(PhysicsRigidbody& rigidbody);
        
        void Run(float rate);

        void Shutdown();

        ~Physics();
    };
}