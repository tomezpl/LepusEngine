#include "../Texture.h"

using namespace LepusEngine::Lepus3D;

Texture2D::Texture2D(char* fileName)
{
	this->Load(fileName);
}

void Texture2D::Load(char* fN)
{
	std::string filePath = "../../Content/";
	filePath.append(fN);
	sf::Image image;
	image.loadFromFile(filePath);
	image.flipVertically(); // Texture will be sampled with GLSL's coordinates that define (0,0) as bottom left
	m_Width = image.getSize().x;
	m_Height = image.getSize().y;
	m_Size = m_Width * m_Height * sizeof(unsigned char) * 4;
	auto tempData = (unsigned char*)(image.getPixelsPtr());
	m_Data = new unsigned char[m_Width * m_Height * 4];
	for (auto i = 0; i < m_Width * m_Height * 4; i++) { m_Data[i] = tempData[i]; }
	//m_Data = m_Data;
}

unsigned char* Texture2D::GetData()
{
	return m_Data;
}

int Texture2D::GetSize()
{
	return m_Size;
}

unsigned short Texture2D::GetWidth()
{
	return m_Width;
}

unsigned short Texture2D::GetHeight()
{
	return m_Height;
}