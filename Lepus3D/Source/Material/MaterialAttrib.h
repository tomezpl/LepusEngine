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
			const char* name;
			T value;
			GLint location = 0;
			MaterialAttrib(char* attribName) { this->name = attribName; };
		};
	}
}