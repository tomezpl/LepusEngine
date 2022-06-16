#pragma once

#define LEPUS_MATERIAL_ATTRIB_MAX_NAME_LENGTH 1024

namespace LepusEngine
{
	namespace Lepus3D
	{
		// Class template for a Material Attribute
		// Some class examples using this template are MaterialAttrib<Texture2D> for textures or MaterialAttrib<vec4> for colors.
		template <class T>
		class MaterialAttrib {
		public:
			char* name;
			T value;
			GLint location = -1;

			MaterialAttrib()
			{
				name = nullptr;
			}

			MaterialAttrib(const char* attribName) : MaterialAttrib()
			{ 
				size_t n = strnlen_s(attribName, LEPUS_MATERIAL_ATTRIB_MAX_NAME_LENGTH) + 1;
				this->name = new char[n];
				size_t sz = n * sizeof(char);
				strcpy_s(name, sz, attribName);
			};

			MaterialAttrib(const MaterialAttrib& other)
			{
				name = nullptr;
				
				if (other.name != nullptr)
				{
					size_t n = strnlen_s(other.name, LEPUS_MATERIAL_ATTRIB_MAX_NAME_LENGTH) + 1;
					name = new char[n];
					strcpy_s(name, sizeof(char) * n, other.name);
				}
			}

			~MaterialAttrib()
			{
				if (name != nullptr)
				{
					delete[] name;
				}
			}
		};
	}
}