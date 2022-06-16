#pragma once

#include <fstream>
#include <string>
#include <GL/glew.h>

namespace LepusEngine
{
	namespace Lepus3D
	{
		// Shader language program
		// Abstract base class for vertex and fragment shaders
		class SLProgram {
			friend class Shader;
		protected:
			std::string m_ProgramStr, m_ProgramName;
			virtual bool _LoadShader(const char* directory) {return false;};
			bool _LoadFile(const char* fileName);
			GLuint m_Program;
		public:
			SLProgram(const char* name = "", const char* directory = "../../Content/GLSL");
			bool Load(const char* name = "", const char* directory = "../../Content/GLSL");

			SLProgram(const SLProgram&);

			~SLProgram();
		};

		class VShader : public SLProgram 
		{
		protected:
			virtual bool _LoadShader(const char* directory);
		};

		class FShader : public SLProgram 
		{
		protected:
			virtual bool _LoadShader(const char* directory);
		};

		class GShader : public SLProgram 
		{
		protected:
			virtual bool _LoadShader(const char* directory);
		};
	}
}
