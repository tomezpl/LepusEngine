#include "SLProgram.h"
#include <LEngine/Logger.h>

using namespace LepusEngine::Lepus3D;

SLProgram::SLProgram(char* name, char* dir)
{
#ifdef _DEBUG
	if (name == "")
		//std::cout << "SLProgram(name): warning, program name is null, needs to be passed while loading" << std::endl;
		Logger::LogWarning("SLProgram", "SLProgram", "program name is null, need to pass while loading", "name");
#endif
	this->Load(name, dir);
}

bool SLProgram::Load(char* name, char* dir)
{
	if (name == "")
		return false;
	m_ProgramName = name;
		return this->_LoadShader(dir);
}

bool SLProgram::_LoadFile(char* fileName)
{
	std::ifstream fs;
	fs.open(fileName, std::ios::in);
	if (!(fs.is_open()))
	{
#ifdef _DEBUG
		//std::cout << "SLProgram::_LoadFile(fileName): error, filestream is not open!" << std::endl;
		Logger::LogError("SLProgram", "_LoadFile", "filestream is not open", "fileName");
#endif
		return false;
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
	Logger::LogInfo("SLProgram", "_LoadFile", "file loaded successfully", "fileName");
	return true;
}

bool VShader::_LoadShader(char* dir)
{
	std::string fileName = dir;
	fileName += "/";
	fileName += m_ProgramName;
	fileName += ".vert";
	return this->_LoadFile((char*)fileName.c_str());
}

bool FShader::_LoadShader(char* dir)
{
	std::string fileName = dir;
	fileName += "/";
	fileName += m_ProgramName;
	fileName += ".frag";
	return this->_LoadFile((char*)fileName.c_str());
}
