#pragma once

#include "Shader/SLProgram.h"

namespace LepusEngine
{
	namespace Lepus3D
	{
		// Shader class
		// This is used by Materials to describe the drawing functions for each drawcall and each triangle.
		// Shader reuse is encouraged to reduce shader compilation calls to OpenGL.
		class Shader {
			friend class Material;
			friend class Mesh;
			friend class RenderEngine;
		private:
			char* m_ShaderName; // Shader name to load (e.g. Phong)
			VShader m_VShader; // Vertex shader
			FShader m_FShader; // Fragment shader
			GLuint m_Compiled; // Compiled shader
			bool m_Ready; // Is Shader ready to use in drawcall?
		public:
			// Default constructor. Creates an empty, unready shader.
			Shader() { m_ShaderName = ""; m_Ready = false; };

			// Constructor. Takes a shader name and directory to look in.
			// Calls Load()
			Shader(char* name, char* directory = "../../Content/GLSL");

			// Loads in the shader from a file and compiles it.
			bool Load(char* name = "", char* directory = "../../Content/GLSL");

			// Provides a copy of the vertex shader source code.
			const char* VShaderSrc();
			
			// Provides a copy of the fragment shader source code.
			const char* FShaderSrc();

			// Returns the name of the shader that initialised this instance (e.g. Phong).
			char* Name();

			// Calls OpenGL to set this shader as the active program.
			bool Run();

			// Unloads the shader from memory.
			void Unload();
		};
	}
}
