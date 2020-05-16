#pragma once

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

	// TODO: Swap out for Bullet
	// Bullet3 API class. Contains core objects needed for Physics classes to instantiate.
	class APIBullet
	{
		friend class Physics;
		friend class PhysicsRigidbody;
	private:

	public:
		APIBullet();
		~APIBullet();
	};

    class Physics 
    {
		friend class PhysicsRigidbody;
		friend class Lepus3D::Scene;
    private:
        
        Lepus3D::Scene* m_Scene;

        bool m_Ready;

		APIBullet* m_API;
    public:
        // Initialise pointers with null
		Physics() { m_Ready = false; m_API = nullptr; /* CPU dispatcher and physic scene */ m_Scene = nullptr; }

        // Map a Lepus3D scene to the physics simulation
        Physics(APIBullet& api, Lepus3D::Scene& scene);

        void Init(Lepus3D::Vector3 gravity = Lepus3D::Vector3(0.0f, LEPUS_PHYSICS_DEFAULT_GRAVITY, 0.0f));

		void AddObject(PhysicsRigidbody& rigidbody);
        
        void Run(float rate);

        void Shutdown();

        ~Physics();
    };
}