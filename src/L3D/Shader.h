#pragma once

#include "Shader/SLProgram.h"
#include <vector>

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
			
			GShader m_GShader; // Geometry shader
			bool m_HasGeometryShader;

			GLuint m_Compiled; // Compiled shader
			bool m_Ready; // Is Shader ready to use in drawcall?

			// TODO:
			// The shader cache needs some sort of refcounter to make sure we don't delete shaders while they're stil being used
			static std::vector<Shader> _shaderCache;

			static Shader* getFromShaderCache(const char* shaderName)
			{
				size_t nbShaders = _shaderCache.size();

				for (size_t i = 0; i < nbShaders; i++)
				{
					if (strcmp(_shaderCache[i].m_ShaderName, shaderName) == 0)
					{
						return &_shaderCache[i];
					}
				}

				return nullptr;
			}
		public:
			static void CleanupShaderCache()
			{
				size_t nbShaders = _shaderCache.size();

				for (size_t i = 0; i < nbShaders; i++)
				{
					_shaderCache[i].Unload();
				}
			}

			// Default constructor. Creates an empty, unready shader.
			Shader()
			{
				m_ShaderName = new char[1]{ 0 };
				m_Ready = false; 
				m_HasGeometryShader = false; 
				m_Compiled = 0; 
			}

			// Constructor. Takes a shader name and directory to look in.
			// Calls Load()
			Shader(const char* name, const char* directory = "../../Content/GLSL");

			// Loads in the shader from a file and compiles it.
			bool Load(const char* name = "", const char* directory = "../../Content/GLSL");

			Shader(const Shader&);

			// Provides a copy of the vertex shader source code.
			const char* VShaderSrc();
			
			// Provides a copy of the fragment shader source code.
			const char* FShaderSrc();

			const char* GShaderSrc();

			// Returns the name of the shader that initialised this instance (e.g. Phong).
			const char* Name();

			// Calls OpenGL to set this shader as the active program.
			bool Run();

			// Unloads the shader from memory.
			void Unload();

			~Shader();
		};
	}
}
