#include "../GLFW.h"

using namespace LepusEngine::LepusSystem;

bool WindowingGLFW::Init(unsigned short windowWidth, unsigned short windowHeight)
{
	// Make sure GLFW initialised.
	assert(glfwInit());

	// Set minimum required OpenGL version.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	m_Window = glfwCreateWindow(windowWidth, windowHeight, "LepusGLFW", nullptr, nullptr);
	
	// Make sure the window has been created successfuly.
	if (!m_Window)
	{
		return false;
	}

	return true;
}

bool WindowingGLFW::Update()
{
	glfwPollEvents();

	return !glfwWindowShouldClose(m_Window);
}

void WindowingGLFW::Shutdown()
{
	if (m_Window && !m_WindowDestroyed)
	{
		m_WindowDestroyed = true;
		glfwDestroyWindow(m_Window);
	}
}