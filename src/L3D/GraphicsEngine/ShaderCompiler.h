#ifndef L3D_GRAPHICSENGINE_SHADERCOMPILER
#define L3D_GRAPHICSENGINE_SHADERCOMPILER

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

		class ShaderCompiledResult
		{
		public:
			template <typename TShaderHandle>
			inline ShaderCompiledResult(TShaderHandle shaderHandle)
			{
				ShaderHandle = (void*)shaderHandle;
			}

			void* ShaderHandle = nullptr;

			template <typename TShaderHandle>
			inline TShaderHandle GetShaderHandle() const { return (TShaderHandle)ShaderHandle; }
		};
		
		class ShaderCompiler
		{
		public:
			virtual void Init() = 0;
			virtual ShaderCompiledResult CompileShader(const char* shaderSource, size_t shaderSourceLength, ShaderType type) = 0;
		};
	}
}

#endif