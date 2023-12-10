#ifndef L3D_GRAPHICSENGINE_GRAPHICSAPI_UNIFORMS
#define L3D_GRAPHICSENGINE_GRAPHICSAPI_UNIFORMS

namespace lepus
{
    namespace gfx
    {
        enum UniformType
        {
            MATRIX4,
            FLOAT
        };

#pragma region Engine global uniform names
#define LEPUS_GFX_UNIFORMS_GLOBAL_PROJECTION_MATRIX "PROJ"
#define LEPUS_GFX_UNIFORMS_GLOBAL_VIEW_MATRIX "VIEW"
#pragma endregion
    }
}

#endif