#include "../Shader.h"
#include <iostream>
#include <GL/glew.h>

using namespace LepusEngine::Lepus3D;

Shader::Shader(char* name)
{
	m_Ready = false;
	if (!Load(name))
	{
#ifdef _DEBUG
		std::cout << "Shader::Shader(name): error while loading shader " << name << "." << std::endl;
#endif
	}
	else
	{
#ifdef _DEBUG
		std::cout << "Shader::Shader(name): success loading shader " << name << "." << std::endl;
#endif
		m_Ready = true;
	}
}

bool Shader::Load(char* name)
{
	m_ShaderName = name;

	if (!m_VShader.LoadShader(name))
	{
#ifdef _DEBUG
		std::cout << "Shader::Load(name): error, vertex shader " << name << " failed to load." << std::endl;
#endif
		return false;
	}
	if (!m_FShader.LoadShader(name))
	{
#ifdef _DEBUG
		std::cout << "Shader::Load(name): error, fragment shader " << name << " failed to load." << std::endl;
#endif
		return false;
	}

	GLint success;
	GLchar infoLog[512];

	m_VShader.m_Program = glCreateShader(GL_VERTEX_SHADER);
	m_FShader.m_Program = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* vShaderSource = this->VShaderSrc();
	const GLchar* fShaderSource = this->FShaderSrc();

	glShaderSource(m_VShader.m_Program, 1, &vShaderSource, NULL);
	glShaderSource(m_FShader.m_Program, 1, &fShaderSource, NULL);

	glCompileShader(m_VShader.m_Program);
	glCompileShader(m_FShader.m_Program);

	glGetShaderiv(m_VShader.m_Program, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_VShader.m_Program, 512, NULL, infoLog);
		std::cout << "Shader::Load(name): vertex shader " << m_ShaderName << " failed to compile (InfoLog: " << infoLog << ")" << std::endl;
		return false;
	}

	glGetShaderiv(m_FShader.m_Program, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_FShader.m_Program, 512, NULL, infoLog);
		std::cout << "Shader::Load(name): fragment shader " << m_ShaderName << " failed to compile (InfoLog: " << infoLog << ")" << std::endl;
		return false;
	}

	m_Compiled = glCreateProgram();
	glAttachShader(m_Compiled, m_VShader.m_Program);
	glAttachShader(m_Compiled, m_FShader.m_Program);
	glLinkProgram(m_Compiled);

	glGetProgramiv(m_Compiled, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_Compiled, 512, NULL, infoLog);
		std::cout << "Shader::Load(name): compiled shader " << m_ShaderName << " failed to link (InfoLog: " << infoLog << ")" << std::endl;
		return false;
	}

	m_Ready = true;
	return true;
}

const char* Shader::VShaderSrc()
{
	auto ret = m_VShader.m_ProgramStr.c_str();
	return ret;
}

const char* Shader::FShaderSrc()
{
	auto ret = m_FShader.m_ProgramStr.c_str();
	return ret;
}

char* Shader::Name()
{
	// The VShader and FShader share the names, so it doesn't matter which one we call
	char* ret = (char*)m_VShader.m_ProgramName.c_str();
	if (ret == "")
		ret = (char*)m_FShader.m_ProgramName.c_str();
	return ret;
}

bool Shader::Run()
{
	if(m_Ready)
		glUseProgram(m_Compiled);
	else
	{
#ifdef _DEBUG
		std::cout << "Shader::Run(): Shader " << m_ShaderName << " is not ready, it may have been unloaded or not loaded properly." << std::endl;
#endif
		return false;
	}
	return true;
}

void Shader::Unload()
{
	glDeleteShader(m_VShader.m_Program);
	glDeleteShader(m_FShader.m_Program);
	glDeleteProgram(m_Compiled);
	m_Ready = false;
}
