#include "../ApiGL.h"
#include <GL/gl3w.h>

using namespace lepus::gfx;

void GraphicsApiGL::Init(GraphicsApiOptions* options)
{
	InitInternal<GraphicsApiGLOptions>(reinterpret_cast<GraphicsApiGLOptions*>(options));
}

void GraphicsApiGL::SetupVertexArrays()
{
	// Create a global VAO and bind it.
	glCreateVertexArrays(1, &m_Pipeline.vao);
	glBindVertexArray(m_Pipeline.vao);
}

void GraphicsApiGL::SetupBuffers()
{
	glBindVertexArray(m_Pipeline.vao);

	// Create a global VBO and upload triangle data to it.
	glCreateBuffers(1, &m_Pipeline.vbo);
	//const GLfloat vertices[] = { -0.5f, -0.5f, 0.f, 0.5f, -0.5f, 0.f, 0.f, 0.5f, 0.f };
	glBindBuffer(GL_ARRAY_BUFFER, m_Pipeline.vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, 0);
	glBufferData(GL_ARRAY_BUFFER, m_CubeGeometry.VertexBufferSize(), m_CubeGeometry.GetVertices(), GL_STATIC_DRAW);

	// Create a global IBO and upload triangle index data to it.
	glCreateBuffers(1, &m_Pipeline.ibo);
	//const GLuint indices[] = { 0, 1, 2 };
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Pipeline.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_CubeGeometry.IndexBufferSize(), m_CubeGeometry.GetIndices(), GL_STATIC_DRAW);
}

void GraphicsApiGL::SetupShaders()
{
	// Zero the program array.
	memset(m_Programs, 0, GraphicsApiGLOptions::ProgramCount * sizeof(GLuint));

	for (size_t i = 0; i < GraphicsApiGLOptions::ProgramCount; i++)
	{
		auto& options = GetOptions<GraphicsApiGLOptions>();
		GLuint fragShader = options.GetFragmentShader(i), vertShader = options.GetVertexShader(i);
		if (fragShader && vertShader)
		{
			m_Programs[i] = glCreateProgram();
			glAttachShader(m_Programs[i], fragShader);
			glAttachShader(m_Programs[i], vertShader);
			glLinkProgram(m_Programs[i]);
		}
	}
}

void GraphicsApiGL::SetupUniforms()
{
	// Proj matrix
	auto* proj = new lepus::gfx::GLMatrixUniformBinding(glGetUniformLocation(m_Programs[0], "PROJ"));
	m_Pipeline.uniforms.push_front((lepus::gfx::GLUniformBinding<void*>*)(proj));
	m_Pipeline.uniformMap.insert_or_assign("PROJ", reinterpret_cast<lepus::gfx::GLUniformBinding<void*>*>(proj));

	// View matrix
	auto* view = new lepus::gfx::GLMatrixUniformBinding(glGetUniformLocation(m_Programs[0], "VIEW"));
	m_Pipeline.uniforms.push_front((lepus::gfx::GLUniformBinding<void*>*)(view));
	m_Pipeline.uniformMap.insert_or_assign("VIEW", reinterpret_cast<lepus::gfx::GLUniformBinding<void*>*>(view));
}

void GraphicsApiGL::CreatePipeline()
{
	SetupVertexArrays();
	SetupBuffers();
	SetupShaders();
	SetupUniforms();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void GraphicsApiGL::UpdateUniforms()
{
	for (auto uniform = m_Pipeline.uniforms.begin(); uniform != m_Pipeline.uniforms.end(); uniform++)
	{
		auto uniformVal = *uniform;
		if (uniformVal->IsDirty())
		{
			const GLint& location = uniformVal->Location();
			switch (uniformVal->Type())
			{
				// TODO: with more shaders, we'll want to wrap the uniform objects in "instances" that specify a program handle and hold a reference to the uniform data
				// However, it'll work for now given there is only ever one GL program in use.
				case lepus::gfx::UniformType::MATRIX4:
					glUniformMatrix4fv(location, 1, true, (reinterpret_cast<lepus::gfx::GLMatrixUniformBinding*>(uniformVal))->Value());
					break;
				case lepus::gfx::UniformType::FLOAT:
					glUniform1f(location, (reinterpret_cast<lepus::gfx::GLFloatUniformBinding*>(uniformVal))->Value());
					break;
			}
		}
	}
}

void GraphicsApiGL::Draw()
{
	glUseProgram(m_Programs[0]);

	glBindVertexArray(m_Pipeline.vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_Pipeline.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Pipeline.ibo);

	glDrawElements(GL_TRIANGLES, (GLsizei)m_CubeGeometry.IndexCount(), GL_UNSIGNED_INT, 0);
}

void GraphicsApiGL::ClearFrameBuffer(float r, float g, float b)
{
	glClearColor(r, g, b, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsApiGL::Shutdown()
{
	GraphicsApi::Shutdown();
}