#ifndef L3D_GRAPHICSENGINE_APIS_APIGL_BINDINGS
#define L3D_GRAPHICSENGINE_APIS_APIGL_BINDINGS

#include "Types.h"
#include <memory>

namespace lepus
{
    namespace gfx
    {
        class GLMatrixUniformBinding : public GLUniformBinding<float*>
        {
            public:
            GLMatrixUniformBinding(GLint location) : GLUniformBinding(location)
            {
                Value(new float[4 * 4]);
                memset(Value(), 0, 4 * 4 * sizeof(float));
            }

            inline UniformType Type() { return UniformType::MATRIX4; }
        };

        class GLFloatUniformBinding : public GLUniformBinding<float>
        {
            public:
            GLFloatUniformBinding(GLint location) : GLUniformBinding(location)
            {
                Value(0.f);
            }

            inline UniformType Type() { return UniformType::FLOAT; }
        };
    }
}

#endif