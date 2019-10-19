#include "LEngine/Physics.h"
#include "LEngine/Physics/PhysicsRigidbody.h"

using namespace LepusEngine;
using namespace physx;

APIPhysX::APIPhysX()
{
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrCb);

	m_PxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), true);

	physx::PxTolerancesScale toleranceScale = m_PxPhysics->getTolerancesScale();
	physx::PxCookingParams cookingParams(toleranceScale);
	m_PxCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, cookingParams);
}

APIPhysX::~APIPhysX()
{
	m_PxPhysics->release();
	m_PxCooking->release();
	m_Foundation->release();
}

Physics::Physics(APIPhysX& api, Lepus3D::Scene& scene) : Physics()
{
	m_API = &api;
    m_Scene = &(scene);
	this->Init();
	m_Scene->m_PhysicsEngine = this;
}

void Physics::Init(Lepus3D::Vector3 gravity)
{
	PxSceneDesc sceneDesc(m_API->m_PxPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(gravity.x, gravity.y, gravity.z);
	m_Dispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_Dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	m_PxScene = m_API->m_PxPhysics->createScene(sceneDesc);

	size_t renderableCount = m_Scene->GetRenderableCount();

	for (size_t i = 0; i < renderableCount; i++)
	{
		PhysicsRigidbody* rigidbody = m_Scene->m_ObjArr[i]->GetPhysicsRigidbody();
		m_PxScene->addActor(*rigidbody->m_PxRigidbody);
	}
}

void LepusEngine::Physics::AddObject(PhysicsRigidbody & rigidbody)
{
	m_PxScene->addActor(*rigidbody.m_PxRigidbody);
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
	
	m_Scene = nullptr;
}

Physics::~Physics()
{
	Shutdown();
}