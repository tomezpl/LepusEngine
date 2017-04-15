#include "../RenderEngine.h"
#include <iostream>
#include "LepusEngine/Source/Logger.h"
#include "../Transform.h"

using namespace LepusEngine::Lepus3D;

RenderEngine::RenderEngine(char* name, unsigned short width, unsigned short height)
{
	this->Init(name, width, height);
}

bool RenderEngine::Init(char* name, unsigned short width, unsigned short height)
{
	m_Ready = { false, false };
	m_WindowName = name;
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	m_Window.create(sf::VideoMode(width, height), name, sf::Style::Default, settings);
	m_Ready.window = m_Window.isOpen();
	return this->Init();
}

bool RenderEngine::Init()
{
	m_eCount = 0;
	m_CurrentMesh = nullptr;
	m_CurrentMat = nullptr;
	m_CurrentTrans = nullptr;

	m_LastFrameTime = 0.f;

	m_Clock.restart();

	if (!m_Ready.window)
	{
		Logger::LogError("RenderEngine", "Init", "can't finish, window not ready");
		return false;
	}

	GLenum err = glewInit();
	if (err != GLEW_OK)
		Logger::LogWarning("RenderEngine", "Init", "GLEW failed to load");

	glViewport(0, 0, m_Window.getSize().x, m_Window.getSize().y);

	glEnable(GL_DEPTH_TEST);

	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);
	glGenVertexArrays(1, &m_VAO);
	glGenTextures(sizeof(m_TextureSet) / sizeof(GLuint), m_TextureSet);

	m_Ready.renderer = true;
	
	return true;
}

void RenderEngine::StartScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderEngine::DrawMesh(Mesh& mesh, Material& material, Transform& transform)
{
	m_CurrentMesh = &mesh;
	m_CurrentMat = &material;
	m_CurrentTrans = &transform;
	unsigned int* iD = m_CurrentMesh->GetIndexBuffer(m_eCount);
	VertexPack vP = m_CurrentMesh->GetVertexBuffer();
	Vertex* vD = vP.data;
	unsigned int vDS = vP.size() * sizeof(Vertex);

	GLfloat* vArr = new GLfloat[vDS / sizeof(GLfloat)];
	for (unsigned short i = 0, j = 0; i < vDS / sizeof(GLfloat); i += sizeof(vD[i]) / sizeof(GLfloat), j++)
	{
		vArr[i] = vD[j].x;
		vArr[i+1] = vD[j].y;
		vArr[i+2] = vD[j].z;
		vArr[i+3] = vD[j].r;
		vArr[i+4] = vD[j].g;
		vArr[i+5] = vD[j].b;
		vArr[i+6] = vD[j].s;
		vArr[i+7] = vD[j].t;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	auto textureCount = m_CurrentMat->m_TexAttributes.size();

	if (textureCount > sizeof(m_TextureSet) / sizeof(GLuint))
	{
		Logger::LogWarning("RenderEngine", "DrawMesh", "texture count is bigger than the texture set size!", "mesh, material");
	}

	for (auto i = 0; i < textureCount; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureSet[i]);
		{
			auto currentTex = m_CurrentMat->m_TexAttributes[i].value;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, currentTex.GetWidth(), currentTex.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, currentTex.GetData());
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// bind VAO to VBO
	glBindVertexArray(m_VAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vDS, vArr, GL_STATIC_DRAW); // pass vertices to GPU

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_eCount * sizeof(GLuint), iD, GL_STATIC_DRAW); // pass elements/indices to GPU

		// Set vertex positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Set vertex colours
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// Set texture coords
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
	}
	glBindVertexArray(0); // unbind VAO

	m_CurrentMat->Use();

	for (auto i = 0; i < textureCount; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_TextureSet[i]);
		glUniform1i(glGetUniformLocation(m_CurrentMat->m_Shader.m_Compiled, m_CurrentMat->m_TexAttributes[i].name), i);
	}

	glBindVertexArray(m_VAO);
	{
		glm::mat4 model, view, projection;
		model = m_CurrentTrans->GetMatrix();
		view = glm::translate(view, glm::vec3(0.f, 0.f, -2.f));
		projection = glm::perspective(45.f, (float)m_Window.getSize().x / (float)m_Window.getSize().y, 0.01f, 100.f);

		glUniformMatrix4fv(glGetUniformLocation(m_CurrentMat->m_Shader.m_Compiled, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(m_CurrentMat->m_Shader.m_Compiled, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(m_CurrentMat->m_Shader.m_Compiled, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glEnable(GL_DEPTH_TEST);

		glDrawElements(GL_TRIANGLES, m_eCount, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}

bool RenderEngine::Update()
{
	std::string newWndName = m_WindowName;
	newWndName.append(" - FPS: ");
	float currentTime = m_Clock.getElapsedTime().asSeconds();
	newWndName.append(std::to_string((int)(1.f / (currentTime - m_LastFrameTime))));
	m_LastFrameTime = currentTime;
	if ((int)(glm::round(currentTime)) % 3 == 0)
	{
		m_Window.setTitle(newWndName);
	}
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

	return true;
}

void RenderEngine::EndScene()
{
	m_Window.display();
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