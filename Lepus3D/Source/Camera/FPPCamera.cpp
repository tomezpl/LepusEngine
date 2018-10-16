#include "FPPCamera.h"
#include "LepusEngine/Source/Logger.h"
#include <functional>

using namespace LepusEngine::Lepus3D;

FPPCamera::FPPCamera(GLFWwindow* wnd)
{
	this->SetWindow(wnd);
}

void FPPCamera::SetWindow(GLFWwindow* wnd)
{
	m_Wnd = wnd;
	m_LastX = 0.5f;
	m_LastY = 0.5f;
	_fppCamPtr = this;
	glfwSetWindowSizeCallback(wnd, ResizeCallback);
}

glm::mat4 FPPCamera::Run()
{
	return this->GetView();
}

void FPPCamera::ProcessInput(float deltaTime)
{
	if (!glfwGetWindowAttrib(m_Wnd, GLFW_FOCUSED))
	{
		glfwSetInputMode(m_Wnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // give back cursor control
		return;
	}

	glfwSetInputMode(m_Wnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	const float lookSensitivity = 100.f;
	auto position = m_Transform.GetPosition().vec3();
	if (glfwGetKey(m_Wnd, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += m_Target.vec3() * deltaTime * 2.f;
	}
	if (glfwGetKey(m_Wnd, GLFW_KEY_S) == GLFW_PRESS)
	{
		position -= m_Target.vec3() * deltaTime * 2.f;
	}
	if (glfwGetKey(m_Wnd, GLFW_KEY_A) == GLFW_PRESS)
	{
		position -= glm::normalize(glm::cross(m_Target.vec3(), m_Up.vec3())) * 2.0f * deltaTime;
	}
	if (glfwGetKey(m_Wnd, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += glm::normalize(glm::cross(m_Target.vec3(), m_Up.vec3())) * 2.0f * deltaTime;
	}
	m_Transform.SetPosition(Vector3(position.x, position.y, position.z));
	Vector3 rotation = m_Transform.GetRotation();



	double mouseX, mouseY = 0.0;
	glfwGetCursorPos(m_Wnd, &mouseX, &mouseY);
	int wndWidth, wndHeight = 0;
	glfwGetWindowSize(m_Wnd, &wndWidth, &wndHeight);
	bool mouseGood = false;

		mouseGood = true;
		mouseX /= wndWidth;
		mouseY /= wndHeight;
		Vector2 mouseRot(mouseX - 0.5, mouseY - 0.5);
		rotation.y += mouseRot.x * lookSensitivity;
		if (rotation.y <= 0.0f)
			rotation.y += 360.0f; // quick fix for issue #2 (negative rotation not supported)
		rotation.x -= mouseRot.y * lookSensitivity;
		Logger::LogInfo("FPPCamera", "ProcessInput", (char*)std::to_string(mouseY).c_str());

	// quick fix for issue #2 (no negative rotation)
	if (rotation.x <= 0.0f)
		rotation.x = 360.0f;
	if (rotation.x > 449.9f)
		rotation.x = 449.9f;
	if (rotation.x < 360.0f - 89.9f)
		rotation.x = 360.0f - 89.9f; // I was lazy to do the maths lol
	m_Transform.SetRotation(rotation);

	//Logger::LogInfo("FPPCamera", "ProcessInput", (char*)(m_Transform.ToString().c_str()));
	if (mouseGood)
	{
		m_LastX = mouseX;
		m_LastY = mouseY;
		if (glfwGetWindowAttrib(m_Wnd, GLFW_FOCUSED))
		{
			glfwSetCursorPos(m_Wnd, wndWidth / 2, wndHeight / 2);
			glfwSetInputMode(m_Wnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
}