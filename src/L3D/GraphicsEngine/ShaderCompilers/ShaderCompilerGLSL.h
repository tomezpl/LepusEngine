#ifndef L3D_GRAPHICSENGINE_SHADERCOMPILER_GLSL
#define L3D_GRAPHICSENGINE_SHADERCOMPILER_GLSL

#include "../ShaderCompiler.h"
#include <GL/gl3w.h>
#include <cassert>
#include <climits>


#ifndef MAXINT
#define MAXINT ULLONG_MAX
#endif

namespace LepusEngine
{
	namespace Lepus3D
	{
		typedef ShaderCompiledResult<GLuint> GLShaderCompiledResult;

		class ShaderCompilerGLSL : public ShaderCompiler<GLuint>
		{
		private:
			static ShaderCompilerGLSL* m_Instance;
		public:
			// Inherited via ShaderCompiler
			virtual void Init() override {}

			inline GLShaderCompiledResult CompileShader(const char* shaderSource, size_t shaderSrcLength, ShaderType type) override
			{
				assert(shaderSrcLength <= MAXINT);

				GLenum shaderType = -1;

				switch (type)
				{
				case VertexShader:
					shaderType = GL_VERTEX_SHADER;
					break;
				case FragmentShader:
					shaderType = GL_FRAGMENT_SHADER;
					break;
				case GeometryShader:
					shaderType = GL_GEOMETRY_SHADER;
					break;
				}

				GLuint shaderHandle = glCreateShader(shaderType);
				glShaderSource(shaderHandle, 1, ((const char**)&shaderSource), (const GLint*)(&shaderSrcLength));
				glCompileShader(shaderHandle);
				ShaderCompiledResult compiled(shaderHandle);

				return compiled;
			}

			static ShaderCompilerGLSL& Singleton()
			{
				if(!m_Instance) {
					m_Instance = new ShaderCompilerGLSL();
				}

				return *m_Instance;
			}
		};
	}
}

#endif