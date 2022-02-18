#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Transform/Vector.h"
#include "Material/MaterialAttrib.h"
#include <vector>

namespace LepusEngine
{
	namespace Lepus3D
	{
		// Material class
		// Describes how to render primitives, what shader to use and what data to pass to it.
		class Material {
			friend class RenderEngine;
		private:
			Shader m_Shader;
			char* m_Name;

			// Attribute vectors
			// These get passed to the GLSL shader
			// Separate vector for each data type
			// See SetAttribute/GetAttribute access functions.
			std::vector<MaterialAttrib<GLint>> m_IntAttributes;
			std::vector<MaterialAttrib<GLfloat>> m_FloatAttributes;
			std::vector<MaterialAttrib<GLfloat[4]>> m_Vec4Attributes;
			std::vector<MaterialAttrib<GLfloat[3]>> m_Vec3Attributes;
			std::vector<MaterialAttrib<GLfloat[2]>> m_Vec2Attributes;
			std::vector<MaterialAttrib<Texture2D>> m_TexAttributes;

			bool m_HasLoadedShader;
		public:
			Material() { m_HasLoadedShader = false; m_Name = nullptr; }
			Material(char* materialName);
			Material(char* materialName, char* shaderName);
			bool SetShader(char* shaderName);
			void SetName(char* materialName);
			const char* GetName();

			// Attribute setter functions
			// Location will only be modified if it's >= 0
			bool SetAttributeI(char* attributeName, GLint value, GLint location = 0);
			bool SetAttributeF(char* attributeName, GLfloat value, GLint location = 0);
			bool SetAttributeF4(char* attributeName, GLfloat value[4], GLint location = 0);
			bool SetAttributeF4(char* attributeName, Vector4 value, GLint location = 0);
			bool SetAttributeF3(char* attributeName, GLfloat value[3], GLint location = 0);
			bool SetAttributeF3(char* attributeName, Vector3 value, GLint location = 0);
			bool SetAttributeF2(char* attributeName, GLfloat value[2], GLint location = 0);
			bool SetAttributeF2(char* attributeName, Vector2 value, GLint location = 0);
			bool SetAttributeFP(char* attributeName, GLfloat* value, GLint location = 0);
			bool SetAttributeTex(char* attributeName, Texture2D value, int location = 0);

			// Attribute getter functions
			GLint GetAttributeI(char* attributeName);
			GLfloat GetAttributeF(char* attributeName);
			GLfloat* GetAttributeVec4(char* attributeName);
			GLfloat* GetAttributeVec3(char* attributeName);
			GLfloat* GetAttributeVec2(char* attributeName);
			Texture2D GetAttributeTex(char* attributeName);

			// Apply the material for the next drawcall
			void Use();

			// Release material resources, unload the shader from memory
			void Destroy();

			// Destructor. Calls Destroy()
			~Material();
		};
	}
}
