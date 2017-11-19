#pragma once

#include "../Camera.h"
#include "../Window.h"

using namespace LepusEngine;

namespace LepusEngine::Lepus3D {
	class FPPCamera : public Camera {
	protected:
		Window* m_Wnd;
		float m_LastX, m_LastY;
	public:
		FPPCamera() : Camera() { m_LastX = m_LastY = 0.5f; };
		FPPCamera(Transform transform) : Camera(transform) { };
		FPPCamera(Window* window);
		// Process input
		// Returns this->GetView()
		// Saves you that one line of code
		void SetWindow(Window* window);
		glm::mat4 Run();
		void ProcessInput(float deltaTime);
	};
}