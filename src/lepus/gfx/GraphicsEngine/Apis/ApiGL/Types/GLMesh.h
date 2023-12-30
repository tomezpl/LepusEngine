#ifndef LEPUS_GFX_GL_MESH
#define LEPUS_GFX_GL_MESH

#include <lepus/engine/Objects/Mesh.h>
#include <GL/gl3w.h>

namespace lepus
{
    namespace gfx
    {
        class GLMesh : protected lepus::engine::objects::Mesh
        {
            private:
            GLuint m_VBO;
            GLuint m_IBO;

            bool m_HasVBO;
            bool m_HasIBO;

            void _CreateVBO();
            void _CreateIBO();

            protected:
            inline void InitInternal() override
            {
                m_VBO = 0;
                m_IBO = 0;
                m_HasIBO = false;
                m_HasVBO = false;

                // Avoid creating a VBO or IBO if the size is 0 - this is likely because the mesh is being created on the stack
                // before a VAO was bound. This will only result in a crash.
                if (VertexBufferSize() > 0)
                {
                    _CreateVBO();
                }

                if (IndexCount() > 0)
                {
                    _CreateIBO();
                }
            }

            inline void CopyInternal(const GLMesh& other)
            {
                Mesh::CopyInternal(other);
                InitInternal();
            }

            public:
            LEPUS_MESH_CONSTRUCTOR(GLMesh);

            GLMesh(const GLMesh& other)
            {
                CopyInternal(other);
            }

            GLMesh(GLMesh&& other)
            {
                *this = std::move(other);
            }

            GLMesh& operator=(const GLMesh& other)
            {
                CopyInternal(other);
                return *this;
            }

            GLMesh& operator=(GLMesh&& other)
            {
                *((Mesh*)this) = (Mesh)other;

                m_IBO = other.m_IBO;
                m_VBO = other.m_VBO;
                other.m_HasIBO = false;
                other.m_HasVBO = false;

                return *this;
            }

            inline GLuint GetVBO() const { return m_VBO; }
            inline GLuint GetIBO() const { return m_IBO; }

            inline void Dispose() override
            {
                Mesh::Dispose();

                if (m_HasVBO)
                {
                    glDeleteBuffers(1, &m_VBO);
                    m_HasVBO = false;
                }

                if (m_HasIBO)
                {
                    glDeleteBuffers(1, &m_IBO);
                    m_HasIBO = false;
                }
            }
        };
    }
}

#endif