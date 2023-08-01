#include "../ApiGL.h"
#include <GL/gl3w.h>

using namespace LepusEngine::Lepus3D;

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

void GraphicsApiGL::CreatePipeline()
{
	SetupVertexArrays();
	SetupBuffers();
	SetupShaders();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_CLAMP);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
}

void GraphicsApiGL::Draw()
{
	glUseProgram(m_Programs[0]);

	glBindVertexArray(m_Pipeline.vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_Pipeline.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Pipeline.ibo);

	glDrawElements(GL_TRIANGLES, m_CubeGeometry.IndexCount(), GL_UNSIGNED_INT, 0);
}

void GraphicsApiGL::ClearFrameBuffer(float r, float g, float b)
{
	glClearColor(r, g, b, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GraphicsApiGL::Shutdown()
{
	GraphicsApi::Shutdown();
}