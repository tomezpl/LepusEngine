#ifndef LEPUS_GFX_GL_MESH
#define LEPUS_GFX_GL_MESH

#include <lepus/engine/Objects/Mesh.h>
#include <GL/gl3w.h>

namespace lepus
{
    namespace gfx
    {
        /// @brief A Mesh specialisation to create and manage OpenGL resources for mesh data as needed.
        class GLMesh : protected lepus::engine::objects::Mesh
        {
            private:
            GLuint m_VBO;
            GLuint m_IBO;

            bool m_HasVBO;
            bool m_HasIBO;

            /// @brief Creates a Vertex Buffer Object for the mesh data.
            void _CreateVBO();
            /// @brief Creates an Index Buffer Object for the mesh data.
            void _CreateIBO();

            protected:
            inline void InitInternal() override
            {
                m_VBO = 0;
                m_IBO = 0;
                m_HasIBO = false;
                m_HasVBO = false;

                // Avoid creating a VBO or IBO if the size is 0 - this is likely because the mesh is being created on the stack
                // with the default constructor before a VAO was bound. This will only result in a crash.
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

            /// @brief Copy constructor
            /// @param other GLMesh object to copy.
            GLMesh(const GLMesh& other)
            {
                CopyInternal(other);
            }

            /// @brief Move constructor. Moves all base Mesh data (including vertices and indices) as well as OpenGL buffer names.
            /// @param other GLMesh rvalue to move data from.
            GLMesh(GLMesh&& other)
            {
                // Calls the move-assignment operator as the logic is the same.
                *this = std::move(other);
            }

            /// @brief Copy-assignment operator
            /// @param other GLMesh object to copy.
            /// @return A reference to a GLMesh object with data internally copied from the assigned object.
            GLMesh& operator=(const GLMesh& other)
            {
                CopyInternal(other);
                return *this;
            }

            /// @brief Move-assignment operator
            /// @param other GLMesh rvalue to move data from.
            /// @return A reference to a GLMesh object with vertex & index data as well as OpenGL buffers moved from the assigned object.
            GLMesh& operator=(GLMesh&& other)
            {
                *((Mesh*)this) = (Mesh)other;

                m_IBO = other.m_IBO;
                m_VBO = other.m_VBO;
                other.m_HasIBO = false;
                other.m_HasVBO = false;

                return *this;
            }

            /// @brief Gets the Vertex Buffer Object used for this Mesh.
            /// @return An OpenGL Vertex Buffer Object name that can be used with glBindBuffer.
            inline GLuint GetVBO() const { return m_VBO; }

            /// @brief Gets the Index Buffer Object used for this Mesh.
            /// @return An OpenGL Index Buffer Object name that can be used with glBindBuffer.
            inline GLuint GetIBO() const { return m_IBO; }

            /// @brief Specialised Dispose() that, in addition to calling base Dispose(), releases OpenGL Vertex and Index Buffer Objects as needed.
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