#include "../Texture.h"
#include <string>
#define STB_IMAGE_IMPLEMENTATION // This is critcal as otherwise the build will fail due to unresolved externals
#include "../3rdparty/stb_image.h"

using namespace LepusEngine::Lepus3D;

Texture2D::Texture2D(char* fileName)
{
	this->Load(fileName);
}

// TODO: update with stbi
void Texture2D::Load(char* fN)
{
	std::string filePath = "../../Content/";
	filePath.append(fN);
	//image.loadFromFile(filePath);
	//image.flipVertically(); // Texture will be sampled with GLSL's coordinates that define (0,0) as bottom left
	m_Data = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_Channels, 4); // load RGBA texture
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