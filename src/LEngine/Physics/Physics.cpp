#include "../Physics.h"

using namespace LepusEngine;
using namespace physx;

Physics::Physics(Lepus3D::Scene& scene) : Physics()
{
    m_Scene = &(scene);
}

void Physics::Init(Lepus3D::Vector3 gravity)
{
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrCb);

	m_PxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), true);

	PxSceneDesc sceneDesc(m_PxPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(gravity.x, gravity.y, gravity.z);
	m_Dispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_Dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	m_PxScene = m_PxPhysics->createScene(sceneDesc);
}

void Physics::Run(float rate)
{
	m_PxScene->simulate(1.0f / rate);
	m_PxScene->fetchResults(true);
}

void Physics::Shutdown()
{
	m_PxScene->release();
	m_Dispatcher->release();
	m_PxPhysics->release();
	m_Foundation->release();
	
	m_Scene = nullptr;
}

Physics::~Physics()
{
	Shutdown();
}