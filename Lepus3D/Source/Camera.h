#pragma once

#include "Transform.h"

#ifndef LEPUS_GLEW_INCLUDED
#define LEPUS_GLEW_INCLUDED
#include <GL/glew.h>
#endif
#ifndef LEPUS_GLFW_INCLUDED
#define LEPUS_GLFW_INCLUDED
#include <GLFW/glfw3.h>
#endif

namespace LepusEngine
{
	namespace Lepus3D
	{
		class Camera
		{
		protected:
			Transform m_Transform;
			Vector3 m_Target, m_Right, m_Up;
			void _CalcVectors();
			void _Init();
		public:
			Camera();
			Camera(Transform transform);
			void SetTransform(Transform transform);
			glm::mat4 GetView();
			Vector3 GetViewVector();
			Transform GetTransform();
		};

		class InputCamera : public Camera {
		protected:
			GLFWwindow* m_Wnd;
		public:
			InputCamera() : Camera() {};
			InputCamera(Transform transform) : Camera(transform) {};
			virtual void SetWindow(GLFWwindow* window);
		};
	}
}
