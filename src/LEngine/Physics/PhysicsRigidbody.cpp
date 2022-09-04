#include "PhysicsRigidbody.h"
#include <LEngine/Logger.h>
//#include <L3D/Vertex.h>
//#include <L3D/Transform.h>
#include <algorithm>
#include <bullet/btBulletDynamicsCommon.h>
//#include <L3D/Mesh.h>

using namespace LepusEngine;

PhysicsRigidbody::PhysicsRigidbody()
{
	// Initialise material, geometry and rigidbody 
	m_Active = false; 
	m_PhysicsEngine = nullptr; 
	mBtCollider = nullptr; 
	mBtRigidbody = nullptr; 
	mBtMotionState = nullptr;
}

PhysicsRigidbody::PhysicsRigidbody(Physics& physicsEngine, Lepus3D::Geometry* geometry, Lepus3D::Transform& transform, float mass)
{
	m_PhysicsEngine = &physicsEngine;

    InitCollider(physicsEngine, geometry, transform);
	//m_PxMat = physicsEngine.m_API->m_PxPhysics->createMaterial(0.5f, 0.5f, 0.6f); // TODO: these are only test parameters - change them, or provide a way to override.
	//TODO: Lepus3D::Vector3 position = transform.GetPosition();
	//TODO: Lepus3D::Quaternion rot = transform.GetRotation();
	//TODO: Lepus3D::Vector3 pos = transform.GetPosition();

	btAssert((!mBtCollider || mBtCollider->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	btVector3 localInertia(0.f, 0.f, 0.f);
	mBtCollider->calculateLocalInertia(1.f, localInertia);

	//mBtMotionState = new btDefaultMotionState(btTransform(btQuaternion(rot.x, rot.y, rot.z, rot.w), btVector3(pos.x, pos.y, pos.z)));
	mBtRigidbody = new btRigidBody(mass, /*mBtMotionState*/nullptr, mBtCollider, localInertia);
	/*m_PxRigidbody = physx::PxCreateDynamic(*physicsEngine.m_API->m_PxPhysics, physx::PxTransform(position.x, position.y, position.z), *m_PxCollider, *m_PxMat, 10.0f);
	m_PxRigidbody->setAngularDamping(0.5f);
	m_PxRigidbody->setLinearVelocity(physx::PxVec3(0.0f));*/
}

void PhysicsRigidbody::InitCollider(Physics& physicsEngine, Lepus3D::Geometry* geometry, Lepus3D::Transform& transform)
{
	/*physx::PxDefaultMemoryOutputStream outStream;
	physx::PxConvexMeshDesc desc;

	desc.points.count = geometry.GetVertexCount();*/
	// Initialise a float array for the data. count * 3 because we're only storing xyz of each vertex - UVs and the normals don't matter.
	float* pointsData = /*geometry->CopyXYZ()*/0;
	// TODO
	/*desc.points.data = pointsData;
	desc.points.stride = sizeof(float) * 3;
	
	desc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;
	physicsEngine.m_API->m_PxCooking->cookConvexMesh(desc, outStream);

	physx::PxDefaultMemoryInputData convexMeshData(outStream.getData(), outStream.getSize());
	physx::PxConvexMesh* mesh = physicsEngine.m_API->m_PxPhysics->createConvexMesh(convexMeshData);*/
	//TODO: Lepus3D::Vector3 scale = transform.GetScale();
	//m_PxCollider = new physx::PxConvexMeshGeometry(mesh, physx::PxMeshScale(physx::PxVec3(scale.x, scale.y, scale.z)));
	//TODO: mBtCollider = new btConvexHullShape(pointsData, geometry->GetVertexCount(), 3 * sizeof(float));
	//TODO: mBtCollider->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
#ifdef _DEBUG
	/*int nbVertices = mesh->getNbVertices();
	const physx::PxVec3* cookedVertices = mesh->getVertices();
	Logger::LogInfo("PhysicsRigidbody", "InitCollider", "Initialised collider geometry from Lepus3D::Mesh. Below are the cooked vertices from PhysX:");
	for (int i = 0; i < nbVertices; i++)
	{
		Logger::LogInfo("PhysicsRigidbody", "InitCollider", (char*)(std::string().append(std::to_string(cookedVertices[i].x))
			.append(", ").append(std::to_string(cookedVertices[i].y))
			.append(", ").append(std::to_string(cookedVertices[i].z)).c_str()));
	}*/
#endif
	delete[] pointsData;
}

void LepusEngine::PhysicsRigidbody::SetActive(bool active)
{
	// Sets active status and removes/adds rigidbody as actor to scene if necessary

	bool oldActive = m_Active;

	m_Active = active; // set object status

	if (oldActive != m_Active)
	{
		if (!m_Active)
		{
			//m_PhysicsEngine->m_PxScene->removeActor(*m_PxRigidbody);
		}
		else
		{
			//m_PhysicsEngine->m_PxScene->addActor(*m_PxRigidbody);
		}
	}
}

bool LepusEngine::PhysicsRigidbody::IsActive()
{
	return m_Active;
}

void LepusEngine::PhysicsRigidbody::SetPhysicsEngine(LepusEngine::Physics* physicsEngine)
{
	m_PhysicsEngine = physicsEngine;
}

btRigidBody* const LepusEngine::PhysicsRigidbody::GetBtRigidbody()
{
	return mBtRigidbody;
	//return m_PxRigidbody;
}

/*Lepus3D::Transform LepusEngine::PhysicsRigidbody::GetTransform()
{
	Lepus3D::Transform ret;
	btTransform pose = mBtRigidbody->getWorldTransform();
	btVector3 position = pose.getOrigin();
	btQuaternion rotation = pose.getRotation();
	ret.SetPosition(Lepus3D::Vector3(position.getX(), position.getY(), position.getZ()));
	ret.SetRotation(Lepus3D::Quaternion(rotation.x(), rotation.y(), rotation.z(), rotation.w()));
	btVector3 localScale = mBtRigidbody->getCollisionShape()->getLocalScaling();
	ret.SetScale(Lepus3D::Vector3(localScale.x(), localScale.y(), localScale.z()));

	return ret;
}
*/
LepusEngine::PhysicsRigidbody::~PhysicsRigidbody()
{
	
}
