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
			virtual bool _LoadShader(char* directory) {return false;};
			bool _LoadFile(char* fileName);
			GLuint m_Program;
		public:
			SLProgram(char* name = "", char* directory = "../../Content/GLSL");
			bool Load(char* name = "", char* directory = "../../Content/GLSL");
		};

		class VShader : public SLProgram {
		protected:
			virtual bool _LoadShader(char* directory);
		};
		class FShader : public SLProgram {
		protected:
			virtual bool _LoadShader(char* directory);
		};
	}
}
