#pragma once

namespace LepusEngine
{
	namespace Lepus3D
	{
		// Vertex format
		class Vertex
		{
		public:
			float x, y, z, r, g, b;
			Vertex(float X = 0.f, float Y = 0.f, float Z = 0.f, float R = 0.f, float G = 0.f, float B = 0.f) { x = X; y = Y; z = Z; r = R; g = G; b = B; };
		};
	}
}