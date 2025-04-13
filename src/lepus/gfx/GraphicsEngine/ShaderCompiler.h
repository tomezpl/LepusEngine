#ifndef L3D_GRAPHICSENGINE_SHADERCOMPILER
#define L3D_GRAPHICSENGINE_SHADERCOMPILER

#include "lepus/gfx/Shader.h"

#include <stddef.h>

namespace lepus
{
    namespace gfx
    {
	template <typename TShaderHandle>
	class ShaderCompiledResult
	{
	    public:
	    inline explicit ShaderCompiledResult(TShaderHandle shaderHandle)
	    {
		ShaderHandle = shaderHandle;
	    }

	    TShaderHandle ShaderHandle;

	    /*template <typename TShaderHandle>
	    inline TShaderHandle GetShaderHandle() const { return reinterpret_cast<TShaderHandle>(ShaderHandle); }*/
	};

	template <typename TShaderHandle>
	class ShaderCompiler
	{
	    public:
	    virtual void Init() = 0;
	    virtual ShaderCompiledResult<TShaderHandle> CompileShader(const void* shaderSource, size_t shaderSourceLength, ShaderStage type) = 0;

	    virtual ~ShaderCompiler()
	    {
	    }
	};
    } // namespace gfx
} // namespace lepus

#endif