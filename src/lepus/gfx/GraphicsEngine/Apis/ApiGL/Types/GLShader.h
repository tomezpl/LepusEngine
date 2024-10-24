#ifndef LEPUS_GFX_GL_SHADER
#define LEPUS_GFX_GL_SHADER

#include <lepus/gfx/GraphicsEngine/Apis/ApiGL/Types.h>
#include <lepus/gfx/Shader.h>

namespace lepus
{
    namespace gfx
    {
	class GLShader : public Shader<GLuint>
	{
	    private:
	    GLuint m_Program;

	    public:
	    GLuint GetApiHandle() override
	    {
		return this->m_Program;
	    }

	    void SetGLProgram(GLuint program)
	    {
		m_Program = program;
	    }

	    explicit GLShader(const ShaderInfo& base)
	        : Shader<GLuint>(base)
	    {
		m_Program = 0;
	    }
	};
    } // namespace gfx
} // namespace lepus

#endif
