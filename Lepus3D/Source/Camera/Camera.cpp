#include "../Camera.h"

using namespace LepusEngine;
using namespace Lepus3D;

Camera::Camera()
{
	this->_Init();
}

void Camera::_Init()
{
	Vector3 tmp = Vector3::Zero();
	m_Target = tmp;
	this->_CalcVectors();
}

void Camera::_CalcVectors()
{
	m_Up.Set(0.f, 1.f, 0.f);
	// this might break because Up is not always gonna be normalised
	auto camPos = m_Transform.GetPosition().vec3();
	Vector3 tmp = Vector3(glm::normalize(glm::cross(m_Up.vec3(), glm::normalize(camPos - m_Target.vec3()))));
	m_Right = tmp;
}

Camera::Camera(Transform transform) : Camera()
{
	this->SetTransform(transform);
	this->_Init();
}

void Camera::SetTransform(Transform transform)
{
	m_Transform = transform;
}

glm::mat4 Camera::GetView()
{
	this->_CalcVectors();
	auto camPos = m_Transform.GetPosition().vec3();
	//auto target = m_Target.vec3();
	auto rotation = m_Transform.GetRotation().vec3();
	Vector3 target(
		cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y)),
		sin(glm::radians(rotation.x)),
		cos(glm::radians(rotation.x)) * sin(glm::radians(rotation.y)));
	auto normTarget = glm::normalize(target.vec3());
	m_Target.x = normTarget.x;
	m_Target.y = normTarget.y;
	m_Target.z = normTarget.z;
	auto up = m_Up.vec3();
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