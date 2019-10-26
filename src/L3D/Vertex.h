#pragma once

namespace LepusEngine
{
	namespace Lepus3D
	{
		// Vertex format
		class Vertex
		{
		public:
			float x, y, z, s, t, nX, nY, nZ;
			Vertex(float X = 0.f, float Y = 0.f, float Z = 0.f, float S = 0.f, float T = 0.f, float normX = 0.f, float normY = 0.f, float normZ = 0.f) { x = X; y = Y; z = Z; s = S; t = T; nX = normX; nY = normY; nZ = normZ; };
			Vertex operator=(Vertex& rs) { x = rs.x; y = rs.y; z = rs.z; s = rs.s; t = rs.t; nX = rs.nX; nY = rs.nY; nZ = rs.nZ; return *this; };
			bool operator==(Vertex& rs) { return x == rs.x && y == rs.y && z == rs.z && s == rs.s && t == rs.t && nX == rs.nX && nY == rs.nY && nZ == rs.nZ; };
			bool operator!=(Vertex& rs) { return !(*this == rs); };
		};

		typedef std::vector<Vertex> VertexArray;
		typedef std::vector<unsigned int> IndexArray;

		class VertexPack
		{
		private:
			unsigned int m_Size;
		public:
			Vertex* data;
			unsigned int size() { return m_Size; };
			VertexPack() { data = new Vertex(); m_Size = 0; }; // TODO: Vertex()? that should be Vertex[0] to create an empty array...
			VertexPack(VertexArray vertexData) { data = new Vertex[m_Size = vertexData.size()]; for (unsigned int i = 0; i < m_Size; i++) { data[i] = vertexData[i]; } };
		};
	}
}
