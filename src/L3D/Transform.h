#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Transform/Vector.h"
#include <L3D/Quaternion.h>
#include <string>

namespace LepusEngine
{
	namespace Lepus3D
	{
#define PI 3.14159265359f
#define toRadians(degrees) ((degrees / 180.0f) * PI)
#define toDegrees(radians) ((radians / PI) * 180.0f)

	class Transform {
	private:
		glm::vec3 m_Pos, m_Scale;
		glm::quat m_Rot;
	public:
		Transform() { m_Pos = { 0.f, 0.f, 0.f }; m_Rot = glm::angleAxis(0.f, glm::vec3(0.f, 0.f, 0.f)); m_Scale = { 1.f, 1.f, 1.f }; };
		Transform(Vector3 position, Vector3 rotation, Vector3 scale);
		glm::mat4 GetMatrix(); // calculate a transformation matrix
		void SetPosition(Vector3 position);
		void SetRotation(Quaternion rotation); // rotation as a quaternion
		void SetRotation(Vector3 axis, double angle); // rotation in axis-angle
		void SetScale(Vector3 scale);
		void SetScale(float scale);
		void Move(Vector3 translation);
		void Rotate(Vector3 rotation);
		Vector3 GetPosition();
		Quaternion GetRotation();
		Vector3 GetScale();
		std::string ToString();
	};

	class Transformable {
	friend class RenderEngine;
	protected:
		Transform mTransform;
	public:
		Transformable() { }
		void SetPosition(Vector3 position) { mTransform.SetPosition(position); }
		void SetRotation(Quaternion rotation) { mTransform.SetRotation(rotation); }
		void SetRotation(Vector3 rotation) { mTransform.SetRotation(rotation); }
		void SetScale(Vector3 scale) { mTransform.SetScale(scale); }
		void SetScale(float scale) { mTransform.SetScale(scale); }
		void Move(Vector3 translation) { mTransform.Move(translation); }
		void Rotate(Vector3 rotation) { mTransform.Rotate(rotation); }
		Transform GetTransform() { return mTransform; }
	};
	}
}
