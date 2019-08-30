#pragma once

#include "../Camera.h"
#include "../RenderEngine.h"

using namespace LepusEngine;

namespace LepusEngine::Lepus3D {
	class FPPCamera; // Forward declaration of the class
	
	// Current instance of the camera.
	// Used in callback functions which are outside of the class scope.
	// Also prevents the engine automatically processing all cameras' input at once, especially if only one is being used.
	static FPPCamera* _fppCamPtr;
	
	// First-person free-flight camera.
	// Simplistic, recommended for debug.
	// If you are building a game, you probably DON'T want to use this. No collision detection, no gameplay systems, etc.
	class FPPCamera : public InputCamera {
	protected:
		GLFWwindow* m_Wnd; // Window handle
		float m_LastX, m_LastY; // XY coordinates of the cursor in the last frame
	public:
		// Window-resize callback used by GLFW
		static void ResizeCallback(GLFWwindow* wnd, int w, int h){
			glViewport(0, 0, w, h);

			// Recenter the cursor coordinates to prevent involuntary movement.
			_fppCamPtr->m_LastX = 0.5f;
			_fppCamPtr->m_LastY = 0.5f;
		}

		// Default constructor. Initialises the cursor coordinates.
		FPPCamera() : InputCamera() { m_LastX = m_LastY = 0.5f; };

		// Constructor. Initialises the camera with an existing Transform, and centers the cursor coordinates.
		FPPCamera(Transform transform) : InputCamera(transform) { m_LastX = m_LastY = 0.5f; };

		// Constructor. Initialises the camera with a window handle.
		FPPCamera(GLFWwindow* window) { this->SetWindow(window); };

		// Sets the window handle for the camera to take mouse input from.
		virtual void SetWindow(GLFWwindow* window);

		// Returns the View matrix.
		// Should be called every frame.
		glm::mat4 Run();

		// Retrieves window input and updates the camera Transform accordingly.
		// Should be called every frame.
		void ProcessInput(float deltaTime);
	};
}
