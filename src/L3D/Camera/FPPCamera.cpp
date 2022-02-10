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
		//glfwSetInputMode(m_Wnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Give back cursor control
		//return;
	}

	// If window is in focus, request unlimited mouse movement from the window.
	glfwSetInputMode(m_Wnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// TODO: This should be moved to LepusEngine, with an input manager class.
	const float lookSensitivity = 5.f;

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
		position += m_Right.vec3() * 2.0f * deltaTime;
	}
	if (glfwGetKey(m_Wnd, GLFW_KEY_D) == GLFW_PRESS)
	{
		position -= m_Right.vec3() * 2.0f * deltaTime;
	}

	// Update the position after reading WASD input.
	m_Transform.SetPosition(Vector3(position.x, position.y, position.z));

	// Get current camera rotation.
	Quaternion rotation = m_Transform.GetRotation();
	
	double mouseX = 0.0, mouseY = 0.0;
	glfwGetCursorPos(m_Wnd, &mouseX, &mouseY); // Get XY coordinates of the cursor in the window, in pixels.

	int wndWidth = 0, wndHeight = 0;
	glfwGetWindowSize(m_Wnd, &wndWidth, &wndHeight); // Get width and height of the window in pixels.

	// Normalise the cursor XY coordinates in the window.
	mouseX /= wndWidth;
	mouseY /= wndHeight;

	// Calculate the relative rotation value
	Vector2 mouseRot((mouseX - 0.5) * -1.f, (mouseY - 0.5) * -1.f);

	// Add the relative rotation to the camera's rotation
	glm::quat newRotation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
	newRotation *= glm::angleAxis(-mouseRot.x / (10.f / lookSensitivity), Vector3(0.f, 1.f, 0.f).vec3());
	newRotation *= glm::angleAxis(mouseRot.y / (10.f / lookSensitivity), m_Right.vec3());
	
	m_Transform.SetRotation(Quaternion(newRotation.x, newRotation.y, newRotation.z, newRotation.w));

	// Normalise target vector
	glm::vec3 eye = glm::vec3(0.f, 0.f, 1.f) * newRotation;
	glm::vec3 normTarget = glm::normalize(eye);

	glm::vec3 right = glm::vec3(1.f, 0.f, 0.f) * newRotation;
	glm::vec3 normRight = glm::normalize(right);


	// Update target vector with new values
	m_Target.x = normTarget.x;
	m_Target.y = normTarget.y;
	m_Target.z = normTarget.z;

	m_Right.x = normRight.x;
	m_Right.y = normRight.y;
	m_Right.z = normRight.z;

	glm::vec3 up = glm::cross(normTarget, normRight);
	m_Up.x = up.x;
	m_Up.y = up.y;
	m_Up.z = up.z;
	// Calculate vectors
	//this->_CalcVectors();

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