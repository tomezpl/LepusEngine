#pragma once

#include <vector>

namespace LepusEngine
{
	namespace Lepus3D
	{
		class Geometry {
		protected:
			std::vector<float> m_Vertices;
		public:
			virtual float* CopyXYZ() = 0;
			virtual size_t GetVertexCount() const = 0;
		};
	}
}