#include "Material.h"
#include "Vertex.h"
#include <vector>

namespace LepusEngine
{
	namespace Lepus3D
	{
		class Mesh {
			friend class RenderEngine;
		private:
			VertexArray m_Vertices;
			std::vector<unsigned int> m_Indices;
		public:
			Mesh();
			Mesh(VertexArray vertices, bool ignoreIndexing = false);
			VertexPack GetVertexBuffer();
			unsigned int* GetIndexBuffer(unsigned int& indexCount);
			void SetIndices(std::vector<unsigned int> indices);
		};
	}
}