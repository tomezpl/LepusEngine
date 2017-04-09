#pragma once

#include <GL\glew.h>
#include <SFML\Window.hpp>
#include <SFML\OpenGL.hpp>

#include "Material.h"

namespace LepusEngine
{
	namespace Lepus3D
	{
		class RenderEngine
		{
		private:
			sf::Window m_Window;
			GLuint m_VBO, m_IBO, m_VAO, m_eCount;
			struct { 
				bool 
					window, 
					renderer; 
			} m_Ready;
			Material* m_CurrentMat;
		public:
			RenderEngine() { m_Ready = { false, false }; };
			RenderEngine(char* windowName, unsigned short windowWidth, unsigned short windowHeight);
			bool Init();
			bool Init(char* windowName, unsigned short windowWidth, unsigned short windowHeight);
			void DrawVertices(GLfloat* vertexData, GLuint vertexCount, GLuint* indices, GLuint indexCount, Material& material);
			
			// Runs the window's event loop, returns false if window was closed
			// Draws the current vertex buffer to the screen
			bool Update();
			
			void Shutdown();
			~RenderEngine();
		};
	}
}