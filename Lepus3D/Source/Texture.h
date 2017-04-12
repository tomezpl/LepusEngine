#pragma once

#include <SFML/Graphics.hpp>

namespace LepusEngine
{
	namespace Lepus3D
	{
		class Texture2D {
		private:
			unsigned char* m_Data;
			int m_Size;
			unsigned short m_Width;
			unsigned short m_Height;
		public:
			Texture2D() { m_Data = nullptr; m_Width = m_Height = m_Size = 0; }
			Texture2D(char* fileName);
			void Load(char* fileName);
			unsigned char* GetData();
			int GetSize();
			unsigned short GetWidth();
			unsigned short GetHeight();
		};
	}
}