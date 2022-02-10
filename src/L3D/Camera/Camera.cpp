#include "../Camera.h"
#include <LEngine/Logger.h>

using namespace LepusEngine;
using namespace Lepus3D;

// Default constructor. Initialises the instance and calculates initial vectors.
Camera::Camera()
{
	this->_Init();
}

void Camera::_Init()
{
	// Sets initial camera target at (0, 0, 0)
	Vector3 defaultTarget = Vector3(0.f, 0.f, 1.f);
	m_Target = defaultTarget;

	m_Up.Set(0.f, 1.f, 0.f);
	m_Right.Set(1.f, 0.f, 0.f);

	// Calculate right & up vectors for this target
	this->_CalcVectors();
}

void Camera::_CalcVectors()
{
	
	// Get current camera position from transform
	glm::vec3 camPos = m_Transform.GetPosition().vec3();
	
	// Calculate new right-vector using upward vector, position and target
	//Vector3 newRight = Vector3(glm::normalize(glm::cross(m_Up.vec3(), glm::normalize(camPos - m_Target.vec3()))));
	//m_Right = newRight;
	Quaternion rot = m_Transform.GetRotation();

	/*m_Target = Vector3(
		2.f * (rot.x * rot.z + rot.w * rot.y),
		2.f * (rot.y * rot.z - rot.w * rot.x),
		1.f - 2.f * (rot.x * rot.x + rot.y * rot.y)
	);

	m_Up = Vector3(
		2.f*(rot.x*rot.y-rot.w*rot.z),
		1.f-2.f*(rot.x*rot.x+rot.z*rot.z),
		2.f*(rot.y*rot.z+rot.w*rot.x)
	);

	m_Right = Vector3(
		-(1.f-2.f*rot.y*rot.y+rot.z*rot.z),
		-(2.f*(rot.x*rot.y+rot.w*rot.z)),
		-(2.f*(rot.x*rot.z-rot.w*rot.y))
	);*/

	Logger::LogInfo("Camera", "_CalcVectors", (char*)("Forward: " + m_Target.ToString()).c_str());
	Logger::LogInfo("Camera", "_CalcVectors", (char*)("Up: " + m_Up.ToString()).c_str());
	Logger::LogInfo("Camera", "_CalcVectors", (char*)("Right: " + m_Right.ToString()).c_str());
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
	_CalcVectors();

	// Get current camera position and rotation vectors from transform
	glm::vec3 camPos = m_Transform.GetPosition().vec3();
	Quaternion quaternion = m_Transform.GetRotation();
	glm::quat rot = glm::quat(quaternion.w, quaternion.x, quaternion.y, quaternion.z);
	glm::vec3 rotation = glm::eulerAngles(rot);

	//rotation.x *= 180.f / PI;
	//rotation.y *= 180.f / PI;
	//rotation.z *= 180.f / PI;
	//rotation.x = glm::radians(rotation.x);
	//rotation.y = glm::radians(rotation.y);
	//rotation.z = glm::radians(rotation.z);

	glm::vec3 up = m_Up.vec3();

	// Return view/lookAt matrix
	glm::mat4 ret = glm::lookAt(camPos, camPos + m_Target.vec3(), m_Up.vec3());
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