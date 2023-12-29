#ifndef LEPUS_GFX_GL_MESH
#define LEPUS_GFX_GL_MESH

#include <lepus/engine/Objects/Mesh.h>
#include <GL/gl3w.h>

namespace lepus
{
    namespace gfx
    {
        class GLMesh : public lepus::engine::objects::Mesh
        {
            private:
            GLuint m_VBO;
            GLuint m_IBO;

            void _CreateVBO();
            void _CreateIBO();

            protected:
            inline void InitInternal() override
            {
                _CreateVBO();
                _CreateIBO();
            }

            public:
            LEPUS_MESH_CONSTRUCTOR(GLMesh);

            inline GLuint GetVBO() const { return m_VBO; }
            inline GLuint GetIBO() const { return m_IBO; }
        };
    }
}

#endif