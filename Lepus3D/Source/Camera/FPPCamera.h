#pragma once

#include "../Camera.h"
#include "../RenderEngine.h"

using namespace LepusEngine;

namespace LepusEngine::Lepus3D {
	class FPPCamera : public Camera {
	protected:
		GLFWwindow* m_Wnd;
		float m_LastX, m_LastY;
	public:
		FPPCamera() : Camera() { m_LastX = m_LastY = 0.5f; };
		FPPCamera(Transform transform) : Camera(transform) { };
		FPPCamera(GLFWwindow* window);
		// Process input
		// Returns this->GetView()
		// Saves you that one line of code
		void SetWindow(GLFWwindow* window);
		glm::mat4 Run();
		void ProcessInput(float deltaTime);
	};
}