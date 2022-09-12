#ifndef LSYSTEM_WINDOWING_GLFW
#define LSYSTEM_WINDOWING_GLFW

#include "../Windowing.h"
#include <GL/gl3w.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <memory>
#include <cassert>

namespace LepusEngine
{
	namespace LepusSystem
	{
		class WindowingGLFW : public Windowing
		{
		private:
			GLFWwindow* m_Window;
			bool m_WindowDestroyed;
		public:
			inline void SetWindowDimensions(unsigned short width, unsigned short height) override
			{
				m_Dimensions.width = width;
				m_Dimensions.height = height;
			}

			void* GetWindowPtr() override { return m_Window; }

			WindowingGLFW()
			{
				m_WindowDestroyed = false;
			}

			WindowingGLFW(unsigned short windowWidth, unsigned short windowHeight)
			{
				m_WindowDestroyed = false;
				Init(windowWidth, windowHeight);
			}

			bool Init(unsigned short windowWidth, unsigned short windowHeight);

			void SetAsCurrentContext()
			{
				assert(m_Window);
				assert(!m_WindowDestroyed);

				// Make this window current OpenGL context.
				glfwMakeContextCurrent(m_Window);

				// Initialise GL3W loaders.
				assert(!gl3wInit());
			}

			void SwapBuffers() override;

			bool Update();

			void Shutdown();

			~WindowingGLFW()
			{
				Shutdown();
			};
		};
	}
}

#endif