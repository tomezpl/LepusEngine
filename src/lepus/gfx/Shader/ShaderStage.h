#ifndef LEPUS_GFX_SHADER_SHADERSTAGE
#define LEPUS_GFX_SHADER_SHADERSTAGE

namespace lepus
{
    namespace gfx
    {
	enum ShaderStage
	{
	    ShaderStageInvalid = 0,
	    ShaderStageVertex = 1 << 0,
	    ShaderStageFragment = 1 << 1
	};
    }
} // namespace lepus

#endif