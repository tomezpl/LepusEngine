#include "../Transform.h"

using namespace LepusEngine::Lepus3D;

Transform::Transform(Vector3 p, Vector3 r, Vector3 s) : Transform::Transform()
{
	m_Pos = glm::vec4(p.x, p.y, p.z, 1.f);
	m_Rot = glm::vec4(r.x, r.y, r.z, 1.f);
	m_Scale = glm::vec4(s.x, s.y, s.z, 1.f);
}

glm::mat4 Transform::GetMatrix()
{
	glm::mat4 ret;
	ret = glm::translate(ret, m_Pos);
	// Check if there is any rotation
	// otherwise it would fill the matrix with NaN for some reason
	if (m_Rot != Vector3(0.f, 0.f, 0.f).vec3())
		ret = glm::rotate(ret, toRadians(m_RotAngle), m_Rot);
	ret = glm::scale(ret, m_Scale);
	return ret;
}

void Transform::SetPosition(Vector3 p)
{
	m_Pos = p.vec3();
}

void Transform::SetRotation(Vector3 r)
{
	float maxAngle = 0.f;
	if (r.x >= r.y && r.x >= r.z)
		maxAngle = r.x;
	else if (r.y >= r.x && r.y >= r.z)
		maxAngle = r.y;
	else if (r.z >= r.x && r.z >= r.y)
		maxAngle = r.z;
	m_Rot = { r.x / maxAngle, r.y / maxAngle, r.z / maxAngle };
	m_RotAngle = maxAngle;
}

void Transform::SetScale(Vector3 s)
{
	m_Scale = s.vec3();
}

void Transform::SetScale(float s)
{
	SetScale(Vector3(s, s, s));
}

void Transform::Move(Vector3 translation)
{
	m_Pos.x += translation.x;
	m_Pos.y += translation.y;
	m_Pos.z += translation.z;
}

void Transform::Rotate(Vector3 rotation)
{
	rotation.x += m_Rot.x;
	rotation.y += m_Rot.y;
	rotation.z += m_Rot.z;
	SetRotation(rotation);
}

Vector3 Transform::GetPosition()
{
	return Vector3(m_Pos.x, m_Pos.y, m_Pos.z);
}

Vector3 Transform::GetRotation()
{
	return Vector3(m_Rot.x * m_RotAngle, m_Rot.y * m_RotAngle, m_Rot.z * m_RotAngle);
}

Vector3 Transform::GetScale()
{
	return Vector3(m_Scale.x, m_Scale.y, m_Scale.z);
}