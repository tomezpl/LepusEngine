#pragma once

namespace LepusEngine
{
	namespace Lepus3D
	{
		class Texture2D {
		private:
			unsigned char* m_Data;
			int m_Size;
			int m_Width;
			int m_Height;
			int m_Channels;
		public:
			Texture2D() { m_Data = nullptr; m_Width = m_Height = m_Size = m_Channels = 0; }
			Texture2D(char* fileName, char* directory = "../../Content");
			void Load(char* fileName, char* directory = "../../Content");
			unsigned char* GetData();
			int GetSize();
			int GetWidth();
			int GetHeight();
		};
	}
}