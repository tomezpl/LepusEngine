#include "LEngine/Physics.h"
#include "LEngine/Physics/PhysicsRigidbody.h"

using namespace LepusEngine;

Physics::Physics()
{
	this->Init();
	//m_Scene->m_PhysicsEngine = this;
}

void Physics::Init(Lepus3D::Vector3 gravity)
{
	mCollisionConfig = new btDefaultCollisionConfiguration();
	mCollisionDispatch = new btCollisionDispatcher(mCollisionConfig);
	mBroadphase = new btDbvtBroadphase();
	mConstraintSolver = new btSequentialImpulseConstraintSolver();

	// TODO: Generalise for adapting to Discrete/CCD?
	mWorld = new btDiscreteDynamicsWorld(mCollisionDispatch, mBroadphase, mConstraintSolver, mCollisionConfig);
	mWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));

	size_t renderableCount = 0/*m_Scene->GetRenderableCount()*/;

	for (size_t i = 0; i < renderableCount; i++)
	{
		//PhysicsRigidbody* rigidbody = m_Scene->m_ObjArr[i]->GetPhysicsRigidbody();
		//m_PxScene->addActor(*rigidbody->m_PxRigidbody);
	}
}

void LepusEngine::Physics::AddObject(PhysicsRigidbody & rigidbody)
{
	//m_PxScene->addActor(*rigidbody.m_PxRigidbody);
	mWorld->addRigidBody(rigidbody.GetBtRigidbody());
	rigidbody.SetPhysicsEngine(this);
}

void Physics::Run(float dTime)
{
	/*m_PxScene->simulate(1.0f / rate);
	m_PxScene->fetchResults(true);*/
	mWorld->stepSimulation(dTime);
}

void Physics::Shutdown()
{
	/*m_PxScene->release();
	m_Dispatcher->release();*/

	// Delete Bullet resources
	delete mWorld;
	delete mConstraintSolver;
	delete mBroadphase;
	delete mCollisionDispatch;
	delete mCollisionConfig;
}

Physics::~Physics()
{
	Shutdown();
}