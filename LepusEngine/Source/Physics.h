#ifndef _LEPUSENGINE_PHYSICS_
#define _LEPUSENGINE_PHYSICS_

#include "PxPhysicsAPI.h"
#include "extensions/PxDefaultAllocator.h"
#include "../../Lepus3D/Source/Scene.h"

#define LEPUS_PHYSICS_DEFAULT_GRAVITY -9.81f

namespace LepusEngine 
{
    class Physics 
    {
    private:
        physx::PxDefaultAllocator m_Allocator;
        physx::PxDefaultErrorCallback m_ErrCb;
        physx::PxFoundation* m_Foundation;
        physx::PxPhysics* m_PxPhysics;
        physx::PxDefaultCpuDispatcher* m_Dispatcher;
        physx::PxScene* m_PxScene;
        
        Lepus3D::Scene* m_Scene;

        bool m_Ready;
    public:
        // Initialise pointers with null
        Physics() { m_Ready = false; m_Foundation = nullptr; m_PxPhysics = nullptr; m_Dispatcher = nullptr; m_PxScene = nullptr; m_Scene = nullptr; }

        // Map a Lepus3D scene to the physics simulation
        Physics(Lepus3D::Scene& scene);

        void Init(Lepus3D::Vector3 gravity = Lepus3D::Vector3(0.0f, LEPUS_PHYSICS_DEFAULT_GRAVITY, 0.0f));
        
        void Run(float rate);

        void Shutdown();

        ~Physics();
    };
}

#endif