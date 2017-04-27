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
			virtual bool _LoadShader() {return false;};
			bool _LoadFile(char* fileName);
			GLuint m_Program;
		public:
			SLProgram(char* name = "");
			bool LoadShader(char* name = "");
		};

		class VShader : public SLProgram {
		protected:
			virtual bool _LoadShader();
		};
		class FShader : public SLProgram {
		protected:
			virtual bool _LoadShader();
		};
	}
}
