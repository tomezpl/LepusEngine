#include "../Transform.h"

using namespace LepusEngine::Lepus3D;

Transform::Transform(Vector3 p, Vector3 r, Vector3 s) : Transform::Transform()
{
	m_Pos = glm::vec4(p.x, p.y, p.z, 1.f);
	m_Scale = glm::vec4(s.x, s.y, s.z, 1.f);
}

glm::mat4 Transform::GetMatrix()
{
	glm::mat4 ret = glm::mat4(1.0);
	ret = glm::translate(ret, m_Pos);
	// Check if there is any rotation
	// otherwise it would fill the matrix with NaN for some reason
	//if (m_Rot != Vector3(0.f, 0.f, 0.f).vec3())
		//ret = glm::rotate(ret, toRadians(m_RotAngle), m_Rot);
	ret *= glm::mat4_cast(glm::quat(m_Rot.w, m_Rot.x, m_Rot.y, m_Rot.z));
	ret = glm::scale(ret, m_Scale);
	return ret;
}

void Transform::SetPosition(Vector3 p)
{
	m_Pos = p.vec3();
}

void Transform::SetRotation(Quaternion r)
{
	m_Rot = glm::quat(r.w, r.x, r.y, r.z);
}

void LepusEngine::Lepus3D::Transform::SetRotation(Vector3 axis, double angle)
{
	m_Rot = glm::angleAxis((float)angle, axis.vec3());
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
	auto ret = Vector3::Create(m_Pos.x, m_Pos.y, m_Pos.z);
	return ret;
}

Quaternion Transform::GetRotation()
{
	return Quaternion(m_Rot.x, m_Rot.y, m_Rot.z, m_Rot.w);
}

Vector3 Transform::GetScale()
{
	return Vector3(m_Scale.x, m_Scale.y, m_Scale.z);
}

std::string Transform::ToString()
{
	Vector3 p = this->GetPosition();
	Quaternion r = this->GetRotation();
	Vector3 s = this->GetScale();

	std::string ret = "P ";
	ret += p.ToString();
	ret += ", R ";
	ret += r.ToString();
	ret += ", S ";
	ret += s.ToString();
	
	return ret;
}