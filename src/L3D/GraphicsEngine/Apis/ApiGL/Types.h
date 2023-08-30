#ifndef L3D_GRAPHICSENGINE_APIS_APIGL_TYPES
#define L3D_GRAPHICSENGINE_APIS_APIGL_TYPES

#include "../../GraphicsApi/BaseBindings.h"

typedef unsigned int GLuint;
typedef int GLint;

namespace lepus
{
    namespace gfx
    {
        template<typename TUniformValue> class GLUniformBinding : public lepus::gfx::UniformBinding<GLint, TUniformValue>
        {
            public:
            GLUniformBinding(size_t location) : UniformBinding<GLint, TUniformValue>(location)
            {
                // No idea why, but sometimes the location won't assign correctly, so we do this terribleness...
                m_Location = location;
            }

            const GLint& Location() const override
            {
                return m_Location;
            }
        };
    }
}

#endif