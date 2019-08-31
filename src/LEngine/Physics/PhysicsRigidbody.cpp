#include "PhysicsRigidbody.h"
#include <algorithm>

using namespace LepusEngine;

PhysxColliderMeshData::PhysxColliderMeshData(Lepus3D::Mesh& mesh)
{
    this->m_Vertices = mesh.m_Vertices.data();
    this->m_VertexCount = mesh.m_Vertices.size();
}

uint32_t PhysxColliderMeshData::read(void* dest, uint32_t count)
{
    memcpy(dest, m_Vertices, std::min(count, m_VertexCount));
	return 0; // TODO
}

PhysicsRigidbody::PhysicsRigidbody(Lepus3D::Mesh& geometry)
{
    InitCollider(geometry);
}

void PhysicsRigidbody::InitCollider(Lepus3D::Mesh& geometry)
{
    physx::PxTriangleMesh* mesh;
    //PxPhysics::createTriangleMesh()
    m_PxCollider = new physx::PxTriangleMeshGeometry();
}