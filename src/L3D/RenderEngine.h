#pragma once

#ifndef LEPUS_GLEW_INCLUDED
#define LEPUS_GLEW_INCLUDED
#include <GL/glew.h>
#endif
#ifndef LEPUS_GLFW_INCLUDED
#define LEPUS_GLFW_INCLUDED
#include <GLFW/glfw3.h>
#endif

#define LEPUS_MAX_TEXTURE_COUNT 16

#include "Material.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Transform.h"
#include "Camera.h"
#include "Scene.h"

#include <map>

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
			GLuint m_TextureSet[LEPUS_MAX_TEXTURE_COUNT];
			double m_LastFrameTime, m_ElapsedTime;
			Camera* m_Cam;

			static GLuint m_MeshVAO;

			std::map<const char*, void*> m_OverridenUniforms;

			inline void refreshOverridenUniforms()
			{
				for (auto it = m_OverridenUniforms.begin(); it != m_OverridenUniforms.end(); it++)
				{
					delete it->second;
				}

				m_OverridenUniforms.clear();
			}

			void bindOverridenUniforms(GLuint& program);

		public:
			static GLuint GLGetGlobalMeshVAO();

			RenderEngine() { m_Ready = { false, false }; };
			RenderEngine(char* windowName, unsigned short windowWidth, unsigned short windowHeight);
			bool Init();
			bool Init(char* windowName, unsigned short windowWidth, unsigned short windowHeight);
			void StartScene(Camera* camera);
			void DrawMesh(Mesh& mesh, Material& material, Transform& transform);
			void DrawScene(Scene& scene);
			GLFWwindow* GetWindowPtr() { return m_Window; };
			void EndScene();

			template <typename T> void OverrideUniform(const char* name, T value)
			{
				T* valCopy = new T(value);
				size_t nbKeyChars = strlen(name) + 1;
				char* keyCopy = new char[nbKeyChars * sizeof(char)];
				strcpy(keyCopy, name);

				auto existingIterator = m_OverridenUniforms.find(name);
				if (existingIterator != m_OverridenUniforms.end())
				{
					delete existingIterator->second;
					existingIterator->second = (void*)valCopy;
				}
				else
				{
					m_OverridenUniforms.emplace(keyCopy, (void*)valCopy);
				}

				delete[] keyCopy;
			}

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