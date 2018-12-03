#include "../Texture.h"
#include <string>
#define STB_IMAGE_IMPLEMENTATION // This is critcal as otherwise the build will fail due to unresolved externals
#include "../3rdparty/stb_image.h"

using namespace LepusEngine::Lepus3D;

Texture2D::Texture2D(char* fileName, char* dir)
{
	this->Load(fileName, dir);
}

void Texture2D::Load(char* fN, char* dir)
{
	std::string filePath = dir;
	filePath.append("/");
	filePath.append(fN);
	m_Data = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_Channels, 4); // load RGBA texture

	if(filePath.find(".tga") == std::string::npos)
		stbi__vertical_flip(m_Data, m_Width, m_Height, 4); // non-TGA images need to be flipped vertically
	
	m_Size = m_Width * m_Height * sizeof(unsigned char) * 4;
}

unsigned char* Texture2D::GetData()
{
	return m_Data;
}

int Texture2D::GetSize()
{
	return m_Size;
}

int Texture2D::GetWidth()
{
	return m_Width;
}

int Texture2D::GetHeight()
{
	return m_Height;
}