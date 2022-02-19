#pragma once

#include <vector>

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
		class Texture2D {
			friend class Material;
			friend class RenderEngine;
		private:
			unsigned char* m_Data;
			int m_Size;
			int m_Width;
			int m_Height;
			int m_Channels;
			char* m_Path;

			static std::vector<Texture2D*> _textureCache;

			GLuint m_GLTexture[1];
			bool m_HasGLTexture;
		public:
			Texture2D() { m_Data = nullptr; m_Path = nullptr; m_Width = m_Height = m_Size = m_Channels = 0; m_HasGLTexture = false; m_GLTexture[0] = 0; }
			Texture2D(const char* fileName, char* directory = "../../Content");
			bool Load(const char* fileName, char* directory = "../../Content");
			static Texture2D Default();
			unsigned char* GetData();
			int GetSize();
			int GetWidth();
			int GetHeight();

			void GLCreateTexture();
			void GLUploadTexture();
			void GLDestroyTexture();
		};
	}
}