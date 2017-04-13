#include "../Material.h"
#include <iostream>

using namespace LepusEngine::Lepus3D;

Material::Material(char* name)
{
	m_Name = name;
}

Material::Material(char* name, char* shaderName) : Material(name)
{
	this->SetShader(shaderName);
}

bool Material::SetShader(char* shaderName)
{
	if (!m_Shader.m_Ready)
	{
		bool result = m_Shader.Load(shaderName);
		return result;
	}
	else
		return true;
}

bool Material::SetAttributeI(char* attributeName, GLint value, GLint location)
{
	short index = -1;

	for (short i = 0; i < m_IntAttributes.size() && index < 0; i++)
	{
		if (m_IntAttributes[i].name == attributeName)
			index = i;
	}
	
	if (index >= 0)
	{
		m_IntAttributes[index].value = value;
		if (location >= 0)
			m_FloatAttributes[index].location = glGetUniformLocation(m_Shader.m_Compiled, attributeName);
	}
	else
	{
		m_IntAttributes.push_back(MaterialAttrib<GLint>(attributeName));
		m_IntAttributes[m_IntAttributes.size() - 1].value = value;
		if (location >= 0)
			m_FloatAttributes[m_IntAttributes.size() - 1].location = glGetUniformLocation(m_Shader.m_Compiled, attributeName);
		return false;
	}

	return true;
}

bool Material::SetAttributeF(char* attributeName, GLfloat value, GLint location)
{
	short index = -1;

	for (short i = 0; i < m_FloatAttributes.size() && index < 0; i++)
	{
		if (m_FloatAttributes[i].name == attributeName)
			index = i;
	}

	if (index >= 0)
	{
		m_FloatAttributes[index].value = value;
		if (location >= 0)
			m_FloatAttributes[index].location = glGetUniformLocation(m_Shader.m_Compiled, attributeName);
	}
	else
	{
		m_FloatAttributes.push_back(MaterialAttrib<GLfloat>(attributeName));
		m_FloatAttributes[m_FloatAttributes.size() - 1].value = value;
		if (location >= 0)
			m_FloatAttributes[m_FloatAttributes.size() - 1].location = glGetUniformLocation(m_Shader.m_Compiled, attributeName);
		return false;
	}

	return true;
}

bool Material::SetAttributeF4(char* attributeName, GLfloat value[4], GLint location)
{
	short index = -1;

	for (short i = 0; i < m_Vec4Attributes.size() && index < 0; i++)
	{
		if (m_Vec4Attributes[i].name == attributeName)
			index = i;
	}

	if (index >= 0)
	{
		m_Vec4Attributes[index].value[0] = value[0];
		m_Vec4Attributes[index].value[1] = value[1];
		m_Vec4Attributes[index].value[2] = value[2];
		m_Vec4Attributes[index].value[3] = value[3];
		if (location >= 0)
			m_Vec4Attributes[index].location = glGetUniformLocation(m_Shader.m_Compiled, attributeName);
	}
	else
	{
		m_Vec4Attributes.push_back(MaterialAttrib<GLfloat[4]>(attributeName));
		index = m_Vec4Attributes.size() - 1;
		m_Vec4Attributes[index].value[0] = value[0];
		m_Vec4Attributes[index].value[1] = value[1];
		m_Vec4Attributes[index].value[2] = value[2];
		m_Vec4Attributes[index].value[3] = value[3];
		if (location >= 0)
			m_Vec4Attributes[index].location = glGetUniformLocation(m_Shader.m_Compiled, attributeName);
		return false;
	}

	return true;
}

bool Material::SetAttributeF3(char* attributeName, GLfloat value[3], GLint location)
{
	short index = -1;

	for (short i = 0; i < m_Vec3Attributes.size() && index < 0; i++)
	{
		if (m_Vec3Attributes[i].name == attributeName)
			index = i;
	}

	if (index >= 0)
	{
		m_Vec3Attributes[index].value[0] = value[0];
		m_Vec3Attributes[index].value[1] = value[1];
		m_Vec3Attributes[index].value[2] = value[2];
		if (location >= 0)
			m_Vec3Attributes[index].location = glGetUniformLocation(m_Shader.m_Compiled, attributeName);
	}
	else
	{
		m_Vec3Attributes.push_back(MaterialAttrib<GLfloat[3]>(attributeName));
		index = m_Vec3Attributes.size() - 1;
		m_Vec3Attributes[index].value[0] = value[0];
		m_Vec3Attributes[index].value[1] = value[1];
		m_Vec3Attributes[index].value[2] = value[2];
		if (location >= 0)
			m_Vec3Attributes[index].location = glGetUniformLocation(m_Shader.m_Compiled, attributeName);
		return false;
	}

	return true;
}

bool Material::SetAttributeF2(char* attributeName, GLfloat value[2], GLint location)
{
	short index = -1;

	for (short i = 0; i < m_Vec2Attributes.size() && index < 0; i++)
	{
		if (m_Vec2Attributes[i].name == attributeName)
			index = i;
	}

	if (index >= 0)
	{
		m_Vec2Attributes[index].value[0] = value[0];
		m_Vec2Attributes[index].value[1] = value[1];
		if (location >= 0)
			m_Vec2Attributes[index].location = glGetUniformLocation(m_Shader.m_Compiled, attributeName);
	}
	else
	{
		m_Vec2Attributes.push_back(MaterialAttrib<GLfloat[2]>(attributeName));
		index = m_Vec2Attributes.size() - 1;
		m_Vec2Attributes[index].value[0] = value[0];
		m_Vec2Attributes[index].value[1] = value[1];
		if (location >= 0)
			m_Vec2Attributes[index].location = glGetUniformLocation(m_Shader.m_Compiled, attributeName);
		return false;
	}

	return true;
}

bool Material::SetAttributeTex(char* attributeName, Texture2D value, int location)
{
	short index = -1;

	for (short i = 0; i < m_TexAttributes.size() && index < 0; i++)
	{
		if (m_TexAttributes[i].name == attributeName)
			index = i;
	}

	if (index >= 0)
	{
		m_TexAttributes[index].value = value;
		if (location >= 0)
			m_TexAttributes[index].location = glGetUniformLocation(m_Shader.m_Compiled, attributeName);
	}
	else
	{
		m_TexAttributes.push_back(MaterialAttrib<Texture2D>(attributeName));
		index = m_TexAttributes.size() - 1;
		m_TexAttributes[index].value = value;
		if (location >= 0)
			m_TexAttributes[index].location = glGetUniformLocation(m_Shader.m_Compiled, attributeName);
		return false;
	}

	return true;
}

bool Material::SetAttributeFP(char* attributeName, GLfloat* value, GLint location)
{
	std::size_t valueCount = sizeof(value) / sizeof(GLfloat);
	switch (valueCount)
	{
	case 4:
		return this->SetAttributeF4(attributeName, new GLfloat[4]{ value[0], value[1], value[2], value[3] }, location);
		break;
	case 3:
		return this->SetAttributeF3(attributeName, new GLfloat[3]{ value[0], value[1], value[2] }, location);
		break;
	case 2:
		return this->SetAttributeF2(attributeName, new GLfloat[2]{ value[0], value[1] }, location);
		break;
	case 1:
		return this->SetAttributeF(attributeName, *value, location);
		break;
	default:
		std::cout << "Material::SetAttribute(attributeName, *value, location): invalid number of floating-point values (" << valueCount << ")!" << std::endl;
		return false;
	}
}

GLint Material::GetAttributeI(char* attributeName)
{
	for (unsigned short i = 0; i < m_IntAttributes.size(); i++)
	{
		if (m_IntAttributes[i].name == attributeName)
		{
			return m_IntAttributes[i].value;
		}
	}
}

GLfloat Material::GetAttributeF(char* attributeName)
{
	for (unsigned short i = 0; i < m_FloatAttributes.size(); i++)
	{
		if (m_FloatAttributes[i].name == attributeName)
		{
			return m_FloatAttributes[i].value;
		}
	}
}

float* Material::GetAttributeVec4(char* attributeName)
{
	for (unsigned short i = 0; i < m_Vec4Attributes.size(); i++)
	{
		if (m_Vec4Attributes[i].name == attributeName)
		{
			return m_Vec4Attributes[i].value;
		}
	}
}

float* Material::GetAttributeVec3(char* attributeName)
{
	for (unsigned short i = 0; i < m_Vec3Attributes.size(); i++)
	{
		if (m_Vec3Attributes[i].name == attributeName)
		{
			return m_Vec3Attributes[i].value;
		}
	}
}

float* Material::GetAttributeVec2(char* attributeName)
{
	for (unsigned short i = 0; i < m_Vec2Attributes.size(); i++)
	{
		if (m_Vec2Attributes[i].name == attributeName)
		{
			return m_Vec2Attributes[i].value;
		}
	}
}

Texture2D Material::GetAttributeTex(char* attributeName)
{
	for (unsigned short i = 0; i < m_TexAttributes.size(); i++)
	{
		if (m_TexAttributes[i].name == attributeName)
		{
			return m_TexAttributes[i].value;
		}
	}
}

void Material::Use()
{
	m_Shader.Run(); // use the GLSL program

	for (int i = 0; i < m_IntAttributes.size(); i++)
	{
		glUniform1i(m_IntAttributes[i].location, m_IntAttributes[i].value);
	}
	for (int i = 0; i < m_FloatAttributes.size(); i++)
	{
		glUniform1f(m_FloatAttributes[i].location, m_FloatAttributes[i].value);
	}
	for (int i = 0; i < m_Vec4Attributes.size(); i++)
	{
		glUniform4f(m_Vec4Attributes[i].location, m_Vec4Attributes[i].value[0], m_Vec4Attributes[i].value[1], m_Vec4Attributes[i].value[2], m_Vec4Attributes[i].value[3]);
	}
	for (int i = 0; i < m_Vec3Attributes.size(); i++)
	{
		glUniform3f(m_Vec3Attributes[i].location, m_Vec3Attributes[i].value[0], m_Vec3Attributes[i].value[1], m_Vec3Attributes[i].value[2]);
	}
	for (int i = 0; i < m_Vec2Attributes.size(); i++)
	{
		glUniform2f(m_Vec2Attributes[i].location, m_Vec2Attributes[i].value[0], m_Vec2Attributes[i].value[1]);
	}
}

void Material::Destroy()
{
	m_Shader.Unload();
}

Material::~Material()
{
	this->Destroy();
}