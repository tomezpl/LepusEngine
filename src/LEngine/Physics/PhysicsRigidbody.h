#pragma once

#include "../Physics.h"
#include <L3D/Vertex.h>

namespace LepusEngine
{
        class PhysicsRigidbody {
			friend class Physics;
        private:
            physx::PxMaterial* m_PxMat;
            physx::PxGeometry* m_PxCollider;
            physx::PxRigidDynamic* m_PxRigidbody;
        public:
			PhysicsRigidbody() { m_PxMat = nullptr; m_PxRigidbody = nullptr; m_PxCollider = nullptr; }
            PhysicsRigidbody(Physics& physicsEngine, Lepus3D::Mesh& geometry, Lepus3D::Transform& transform);
            void InitCollider(Physics& physicsEngine, Lepus3D::Mesh& geometry, Lepus3D::Transform& transform);
			physx::PxRigidDynamic* const GetDynamic();
			~PhysicsRigidbody();
        };

        class PhysxColliderMeshData : public physx::PxInputStream {
        private:
            void* m_Vertices;
            uint32_t m_VertexCount;
			uint32_t m_ByteOffset;
        public:
            virtual uint32_t read(void* dest, uint32_t count);
            PhysxColliderMeshData(Lepus3D::Mesh& mesh);
            virtual ~PhysxColliderMeshData();
        };
}