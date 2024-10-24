#ifndef L3D_GRAPHICSENGINE_GRAPHICSAPI_UNIFORMS
#define L3D_GRAPHICSENGINE_GRAPHICSAPI_UNIFORMS

namespace lepus
{
    namespace gfx
    {
	enum UniformType
	{
	    INVALID = 0,
	    MATRIX4,
	    FLOAT,
	};

#pragma region Engine global uniform names
#define LEPUS_GFX_UNIFORMS_GLOBAL_PROJECTION_MATRIX "PROJ"
#define LEPUS_GFX_UNIFORMS_GLOBAL_VIEW_MATRIX "VIEW"
#define LEPUS_GFX_UNIFORMS_GLOBAL_MODEL_MATRIX "MODEL"
#pragma endregion
    } // namespace gfx
} // namespace lepus

#endif