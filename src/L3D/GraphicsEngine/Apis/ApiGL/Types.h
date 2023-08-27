#ifndef L3D_GRAPHICSENGINE_APIS_APIGL_TYPES
#define L3D_GRAPHICSENGINE_APIS_APIGL_TYPES

#include "../../GraphicsApi/BaseBindings.h"

typedef unsigned int GLuint;
typedef int GLint;

namespace lepus
{
    namespace gfx
    {
        typedef lepus::gfx::UniformBinding<GLint> GLUniformBinding;
    }
}

#endif