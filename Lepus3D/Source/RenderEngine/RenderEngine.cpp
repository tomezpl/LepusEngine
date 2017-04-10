#include "../RenderEngine.h"
#include <iostream>
#include "LepusEngine\Source\Logger.h"

using namespace LepusEngine::Lepus3D;

RenderEngine::RenderEngine(char* name, unsigned short width, unsigned short height)
{
	this->Init(name, width, height);
}

bool RenderEngine::Init(char* name, unsigned short width, unsigned short height)
{
	m_Ready = { false, false };
	m_Window.create(sf::VideoMode(width, height), name, sf::Style::Default);
	m_Ready.window = m_Window.isOpen();
	return this->Init();
}

bool RenderEngine::Init()
{
	m_eCount = 0;
	m_CurrentMat = nullptr;

	if (!m_Ready.window)
	{
		Logger::LogError("RenderEngine", "Init", "can't finish, window not ready");
		return false;
	}

	GLenum err = glewInit();
	if (err != GLEW_OK)
		Logger::LogWarning("RenderEngine", "Init", "GLEW failed to load");

	glViewport(0, 0, m_Window.getSize().x, m_Window.getSize().y);

	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);
	glGenVertexArrays(1, &m_VAO);

	m_Ready.renderer = true;
	
	return true;
}

void RenderEngine::DrawVertices(Vertex* vD, GLuint vDS, GLuint* iD, GLuint iC, Material& mat)
{
	m_CurrentMat = &mat;
	m_eCount = iC;

	GLfloat* vArr = new GLfloat[vDS / sizeof(GLfloat)];
	for (unsigned short i = 0, j = 0; i < vDS / sizeof(GLfloat); i += sizeof(vD[i]) / sizeof(GLfloat), j++)
	{
		vArr[i] = vD[j].x;
		vArr[i+1] = vD[j].y;
		vArr[i+2] = vD[j].z;
		vArr[i+3] = vD[j].r;
		vArr[i+4] = vD[j].g;
		vArr[i+5] = vD[j].b;
	}

	// bind VAO to VBO
	glBindVertexArray(m_VAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vDS, vArr, GL_STATIC_DRAW); // pass vertices to GPU

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, iC * sizeof(GLuint), iD, GL_STATIC_DRAW); // pass elements/indices to GPU

		// Set vertex positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Set vertex colours
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
	}
	glBindVertexArray(0); // unbind VAO
}

bool RenderEngine::Update()
{
	sf::Event ev;
	while (m_Window.pollEvent(ev))
	{
		switch (ev.type)
		{
		case sf::Event::Closed:
			m_Window.close();
			return false;
		case sf::Event::Resized:
			glViewport(0, 0, ev.size.width, ev.size.height);
			break;
		}
	}

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	m_CurrentMat->Use();

	glBindVertexArray(m_VAO);
	{
		glDrawElements(GL_TRIANGLES, m_eCount, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);

	m_Window.display();

	return true;
}

void RenderEngine::Shutdown()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);
	glDeleteVertexArrays(1, &m_VAO);

	m_Window.close();
}

RenderEngine::~RenderEngine()
{
	this->Shutdown();
}