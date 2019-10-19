#include "PhysicsRigidbody.h"
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
	//PhysxColliderMeshData stream(geometry);
	physx::PxDefaultMemoryOutputStream outStream;
	physx::PxConvexMeshDesc desc;
	unsigned long long indexCount;
	//desc.triangles.data = geometry.GetIndexBuffer(indexCount);
	//desc.triangles.count = indexCount;
	//desc.triangles.stride = sizeof(unsigned long long) * 3;
	desc.points.count = geometry.GetVertexCount();
	desc.points.data = geometry.GetVertexBuffer();
	desc.points.stride = sizeof(Lepus3D::Vertex);
	desc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;
	physicsEngine.m_API->m_PxCooking->cookConvexMesh(desc, outStream);

	physx::PxDefaultMemoryInputData convexMeshData(outStream.getData(), outStream.getSize());
	// TODO: the vertices are not supposed to be raw vertices, but rather some Mesh format of chars that PhysX uses.
	physx::PxConvexMesh* mesh = physicsEngine.m_API->m_PxPhysics->createConvexMesh(convexMeshData); // TODO: Crashes here. Possibly incorrect vertex stream.
	Lepus3D::Vector3 scale = transform.GetScale();
	m_PxCollider = new physx::PxConvexMeshGeometry(mesh, physx::PxMeshScale(physx::PxVec3(scale.x, scale.y, scale.z)));
}

physx::PxRigidDynamic* const LepusEngine::PhysicsRigidbody::GetDynamic()
{
	return m_PxRigidbody;
}

LepusEngine::PhysicsRigidbody::~PhysicsRigidbody()
{
	
}
