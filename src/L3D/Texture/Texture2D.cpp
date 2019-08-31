#include "../Texture.h"
#include <string>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION // This is critcal as otherwise the build will fail due to unresolved externals
#include "../3rdparty/stb_image.h"

using namespace LepusEngine::Lepus3D;

Texture2D::Texture2D(char* fileName, char* dir)
{
	this->Load(fileName, dir);
}

bool Texture2D::Load(char* fN, char* dir)
{
	std::string filePath = dir;
	filePath.append("/");
	filePath.append(fN);

	// Check if file exists. If not, load the default texture (1x1 white).
	std::ifstream file(filePath.c_str());

	if(!file.good())
	{
		Texture2D default = Texture2D::Default();
		m_Data = default.m_Data;
		m_Size = default.m_Size;
		m_Channels = default.m_Channels;
		m_Width = default.m_Width;
		m_Height = default.m_Height;

		return false;
	}
	
	m_Data = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_Channels, 4); // load RGBA texture

	if(filePath.find(".tga") == std::string::npos)
		stbi__vertical_flip(m_Data, m_Width, m_Height, 4); // non-TGA images need to be flipped vertically
	
	m_Size = m_Width * m_Height * sizeof(unsigned char) * 4;

	return true;
}

Texture2D Texture2D::Default()
{
	Texture2D texture;
	texture.m_Data = new unsigned char[4] {255, 255, 255, 255};
	texture.m_Channels = 4;
	texture.m_Size = sizeof(unsigned char) * 4; // TODO: is this sizeof really necessary?
	texture.m_Width = texture.m_Height = 1;

	return texture;
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