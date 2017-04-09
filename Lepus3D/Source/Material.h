#pragma once

#include "Shader.h"
#include "Material\MaterialAttrib.h"
#include <vector>

namespace LepusEngine
{
	namespace Lepus3D
	{
		class Material {
		private:
			Shader m_Shader;
			char* m_Name;
			std::vector<MaterialAttrib<GLint>> m_IntAttributes;
			std::vector<MaterialAttrib<GLfloat>> m_FloatAttributes;
			std::vector<MaterialAttrib<GLfloat[4]>> m_Vec4Attributes;
			std::vector<MaterialAttrib<GLfloat[3]>> m_Vec3Attributes;
			std::vector<MaterialAttrib<GLfloat[2]>> m_Vec2Attributes;
		public:
			Material(char* materialName);
			Material(char* materialName, char* shaderName);
			bool SetShader(char* shaderName);
			// Location will only be modified if it's >= 0
			bool SetAttributeI(char* attributeName, GLint value, GLint location = -1);
			bool SetAttributeF(char* attributeName, GLfloat value, GLint location = -1);
			bool SetAttributeF4(char* attributeName, GLfloat value[4], GLint location = -1);
			bool SetAttributeF3(char* attributeName, GLfloat value[3], GLint location = -1);
			bool SetAttributeF2(char* attributeName, GLfloat value[2], GLint location = -1);
			bool SetAttributeFP(char* attributeName, GLfloat* value, GLint location = -1);
			GLint GetAttributeI(char* attributeName);
			GLfloat GetAttributeF(char* attributeName);
			GLfloat* GetAttributeVec4(char* attributeName);
			GLfloat* GetAttributeVec3(char* attributeName);
			GLfloat* GetAttributeVec2(char* attributeName);
			void Use();
			void Destroy();
			~Material();
		};
	}
}