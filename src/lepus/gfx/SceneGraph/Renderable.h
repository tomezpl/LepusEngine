#ifndef LEPUS_GFX_SCENEGRAPH_RENDERABLE
#define LEPUS_GFX_SCENEGRAPH_RENDERABLE

#include <lepus/engine/Objects/Mesh.h>
#include <lepus/utility/types/Transform.h>

namespace lepus
{
    namespace gfx
    {
        template<class MeshType = lepus::engine::objects::Mesh>
        class Renderable
        {
            private:
            const MeshType* m_Mesh;
            const lepus::math::Transform* m_Transform;
            bool m_OwnsTransform;

            public:
            Renderable() = delete;

            Renderable(const MeshType* mesh, const lepus::math::Transform& transform)
            {
                m_Mesh = mesh;
                m_Transform = &transform;
                m_OwnsTransform = false;
            }

            Renderable(const MeshType* mesh, lepus::math::Transform&& transform)
            {
                m_Mesh = mesh;
                m_Transform = new lepus::math::Transform(transform);
                m_OwnsTransform = true;
            }

            ~Renderable()
            {
                m_Mesh = nullptr;

                if (m_OwnsTransform)
                {
                    delete m_Transform;
                    m_Transform = nullptr;
                }
            }

            const MeshType* GetMesh() const
            {
                return m_Mesh;
            }

            const lepus::math::Transform& GetTransform() const
            {
                return *m_Transform;
            }
        };
    }
}

#endif