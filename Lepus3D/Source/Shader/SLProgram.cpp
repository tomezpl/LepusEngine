#include "SLProgram.h"
#include <iostream>

using namespace LepusEngine::Lepus3D;

SLProgram::SLProgram(char* name)
{
#ifdef _DEBUG
	if(name == "")
		std::cout << "SLProgram(name): warning, program name is null, needs to be passed while loading" << std::endl;
#endif
	this->LoadShader(name);
}

bool SLProgram::LoadShader(char* name)
{
	if (name == "")
		return false;
	m_ProgramName = name;
		return this->_LoadShader();
}

bool SLProgram::_LoadFile(char* fileName)
{
	std::ifstream fs;
	fs.open(fileName, std::ios::in);
	if (!(fs.is_open()))
	{
#ifdef _DEBUG
		std::cout << "SLProgram::_LoadFile(fileName): error, filestream is not open!" << std::endl;
#endif
	}
	if (fs.bad())
		return false;
	while (fs.good())
	{
		std::string line = "";
		getline(fs, line);
		if (!fs.eof())
			line += "\n";
		m_ProgramStr.append(line);
	}
	return true;
}

bool VShader::_LoadShader()
{
	std::string fileName = "../../GLSL/";
	fileName += m_ProgramName;
	fileName += ".vert";
	return this->_LoadFile((char*)fileName.c_str());
}

bool FShader::_LoadShader()
{
	std::string fileName = "../../GLSL/";
	fileName += m_ProgramName;
	fileName += ".frag";
	return this->_LoadFile((char*)fileName.c_str());
}