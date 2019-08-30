#ifndef _LEPUSENGINE_PHYSICSRIGIDBODY_
#define _LEPUSENGINE_PHYSICSRIGIDBODY_

#define NDEBUG

#include "../Physics.h"
#include "Lepus3D/Source/Vertex.h"

namespace LepusEngine
{
    namespace Physics {
        class Rigidbody {
        private:
            physx::PxMaterial* m_PxMat;
            physx::PxGeometry* m_PxCollider;
            physx::PxRigidDynamic* m_PxRigidbody;
        public:
            Rigidbody() { m_PxMat = nullptr; m_PxRigidbody = nullptr; m_PxCollider = nullptr; }
            Rigidbody(Mesh& geometry);
            void InitCollider(Mesh& geometry);
        };

        class PhysxColliderMeshData : public physx::PxInputStream {
        private:
            void* m_Vertices;
            uint32_t m_VertexCount;
        public:
            virtual uint32_t read(void* dest, uint32_t count);
            PhysxColliderMeshData(Mesh& mesh);
            virtual ~PhysxColliderMeshData();
        };
    }
}

#endif