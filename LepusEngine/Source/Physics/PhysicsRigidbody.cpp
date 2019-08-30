#include "PhysicsRigidbody.h"
#include <cmath>

using namespace LepusEngine;
using namespace Lepus3D;

Physics::PhysxColliderMeshData::PhysxColliderMeshData(Lepus3D::Mesh& mesh)
{
    m_Vertices = mesh.m_Vertices.data();
    m_VertexCount = mesh.m_Vertices.size();
}

uint32_t Physics::PhysxColliderMeshData::read(void* dest, uint32_t count)
{
    memcpy(dest, m_Vertices, min(count, m_VertexCount));
}

Physics::Rigidbody::Rigidbody(Mesh& geometry)
{
    InitCollider(geometry);
}

void Physics::Rigidbody::InitCollider(Mesh& geometry)
{
    physx::PxTriangleMesh* mesh;
    //PxPhysics::createTriangleMesh()
    m_PxCollider = new physx::PxTriangleMeshGeometry();
}