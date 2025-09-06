#ifndef LEPUS_GFX_MATERIALATTRIBUTES_ATTRIBTYPES_H
#define LEPUS_GFX_MATERIALATTRIBUTES_ATTRIBTYPES_H
#include "lepus/gfx/GraphicsEngine/GraphicsApi/Uniforms.h"

namespace lepus::gfx
{
    class AttributeTypes
    {
	public:
	AttributeTypes() = delete;

	static inline constexpr size_t GetDataSize(UniformType type)
	{
	    switch (type)
	    {
	    case FLOAT:
		return sizeof(float);
	    case VEC3:
		return sizeof(float) * 3;
	    case MATRIX4:
		return sizeof(float) * 4 * 4;
	    case INVALID:
	    default:
		return 0;
	    }
	}
    };
} // namespace lepus::gfx

#endif
