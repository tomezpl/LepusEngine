#pragma once

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
			MaterialAttrib(char* attribName)
			{ 
				size_t n = strnlen_s(attribName, 1024) + 1;
				this->name = new char[n];
				size_t sz = n * sizeof(char);
				strcpy_s(name, sz, attribName);
			};
		};
	}
}