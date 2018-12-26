#include "../RenderEngine.h"
#include <iostream>
#include "LepusEngine/Source/Logger.h"
#include "../Transform.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION // This is critcal as otherwise the build will fail due to unresolved externals
#include "../3rdparty/stb_image_write.h"

using namespace LepusEngine::Lepus3D;

RenderEngine::RenderEngine(char* name, unsigned short width, unsigned short height)
{
	this->Init(name, width, height);
}

// TODO
bool RenderEngine::Init(char* name, unsigned short width, unsigned short height)
{
	glfwInit(); // start GLFW
	m_ElapsedTime = glfwGetTime();
	m_LastFrameTime = 0.0;

	m_Cam = nullptr;
	m_Ready = { false, false };
	m_WindowName = name;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	m_Window = glfwCreateWindow(width, height, name, 0, 0);
	m_Ready.window = (glfwGetWindowAttrib(m_Window, GLFW_VISIBLE) == 0) ? false : true;
	glfwMakeContextCurrent(m_Window);
	return this->Init();
}

bool RenderEngine::Init()
{
	m_eCount = 0;

	if (!m_Ready.window)
	{
		Logger::LogError("RenderEngine", "Init", "can't finish, window not ready");
		return false;
	}

	GLenum err = glewInit();
	if (err != GLEW_OK)
		Logger::LogWarning("RenderEngine", "Init", "GLEW failed to load");

	// TODO: replace SFML window
	int width, height = 0;
	glfwGetFramebufferSize(m_Window, &width, &height);
	glViewport(0, 0, width, height);

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
	m_ElapsedTime = glfwGetTime(); // get time for frame time measurement
	m_Cam = cam;
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderEngine::DrawMesh(Mesh& mesh, Material& material, Transform& transform)
{
	bool useIndexing = mesh.IsIndexed();

	unsigned long long* iD = nullptr;
	if(useIndexing)
	{
		unsigned long long eCount = m_eCount;
		iD = mesh.GetIndexBuffer(eCount);
	}
	unsigned long long vC = mesh.GetVertexCount();
	unsigned long long vDS = vC * sizeof(Vertex);

	GLfloat* vArr = mesh.GetVertexBuffer();

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
		// TODO: replace SFML window
		int width, height = 0;
		glfwGetWindowSize(m_Window, &width, &height);
		projection = glm::perspective(45.f, (float)width / (float)height, 0.01f, 100.f);

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

// TODO: Remove SFML
bool RenderEngine::Update()
{
	std::string newWndName = m_WindowName;
	newWndName.append(" - FPS: ");
	newWndName.append(std::to_string((int)(1.f / m_LastFrameTime)));
	glfwSetWindowTitle(m_Window, newWndName.c_str());

	if (m_Ready.window && glfwWindowShouldClose(m_Window))
	{
		glfwDestroyWindow(m_Window);
		m_Ready.window = false;
		return false;
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
	glfwSwapBuffers(m_Window);
	float currentTime = glfwGetTime();
	m_LastFrameTime = currentTime - m_ElapsedTime;
	m_ElapsedTime = currentTime;
	glfwPollEvents();
}

double RenderEngine::LastFrameTime()
{
	return m_LastFrameTime;
}

void RenderEngine::DumpToFile(std::string fp)
{
	int width = 0;
	int height = 0;
	glfwGetFramebufferSize(m_Window, &width, &height);
	unsigned char* data = new unsigned char[width*height * 3];
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_flip_vertically_on_write(1); // GL coordinates origin is vertically inverted, so data needs to be flipped on writing
	stbi_write_bmp(fp.c_str(), width, height, 3, data);
}

void RenderEngine::Shutdown()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);
	glDeleteVertexArrays(1, &m_VAO);

	if(m_Ready.window)
		glfwDestroyWindow(m_Window);

	glfwTerminate();
}

RenderEngine::~RenderEngine()
{
	this->Shutdown();
}
