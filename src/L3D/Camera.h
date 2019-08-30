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
		// Base camera class. Completely static, only moved programmatically. Derive from this to build your own camera models.
		class Camera
		{
		protected:
			// Transformation values
			Transform m_Transform;

			// Target and relative vector values
			Vector3 m_Target, m_Right, m_Up;

			// Calculates relative vectors based on target
			void _CalcVectors();

			// Initialises instance
			void _Init();
		public:
			// Default constructor. Initialises the camera looking at (0.0, 0.0, 0.0).
			Camera();

			// Initialises the camera with a transform (position & rotation), and calculates relative vectors to reflect it.
			Camera(Transform transform);

			// Assigns new transform to use for calculations.
			// transform is passed by copy, so you have to call this on each transform update.
			void SetTransform(Transform transform);

			// Returns the view matrix for this camera. This can be passed to RenderEngine for drawing calculations.
			glm::mat4 GetView();

			// Returns the target vector (where the camera is pointing)
			Vector3 GetViewVector();

			// Returns current transform
			Transform GetTransform();
		};

		// Base class for input-controlled camera. Requires handle to app window.
		class InputCamera : public Camera {
		protected:
			GLFWwindow* m_Wnd; // Pointer to window handle
		public:
			// Default constructor
			InputCamera() : Camera() {};

			// Initialises camera with a transform.
			// See Camera::Camera(Transform)
			InputCamera(Transform transform) : Camera(transform) {};
			
			// Sets window handle
			virtual void SetWindow(GLFWwindow* window);
		};
	}
}
