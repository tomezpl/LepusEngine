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
			Vertex operator=(Vertex& rs) { x = rs.x; y = rs.y; z = rs.z; r = rs.r; g = rs.g; b = rs.b; return *this;  };
			bool operator==(Vertex& rs) { return x == rs.x && y == rs.y && z == rs.z && r == rs.r && g == rs.g && b == rs.b; };
			bool operator!=(Vertex& rs) { return !(*this == rs); };
		};

		typedef std::vector<Vertex> VertexArray;

		class VertexPack
		{
		private:
			unsigned int m_Size;
		public:
			Vertex* data;
			unsigned int size() { return m_Size; };
			VertexPack() { data = new Vertex(); m_Size = 0; };
			VertexPack(VertexArray vertexData) { data = new Vertex[m_Size = vertexData.size()]; for (auto i = 0; i < m_Size; i++) { data[i] = vertexData[i]; } };
		};
	}
}