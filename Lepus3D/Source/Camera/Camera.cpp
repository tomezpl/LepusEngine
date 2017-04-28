#include "../Camera.h"

using namespace LepusEngine;
using namespace Lepus3D;

Camera::Camera()
{
	this->_Init();
}

void Camera::_Init()
{
	m_Target = Vector3::Zero();
	this->_CalcVectors();
}

void Camera::_CalcVectors()
{
	m_Up.Set(0.f, 1.f, 0.f);
	// this might break because Up is not always gonna be normalised
	auto camPos = m_Transform.GetPosition().vec3();
	m_Right = Vector3(glm::normalize(glm::cross(m_Up.vec3(), glm::normalize(camPos - m_Target.vec3()))));
	m_Right = m_Right;
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
	auto target = m_Target.vec3();
	auto up = m_Up.vec3();
	glm::mat4 ret = glm::lookAt(camPos, target, up);
	return ret;
}