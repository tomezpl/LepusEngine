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
	m_Cam = nullptr;
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

	m_LastFPSUpdateTime = m_LastFrameTime = 0.f;

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
	glEnable(GL_CULL_FACE);

	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);
	glGenVertexArrays(1, &m_VAO);
	glGenTextures(sizeof(m_TextureSet) / sizeof(GLuint), m_TextureSet);

	m_Ready.renderer = true;

	return true;
}

void RenderEngine::StartScene(Camera* cam)
{
	m_Cam = cam;
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderEngine::DrawMesh(Mesh& mesh, Material& material, Transform& transform)
{
	bool useIndexing = mesh.IsIndexed();

	unsigned long long* iD = nullptr;
	if(useIndexing)
		iD = mesh.GetIndexBuffer((unsigned long long)m_eCount);
	unsigned long long vC = mesh.GetVertexCount();
	unsigned long long vDS = vC * sizeof(Vertex);

	GLfloat* vArr = mesh.GetVertexBuffer();
	// TODO: This copy loop might be a potential culprit of framerate issues. It has to copy all floats on each draw call.
	/*for (unsigned long long i = 0, j = 0; i < vDS / sizeof(GLfloat); i += sizeof(vD[i]) / sizeof(GLfloat), j++)
	{
		vArr[i] = vD[j].x; // vertex model space position (X coord)
		vArr[i+1] = vD[j].y; // vertex model space position (Y coord)
		vArr[i+2] = vD[j].z; // vertex model space position (Z coord)
		vArr[i+3] = vD[j].s; // texture coord S
		vArr[i+4] = vD[j].t; // texture coord T
		vArr[i+5] = vD[j].nX; // normal vector X
		vArr[i+6] = vD[j].nY; // normal vector Y
		vArr[i+7] = vD[j].nZ; // normal vector Z
	}*/

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	auto textureCount = material.m_TexAttributes.size();

	if (textureCount > sizeof(m_TextureSet) / sizeof(GLuint))
	{
		Logger::LogWarning("RenderEngine", "DrawMesh", "texture count is bigger than the texture set size!", "mesh, material");
	}

	for (auto i = 0; i < textureCount; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureSet[i]);
		{
			auto currentTex = material.m_TexAttributes[i].value;
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

		if (useIndexing)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_eCount * sizeof(unsigned long long), iD, GL_STATIC_DRAW); // pass elements/indices to GPU
		}

		// Set vertex positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Set texture coords
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// Set normal vectors
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(5 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
	}
	glBindVertexArray(0); // unbind VAO

	// Set View position for lighting

	material.SetAttributeF3("_ViewPos", m_Cam->GetTransform().GetPosition());

	material.Use();

	for (auto i = 0; i < textureCount; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_TextureSet[i]);
		glUniform1i(glGetUniformLocation(material.m_Shader.m_Compiled, material.m_TexAttributes[i].name), i);
	}

	glBindVertexArray(m_VAO);
	{
		glm::mat4 model, view, projection;
		model = transform.GetMatrix();
		//view = glm::translate(view, glm::vec3(0.f, 0.f, -2.f));
		view = m_Cam->GetView();
		projection = glm::perspective(45.f, (float)m_Window.getSize().x / (float)m_Window.getSize().y, 0.01f, 100.f);

		glUniformMatrix4fv(glGetUniformLocation(material.m_Shader.m_Compiled, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(material.m_Shader.m_Compiled, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(material.m_Shader.m_Compiled, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		useIndexing = false; // TODO: temporary fix for drawing artifacts
		if (useIndexing)
			glDrawElements(GL_TRIANGLES, m_eCount, GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(GL_TRIANGLES, 0, vC);
	}
	glBindVertexArray(0);

	// Release resources
	delete iD;
	//delete vArr;
}

bool RenderEngine::Update()
{
	std::string newWndName = m_WindowName;
	newWndName.append(" - FPS: ");
	float currentTime = m_Clock.getElapsedTime().asSeconds();
	newWndName.append(std::to_string((int)(1.f / (currentTime - m_LastFrameTime))));
	if (currentTime - m_LastFPSUpdateTime >= 1.f)
	{
		m_Window.setTitle(newWndName);
		m_LastFPSUpdateTime = currentTime;
	}
	m_LastFrameTime = currentTime;
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

void RenderEngine::DrawScene(Scene& sc)
{
	int length = sc.GetRenderableCount();
	for (int i = 0; i < length; i++)
	{
		Mesh* mesh = sc.m_ObjArr[i]->GetMesh();
		if(sc.GetLightCount() > 0 && sc.m_LightArr[0] != nullptr)
		{
			mesh->m_Mat->SetAttributeF3("_LightPos", sc.m_LightArr[0]->GetPosition());
			mesh->m_Mat->SetAttributeF3("_LightColor", sc.m_LightArr[0]->GetColor().GetVector3());
		}
		mesh->m_Mat->SetAttributeF3("_AmbientColor", sc.m_AmbientColor.GetVector3());
		mesh->m_Mat->SetAttributeF("_AmbientStrength", sc.m_AmbientIntensity);
		DrawMesh(*mesh, *(mesh->m_Mat), sc.m_ObjArr[i]->mTransform);
	}
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
