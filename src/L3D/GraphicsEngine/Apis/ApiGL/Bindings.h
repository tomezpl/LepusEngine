#ifndef L3D_GRAPHICSENGINE_APIS_APIGL_BINDINGS
#define L3D_GRAPHICSENGINE_APIS_APIGL_BINDINGS

#include "Types.h"

namespace lepus
{
    namespace gfx
    {
        class GLMatrixUniformBinding : public GLUniformBinding
        {
            public:
            GLMatrixUniformBinding(size_t location) : GLUniformBinding(location)
            {
                Value(new float[4 * 4]);
                memset(Value(), 0, 4 * 4 * sizeof(float));
            }

            inline void Value(float* value)
            {
                GLUniformBinding::Value<float*>(value);
            }

            inline float* Value()
            {
                return GLUniformBinding::Value<float*>();
            }

            inline UniformType Type() { return UniformType::MATRIX4; }
        };
    }
}

#endif