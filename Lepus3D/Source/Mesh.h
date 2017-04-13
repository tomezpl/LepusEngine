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

		class PlaneMesh : public Mesh {
		public:
			PlaneMesh() : Mesh(VertexArray {
				Vertex(-0.5f, 0.5f, 0.f, 1.f, 1.f, 1.f, 0.0f, 1.0f), // Top-left
				Vertex(0.5f, 0.5f, 0.f, 1.f, 1.f, 1.f, 1.0f, 1.0f), // Top-right
				Vertex(-0.5f, -0.5f, 0.f, 1.f, 1.f, 1.f, 0.0f, 0.0f), // Bottom-left
				Vertex(0.5f, -0.5f, 0.f, 1.f, 1.f, 1.f, 1.0f, 0.0f) // Bottom-right
			}) {
				this->SetIndices(std::vector<unsigned int>{0, 1, 2, 1, 3, 2});
			};
		};
	}
}