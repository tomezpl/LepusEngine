#pragma once

#ifndef LEPUS_GLEW_INCLUDED
#define LEPUS_GLEW_INCLUDED
#include <GL/glew.h>
#endif
#ifndef LEPUS_GLFW_INCLUDED
#define LEPUS_GLFW_INCLUDED
#include <GLFW/glfw3.h>
#endif

#include "Material.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Transform.h"
#include "Camera.h"
#include "Scene.h"

namespace LepusEngine
{
	namespace Lepus3D
	{
		class RenderEngine
		{
		private:
			GLFWwindow* m_Window;
			char* m_WindowName;
			GLuint m_VBO, m_IBO, m_VAO;
			unsigned long long m_eCount;
			struct { 
				bool 
					window, 
					renderer; 
			} m_Ready;
			GLuint m_TextureSet[16];
			double m_LastFrameTime, m_ElapsedTime;
			Camera* m_Cam;
		public:
			RenderEngine() { m_Ready = { false, false }; };
			RenderEngine(char* windowName, unsigned short windowWidth, unsigned short windowHeight);
			bool Init();
			bool Init(char* windowName, unsigned short windowWidth, unsigned short windowHeight);
			void StartScene(Camera* camera);
			void DrawMesh(Mesh& mesh, Material& material, Transform& transform);
			void DrawScene(Scene& scene);
			GLFWwindow* GetWindowPtr() { return m_Window; };
			void EndScene();

			// Runs the window's event loop, returns false if window was closed
			// Draws the current vertex buffer to the screen
			bool Update();

			// Returns the time it took to render the last frame
			double LastFrameTime();

			void DumpToFile(std::string filePath = "output.bmp");
			
			void Shutdown();
			~RenderEngine();
		};
	}
}