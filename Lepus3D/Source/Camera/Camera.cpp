#include "../Camera.h"

using namespace LepusEngine;
using namespace Lepus3D;

// Default constructor. Initialises the instance and calculates initial vectors.
Camera::Camera()
{
	this->_Init();
}

void Camera::_Init()
{
	Vector3 defaultTarget = Vector3::Zero();
	m_Target = defaultTarget;

	// Calculate right & up vectors for this target
	this->_CalcVectors();
}

void Camera::_CalcVectors()
{
	// Set upward vector
	m_Up.Set(0.f, 1.f, 0.f);
	
	// Get current camera position from transform
	glm::vec3 camPos = m_Transform.GetPosition().vec3();
	
	// Calculate new right-vector using upward vector, position and target
	Vector3 newRight = Vector3(glm::normalize(glm::cross(m_Up.vec3(), glm::normalize(camPos - m_Target.vec3()))));
	m_Right = newRight;
}

Camera::Camera(Transform transform) : Camera()
{
	// Assign transform
	this->SetTransform(transform);

	// Initialise instance
	this->_Init();
}

void Camera::SetTransform(Transform transform)
{
	// Copy transform values
	m_Transform = transform;
}

glm::mat4 Camera::GetView()
{
	// Calculate vectors
	this->_CalcVectors();

	// Get current camera position and rotation vectors from transform
	glm::vec3 camPos = m_Transform.GetPosition().vec3();
	glm::vec3 rotation = m_Transform.GetRotation().vec3();

	// Move target vector based on rotation
	Vector3 target(
		cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y)),
		sin(glm::radians(rotation.x)),
		cos(glm::radians(rotation.x)) * sin(glm::radians(rotation.y)));

	// Normalise target vector
	glm::vec3 normTarget = glm::normalize(target.vec3());

	// Update target vector with new values
	m_Target.x = normTarget.x;
	m_Target.y = normTarget.y;
	m_Target.z = normTarget.z;

	glm::vec3 up = m_Up.vec3();

	// Return view/lookAt matrix
	glm::mat4 ret = glm::lookAt(camPos, camPos + normTarget, up);
	return ret;
}

Vector3 Camera::GetViewVector()
{
	return this->m_Target;
}

Transform Camera::GetTransform()
{
	return this->m_Transform;
}

void InputCamera::SetWindow(GLFWwindow* wnd)
{
	m_Wnd = wnd;
}