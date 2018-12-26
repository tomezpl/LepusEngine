#pragma once

#include "../Camera.h"
#include "../RenderEngine.h"

using namespace LepusEngine;

namespace LepusEngine::Lepus3D {
	class FPPCamera; // forward declaration
	static FPPCamera* _fppCamPtr; // current instance
	class FPPCamera : public InputCamera {
	protected:
		GLFWwindow* m_Wnd;
		float m_LastX, m_LastY;
	public:
		static void ResizeCallback(GLFWwindow* wnd, int w, int h){
		glViewport(0, 0, w, h);
		_fppCamPtr->m_LastX = 0.5f;
		_fppCamPtr->m_LastY = 0.5f;

		}
		FPPCamera() : InputCamera() { m_LastX = m_LastY = 0.5f; };
		FPPCamera(Transform transform) : InputCamera(transform) { };
		FPPCamera(GLFWwindow* window);
		// Process input
		// Returns this->GetView()
		// Saves you that one line of code
		virtual void SetWindow(GLFWwindow* window);
		glm::mat4 Run();
		void ProcessInput(float deltaTime);
	};
#ifndef LEPUS_FPP_CAM_RESIZE_CLB
#define LEPUS_FPP_CAM_RESIZE_CLB
	// GLFW resize window callback - lastXY need to be reset to avoid erroneous input
	// TODO: there is still unexpected movement when the window gets resized...
/*	static void ResizeCallback(GLFWwindow* wnd, int w, int h) {
		glViewport(0, 0, w, h);
		_fppCamPtr->m_LastX = 0.5f;
		_fppCamPtr->m_LastY = 0.5f;
	}*/
#endif
}
