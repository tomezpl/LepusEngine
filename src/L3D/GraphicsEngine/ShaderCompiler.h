#ifndef L3D_GRAPHICSENGINE_SHADERCOMPILER
#define L3D_GRAPHICSENGINE_SHADERCOMPILER

#include <stddef.h>

namespace LepusEngine
{
	namespace Lepus3D
	{
		enum ShaderType
		{
			FragmentShader,
			VertexShader,
			GeometryShader
		};

		template <typename TShaderHandle>
		class ShaderCompiledResult
		{
		public:
			inline ShaderCompiledResult(TShaderHandle shaderHandle)
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
			virtual ShaderCompiledResult<TShaderHandle> CompileShader(const char* shaderSource, size_t shaderSourceLength, ShaderType type) = 0;
		};
	}
}

#endif