#ifndef L3D_GRAPHICSENGINE_SHADERCOMPILER_GLSL
#define L3D_GRAPHICSENGINE_SHADERCOMPILER_GLSL

#include "../ShaderCompiler.h"
#include <GL/gl3w.h>
#include <cassert>
#include <climits>
#include <iostream>

#ifndef MAXINT
#define MAXINT ULLONG_MAX
#endif

namespace lepus
{
    namespace gfx
    {
	typedef ShaderCompiledResult<GLuint> GLShaderCompiledResult;

	class ShaderCompilerGLSL : public ShaderCompiler<GLuint>
	{
	    private:
	    static ShaderCompilerGLSL* m_Instance;

	    public:
	    // Inherited via ShaderCompiler
	    virtual void Init() override
	    {
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback([](GLenum source,
		                          GLenum type,
		                          GLuint id,
		                          GLenum severity,
		                          GLsizei length,
		                          const GLchar* message,
		                          const void* userParam)
		                       { std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") << " type = " << type << ", severity = " << severity << ", message = " << message << std::endl; },
		                       0);
	    }

	    inline GLShaderCompiledResult CompileShader(const char* shaderSource, size_t shaderSrcLength, ShaderType type) override
	    {
		assert(shaderSrcLength <= MAXINT);

		GLenum shaderType = 0;

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

	    [[nodiscard]] inline GLuint BuildProgram(const GLShaderCompiledResult& fragResult, const GLShaderCompiledResult& vertResult)
	    {
		GLuint program = glCreateProgram();
		glAttachShader(program, fragResult.ShaderHandle);
		glAttachShader(program, vertResult.ShaderHandle);
		glLinkProgram(program);

		return program;
	    }

	    static ShaderCompilerGLSL& Singleton()
	    {
		if (!m_Instance)
		{
		    m_Instance = new ShaderCompilerGLSL();
		    m_Instance->Init();
		}

		return *m_Instance;
	    }
	};
    } // namespace gfx
} // namespace lepus

#endif