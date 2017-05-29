#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Transform/Vector.h"

namespace LepusEngine
{
	namespace Lepus3D
	{
#define PI 3.14159265359f
#define toRadians(degrees) ((degrees / 180.0f) * PI)
#define toDegrees(radians) ((radians / PI) * 180.0f)

	class Transform {
	private:
		glm::vec3 m_Pos, m_Rot, m_Scale;
		float m_RotAngle;
	public:
		Transform() { m_Pos = m_Rot = { 0.f, 0.f, 0.f }; m_Scale = { 1.f, 1.f, 1.f }; m_RotAngle = 0.f; };
		Transform(Vector3 position, Vector3 rotation, Vector3 scale);
		glm::mat4 GetMatrix(); // calculate a transformation matrix
		void SetPosition(Vector3 position);
		void SetRotation(Vector3 rotation);
		void SetScale(Vector3 scale);
		void SetScale(float scale);
		void Move(Vector3 translation);
		void Rotate(Vector3 rotation);
		Vector3 GetPosition();
		Vector3 GetRotation();
		Vector3 GetScale();
	};
	}
}