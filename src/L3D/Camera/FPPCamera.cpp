#include "FPPCamera.h"
#include <LEngine/Logger.h>
#include <functional>

using namespace LepusEngine::Lepus3D;

void FPPCamera::SetWindow(GLFWwindow* wnd)
{
	m_Wnd = wnd; // set the window handle

	// Centers (or recenters) the XY coordinates of the cursor in the frame.
	m_LastX = 0.5f; 
	m_LastY = 0.5f;

	_fppCamPtr = this; // Sets this camera as the current instance.
	glfwSetWindowSizeCallback(wnd, ResizeCallback); // Sets the resize callback for the window handle to FPPCamera's callback function.
}

glm::mat4 FPPCamera::Run()
{
	// Return View matrix
	return this->GetView();
}

void FPPCamera::ProcessInput(float deltaTime)
{
	// Check whether window is out of focus
	if (!glfwGetWindowAttrib(m_Wnd, GLFW_FOCUSED))
	{
		glfwSetInputMode(m_Wnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Give back cursor control
		return;
	}

	// If window is in focus, request unlimited mouse movement from the window.
	glfwSetInputMode(m_Wnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// TODO: This should be moved to LepusEngine, with an input manager class.
	const float lookSensitivity = 100.f;

	// Get current camera position.
	glm::vec3 position = m_Transform.GetPosition().vec3();

	// Check for key presses (WASD).
	// TODO: Build an input handler in LepusEngine separate from Camera classes. Replace this with calls to the handler.
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

	// Update the position after reading WASD input.
	m_Transform.SetPosition(Vector3(position.x, position.y, position.z));

	// Get current camera rotation.
	Vector3 rotation = m_Transform.GetRotation();

	double mouseX = 0.0, mouseY = 0.0;
	glfwGetCursorPos(m_Wnd, &mouseX, &mouseY); // Get XY coordinates of the cursor in the window, in pixels.

	int wndWidth = 0, wndHeight = 0;
	glfwGetWindowSize(m_Wnd, &wndWidth, &wndHeight); // Get width and height of the window in pixels.

	// Normalise the cursor XY coordinates in the window.
	mouseX /= wndWidth;
	mouseY /= wndHeight;

	// Calculate the relative rotation value
	Vector2 mouseRot(mouseX - 0.5, mouseY - 0.5);

	// Add the relative rotation to the camera's rotation
	rotation.y += mouseRot.x * lookSensitivity;
	if (rotation.y <= 0.0f)
		rotation.y += 360.0f; // This is a quick fix for issue #2, where negative rotation was not supported
	rotation.x -= mouseRot.y * lookSensitivity;
	Logger::LogInfo("FPPCamera", "ProcessInput", (char*)std::to_string(mouseY).c_str());

	// quick fix for issue #2 (no negative rotation)
	if (rotation.x <= 0.0f)
		rotation.x = 360.0f;

	// Lock rotation so that camera cannot turn upside down
	if (rotation.x > 449.9f)
		rotation.x = 449.9f;
	if (rotation.x < 360.0f - 89.9f)
		rotation.x = 360.0f - 89.9f;

	// Apply new rotation
	m_Transform.SetRotation(rotation);

	// Update last cursor position
	m_LastX = mouseX;
	m_LastY = mouseY;

	// Recenter cursor for next frame
	if (glfwGetWindowAttrib(m_Wnd, GLFW_FOCUSED))
	{
		glfwSetCursorPos(m_Wnd, wndWidth / 2, wndHeight / 2);
		glfwSetInputMode(m_Wnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}