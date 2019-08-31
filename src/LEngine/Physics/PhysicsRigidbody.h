#ifndef _LEPUSENGINE_PHYSICSRIGIDBODY_
#define _LEPUSENGINE_PHYSICSRIGIDBODY_

#define NDEBUG

#include "../Physics.h"
#include <L3D/Vertex.h>

namespace LepusEngine
{
        class PhysicsRigidbody {
        private:
            physx::PxMaterial* m_PxMat;
            physx::PxGeometry* m_PxCollider;
            physx::PxRigidDynamic* m_PxRigidbody;
        public:
            PhysicsRigidbody() { m_PxMat = nullptr; m_PxRigidbody = nullptr; m_PxCollider = nullptr; }
            PhysicsRigidbody(Lepus3D::Mesh& geometry);
            void InitCollider(Lepus3D::Mesh& geometry);
        };

        class PhysxColliderMeshData : public physx::PxInputStream {
        private:
            void* m_Vertices;
            uint32_t m_VertexCount;
        public:
            virtual uint32_t read(void* dest, uint32_t count);
            PhysxColliderMeshData(Lepus3D::Mesh& mesh);
            virtual ~PhysxColliderMeshData();
        };
}

#endif