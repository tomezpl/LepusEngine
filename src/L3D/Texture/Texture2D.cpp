#include "../Texture.h"
#include <string>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION // This is critcal as otherwise the build will fail due to unresolved externals
#include "../3rdparty/stb_image.h"

using namespace LepusEngine::Lepus3D;

std::vector<Texture2D*> Texture2D::_textureCache = std::vector<Texture2D*>();

Texture2D::Texture2D(const char* fileName, char* dir, TextureRole role) : Texture2D()
{
	this->Load(fileName, dir, role);
}

bool Texture2D::Load(const char* fN, char* dir, TextureRole role)
{
	SetRole(role);

	std::string filePath = dir;
	filePath.append("/");
	filePath.append(fN);

	// Check cache first.
	size_t nbCached = _textureCache.size();

	// The index of the cached texture we found.
	size_t foundCached = 0;
	for (size_t i = 0; i < nbCached; i++)
	{
		if (strcmp(_textureCache[i]->m_Path, filePath.c_str()) == 0)
		{
			foundCached = i + 1;
			break;
		}
	}

	if (foundCached == 0)
	{
		// Check if file exists. If not, load the default texture (1x1 white).
		std::ifstream file(filePath.c_str());

		if (!file.good())
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

		if (filePath.find(".tga") == std::string::npos)
			stbi__vertical_flip(m_Data, m_Width, m_Height, 4); // non-TGA images need to be flipped vertically

		m_Size = m_Width * m_Height * sizeof(unsigned char) * 4;

		m_Path = new char[filePath.length() + 1];
		strcpy(m_Path, filePath.c_str());

		_textureCache.push_back(this);
	}
	else
	{
		// foundCached is actually index + 1 because it's unsigned. Normally if it was signed we'd just use -1 for not found.
		m_Data = _textureCache[foundCached - 1]->m_Data;
		m_Size = _textureCache[foundCached - 1]->m_Size;
		m_Width = _textureCache[foundCached - 1]->m_Width;
		m_Height = _textureCache[foundCached - 1]->m_Height;
		m_Channels = _textureCache[foundCached - 1]->m_Channels;

		m_Path = new char[filePath.length() + 1];
		strcpy(m_Path, filePath.c_str());
	}

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

void Texture2D::GLCreateTexture()
{
	if (!m_HasGLTexture)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, m_GLTexture);
		m_HasGLTexture = true;
	}
}

void Texture2D::GLUploadTexture()
{
	if (m_HasGLTexture)
	{
		glBindTexture(GL_TEXTURE_2D, m_GLTexture[0]);
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GetWidth(), GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, GetData());
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Texture2D::GLDestroyTexture()
{
	if (m_HasGLTexture)
	{
		glDeleteTextures(1, m_GLTexture);
		m_HasGLTexture = false;
	}
}

void Texture2D::SetRole(TextureRole role)
{
	m_TextureRole = role;
}