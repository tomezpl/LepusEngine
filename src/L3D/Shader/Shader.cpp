#include "../Shader.h"
#include <iostream>
#include <GL/glew.h>

using namespace LepusEngine::Lepus3D;

std::vector<Shader> Shader::_shaderCache = std::vector<Shader>();

Shader::Shader(char* name, char* dir)
{
	m_Ready = false;

	// Perform loading process
	if (!Load(name, dir))
	{
#ifdef _DEBUG
		std::cout << "Shader::Shader(name, dir): error while loading shader " << name << "." << std::endl;
#endif
	}
	else
	{
#ifdef _DEBUG
		std::cout << "Shader::Shader(name, dir): success loading shader " << name << "." << std::endl;
#endif
		m_Ready = true;
	}
}

bool Shader::Load(char* name, char* dir)
{
	m_ShaderName = new char[strlen(name)+1];

	strcpy(m_ShaderName, name);

	Shader* cached = getFromShaderCache(name);
	if (cached)
	{
#ifdef _DEBUG
		std::cout << "Shader::Load(name, dir): found a cached version of " << name << ", using that instead." << std::endl;
#endif
		m_Compiled = cached->m_Compiled;
		m_Ready = true;
		return true;
	}

	// Load vertex & fragment shader source code from file
	if (!m_VShader.Load(name, dir))
	{
#ifdef _DEBUG
		std::cout << "Shader::Load(name, dir): error, vertex shader " << name << " failed to load." << std::endl;
#endif
		return false;
	}
	if (!m_FShader.Load(name, dir))
	{
#ifdef _DEBUG
		std::cout << "Shader::Load(name, dir): error, fragment shader " << name << " failed to load." << std::endl;
#endif
		return false;
	}

	GLint success; // success value of the loading process
	GLchar infoLog[512]; // buffer for error messages, etc.

	// Initialise empty GL shader instances
	m_VShader.m_Program = glCreateShader(GL_VERTEX_SHADER);
	m_FShader.m_Program = glCreateShader(GL_FRAGMENT_SHADER);

	// Copy loaded shader source code
	const GLchar* vShaderSource = this->VShaderSrc();
	const GLchar* fShaderSource = this->FShaderSrc();

	// Feed the shader source code to GL shader instances
	glShaderSource(m_VShader.m_Program, 1, &vShaderSource, NULL);
	glShaderSource(m_FShader.m_Program, 1, &fShaderSource, NULL);

	// Compile the GL shader instances
	glCompileShader(m_VShader.m_Program);
	glCompileShader(m_FShader.m_Program);

	// Get compile status of the vertex shader, store as success value
	glGetShaderiv(m_VShader.m_Program, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_VShader.m_Program, 512, NULL, infoLog);
		std::cout << "Shader::Load(name, dir): vertex shader " << m_ShaderName << " failed to compile (InfoLog: " << infoLog << ")" << std::endl;
		return false; // Fail loading process if vertex shader compilation fails
	}

	// Get compile status of the fragment shader, store as success value
	glGetShaderiv(m_FShader.m_Program, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_FShader.m_Program, 512, NULL, infoLog);
		std::cout << "Shader::Load(name, dir): fragment shader " << m_ShaderName << " failed to compile (InfoLog: " << infoLog << ")" << std::endl;
		return false; // Fail loading process if fragment shader compilation fails
	}

	// Create final compiled shader
	m_Compiled = glCreateProgram();
	glAttachShader(m_Compiled, m_VShader.m_Program);
	glAttachShader(m_Compiled, m_FShader.m_Program);
	glLinkProgram(m_Compiled);

	// Get link status of the final compiled shader, store as success value
	glGetProgramiv(m_Compiled, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_Compiled, 512, NULL, infoLog);
		std::cout << "Shader::Load(name, dir): compiled shader " << m_ShaderName << " failed to link (InfoLog: " << infoLog << ")" << std::endl;
		return false; // Fail loading process if shader linking fails
	}

	_shaderCache.push_back(*this);

	// Set Shader as ready to draw and return loading success
	m_Ready = true;
	return true;
}

const char* Shader::VShaderSrc()
{
	const char* ret = m_VShader.m_ProgramStr.c_str();
	return ret;
}

const char* Shader::FShaderSrc()
{
	const char* ret = m_FShader.m_ProgramStr.c_str();
	return ret;
}

char* Shader::Name()
{
	// The VShader and FShader share the names, so it doesn't matter which one we call.
	char* ret = (char*)m_VShader.m_ProgramName.c_str();

	// Should the Vertex shader return a blank name, try retrieving it from the Fragment shader.
	if (ret == "")
		ret = (char*)m_FShader.m_ProgramName.c_str();

	return ret;
}

bool Shader::Run()
{
	// Check if Shader is ready to draw, and set it as currently used GL program if so.
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
	// Delete the shader data with OpenGL and unready this instance
	glDeleteShader(m_VShader.m_Program);
	glDeleteShader(m_FShader.m_Program);
	glDeleteProgram(m_Compiled);
	m_Ready = false;
}
