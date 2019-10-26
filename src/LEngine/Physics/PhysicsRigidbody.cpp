#include "PhysicsRigidbody.h"
#include <LEngine/Logger.h>
#include <algorithm>

using namespace LepusEngine;

PhysxColliderMeshData::PhysxColliderMeshData(Lepus3D::Mesh& mesh)
{
    this->m_Vertices = mesh.m_Vertices.data();
    this->m_VertexCount = mesh.m_Vertices.size();
	this->m_ByteOffset = 0;
}

LepusEngine::PhysxColliderMeshData::~PhysxColliderMeshData()
{
}

uint32_t PhysxColliderMeshData::read(void* dest, uint32_t count)
{
	// Number of bytes to copy. Choose either provided count or the actual vector length (whichever is smaller) to avoid going out of bounds.
	physx::PxU32 nbBytes = physx::PxMin<physx::PxU32>(count, (m_VertexCount * sizeof(Lepus3D::Vertex)) - m_ByteOffset);
	physx::PxMemCopy(dest, static_cast<unsigned char*>(this->m_Vertices) + m_ByteOffset, nbBytes);
	m_ByteOffset += nbBytes;
	return nbBytes;
}

PhysicsRigidbody::PhysicsRigidbody(Physics& physicsEngine, Lepus3D::Mesh& geometry, Lepus3D::Transform& transform)
{
    InitCollider(physicsEngine, geometry, transform);
	m_PxMat = physicsEngine.m_API->m_PxPhysics->createMaterial(0.5f, 0.5f, 0.6f); // TODO: these are only test parameters - change them, or provide a way to override.
	Lepus3D::Vector3 position = transform.GetPosition();
	m_PxRigidbody = physx::PxCreateDynamic(*physicsEngine.m_API->m_PxPhysics, physx::PxTransform(position.x, position.y, position.z), *m_PxCollider, *m_PxMat, 10.0f);
	m_PxRigidbody->setAngularDamping(0.5f);
	m_PxRigidbody->setLinearVelocity(physx::PxVec3(0.0f));
}

void PhysicsRigidbody::InitCollider(Physics& physicsEngine, Lepus3D::Mesh& geometry, Lepus3D::Transform& transform)
{
	physx::PxDefaultMemoryOutputStream outStream;
	physx::PxConvexMeshDesc desc;

	desc.points.count = geometry.GetVertexCount();
	// Initialise a float array for the data. count * 3 because we're only storing xyz of each vertex - UVs and the normals don't matter.
	float* pointsData = new float[unsigned long long(geometry.GetVertexCount()) * 3];
	float* vertices = geometry.GetVertexBuffer();
	// Copy first 3 floats (xyz) from each Vertex
	for (unsigned int i = 0, j = 0; i < geometry.GetVertexCount() * 3, j < geometry.GetVertexCount() * 8; i += 3, j += 8)
	{
		memcpy(pointsData + i, vertices + j, sizeof(float) * 3);
	}
	delete vertices;
	desc.points.data = pointsData;
	desc.points.stride = sizeof(float) * 3;
	
	desc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;
	physicsEngine.m_API->m_PxCooking->cookConvexMesh(desc, outStream);

	physx::PxDefaultMemoryInputData convexMeshData(outStream.getData(), outStream.getSize());
	physx::PxConvexMesh* mesh = physicsEngine.m_API->m_PxPhysics->createConvexMesh(convexMeshData);
	Lepus3D::Vector3 scale = transform.GetScale();
	m_PxCollider = new physx::PxConvexMeshGeometry(mesh, physx::PxMeshScale(physx::PxVec3(scale.x, scale.y, scale.z)));
#ifdef _DEBUG
	int nbVertices = mesh->getNbVertices();
	const physx::PxVec3* cookedVertices = mesh->getVertices();
	Logger::LogInfo("PhysicsRigidbody", "InitCollider", "Initialised collider geometry from Lepus3D::Mesh. Below are the cooked vertices from PhysX:");
	for (int i = 0; i < nbVertices; i++)
	{
		Logger::LogInfo("PhysicsRigidbody", "InitCollider", (char*)(std::string().append(std::to_string(cookedVertices[i].x))
			.append(", ").append(std::to_string(cookedVertices[i].y))
			.append(", ").append(std::to_string(cookedVertices[i].z)).c_str()));
	}
#endif
	delete[] pointsData;
}

physx::PxRigidDynamic* const LepusEngine::PhysicsRigidbody::GetDynamic()
{
	return m_PxRigidbody;
}

LepusEngine::PhysicsRigidbody::~PhysicsRigidbody()
{
	
}
