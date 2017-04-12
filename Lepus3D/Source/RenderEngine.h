#pragma once

#include <GL\glew.h>
#include <SFML\Window.hpp>
#include <SFML\OpenGL.hpp>

#include "Material.h"
#include "Vertex.h"
#include "Mesh.h"

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
			Mesh* m_CurrentMesh;
			Material* m_CurrentMat;
			GLuint m_TextureSet[16];
		public:
			RenderEngine() { m_Ready = { false, false }; };
			RenderEngine(char* windowName, unsigned short windowWidth, unsigned short windowHeight);
			bool Init();
			bool Init(char* windowName, unsigned short windowWidth, unsigned short windowHeight);
			void DrawMesh(Mesh& mesh, Material& material);
			void DrawVertices(Vertex* vD, GLuint vDS, GLuint* iD, GLuint iC, Material& mat);
			
			// Runs the window's event loop, returns false if window was closed
			// Draws the current vertex buffer to the screen
			bool Update();
			
			void Shutdown();
			~RenderEngine();
		};
	}
}