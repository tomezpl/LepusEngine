#pragma once

#include "Shader\SLProgram.h"

namespace LepusEngine
{
	namespace Lepus3D
	{
		class Shader {
			friend class Material;
			friend class Mesh;
			friend class RenderEngine;
		private:
			char* m_ShaderName;
			VShader m_VShader;
			FShader m_FShader;
			GLuint m_Compiled;
			bool m_Ready;
		public:
			Shader() { m_ShaderName = ""; m_Ready = false; };
			Shader(char* name);
			bool Load(char* name = "");
			const char* VShaderSrc();
			const char* FShaderSrc();
			char* Name();
			bool Run();
			void Unload();
		};
	}
}