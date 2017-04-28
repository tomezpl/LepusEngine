#pragma once

#include "Transform.h"

namespace LepusEngine
{
	namespace Lepus3D
	{
		class Camera
		{
		private:
			Transform m_Transform;
			Vector3 m_Target, m_Right, m_Up;
			void _CalcVectors();
			void _Init();
		public:
			Camera();
			Camera(Transform transform);
			void SetTransform(Transform transform);
			glm::mat4 GetView();
		};
	}
}