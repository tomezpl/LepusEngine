#pragma once

namespace LepusEngine
{
	namespace Lepus3D
	{
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