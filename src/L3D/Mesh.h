#pragma once

#include "Material.h"
#include "Vertex.h"
#include <vector>

namespace LepusEngine
{
	namespace Lepus3D
	{
		class Mesh {
			friend class RenderEngine;
			friend class Renderable;
		private:
			std::vector<float> m_Vertices;
			std::vector<unsigned long long> m_Indices;
			bool m_Indexed;
			Material* m_Mat; // only one material per mesh now - this will be changed with submeshes
		public:
			Mesh();
			Mesh(VertexArray vertices, bool ignoreIndexing = false);
			float* GetVertexBuffer();
			unsigned long long GetVertexCount();
			unsigned long long* GetIndexBuffer(unsigned long long& indexCount);
			void SetIndices(std::vector<unsigned long long> indices);
			void SetMaterial(Material& mat);
			bool IsIndexed();
		};

		class PlaneMesh : public Mesh {
		public:
			PlaneMesh() : Mesh(VertexArray {
				Vertex(-0.5f, 0.f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f), // Top-left
				Vertex(0.5f, 0.f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f), // Top-right
				Vertex(-0.5f, 0.f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f), // Bottom-left
				Vertex(0.5f, 0.f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f) // Bottom-right
			}) {
				this->SetIndices(std::vector<unsigned long long>{0, 1, 2, 1, 3, 2});
			};
		};

		class BoxMesh : public Mesh {
		public:
			BoxMesh() : Mesh(VertexArray{
				// Front
				Vertex(-0.5f, 0.5f, 0.5f, 0.f, 1.f), // Top-left
				Vertex(0.5f, 0.5f, 0.5f, 1.0f, 1.0f), // Top-right
				Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f), // Bottom-left
				Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f), // Bottom-right
				// Right
				Vertex(0.5f, 0.5f, 0.5f, 0.f, 1.f), // Top-left
				Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f), // Top-right
				Vertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f), // Bottom-left
				Vertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f), // Bottom-right
				// Bottom
				Vertex(-0.5f, -0.5f, -0.5f, 0.f, 1.f), // Top-left
				Vertex(0.5f, -0.5f, -0.5f, 1.0f, 1.0f), // Top-right
				Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f), // Bottom-left
				Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f), // Bottom-right
				// Back
				Vertex(-0.5f, 0.5f, -0.5f, 0.f, 1.f), // Top-left
				Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f), // Top-right
				Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f), // Bottom-left
				Vertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f), // Bottom-right
				// Top
				Vertex(-0.5f, 0.5f, -0.5f, 0.f, 1.f), // Top-left
				Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f), // Top-right
				Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f), // Bottom-left
				Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f), // Bottom-right
				// Left
				Vertex(-0.5f, 0.5f, -0.5f, 0.f, 1.f), // Top-left
				Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 1.0f), // Top-right
				Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f), // Bottom-left
				Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 0.0f) // Bottom-right
			}, true) {
				this->SetIndices(IndexArray{
					0,1,2,1,3,2,
					4,5,6,5,7,6,
					8,9,10,9,11,10,
					12,13,14,13,15,14,
					16,17,18,17,19,18,
					20,21,22,21,23,22
				});
			};
		};

		class BoxMeshUnindexed : public Mesh {
		public:
			BoxMeshUnindexed() : Mesh(VertexArray{
				// Front
				Vertex(-0.5f, 0.5f, 0.5f, 0.f, 1.f, 0.0f, 0.0f, 1.0f), // Top-left
				Vertex(0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f), // Top-right
				Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f), // Bottom-left
				Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f), // Bottom-left
				Vertex(0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f), // Top-right
				Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f), // Bottom-right
				// Right
				Vertex(0.5f, 0.5f, 0.5f, 0.f, 1.f, 1.0f, 0.0f, 0.0f), // Top-left
				Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f), // Top-right
				Vertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f), // Bottom-left
				Vertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f), // Bottom-left
				Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f), // Top-right
				Vertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f), // Bottom-right
				// Bottom
				Vertex(-0.5f, -0.5f, -0.5f, 0.f, 1.f, 0.0f, -1.0f, 0.0f), // Top-left
				Vertex(0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f), // Top-right
				Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f), // Bottom-left
				Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f), // Bottom-left
				Vertex(0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f), // Top-right
				Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f), // Bottom-right
				// Back
				Vertex(-0.5f, 0.5f, -0.5f, 0.f, 1.f, 0.0f, 0.0f, -1.0f), // Top-left
				Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f), // Top-right
				Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f), // Bottom-left
				Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f), // Bottom-left
				Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f), // Top-right
				Vertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f), // Bottom-right
				// Top
				Vertex(-0.5f, 0.5f, -0.5f, 0.f, 1.f, 0.0f, 1.0f, 0.0f), // Top-left
				Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f), // Top-right
				Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f), // Bottom-left
				Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f), // Bottom-left
				Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f), // Top-right
				Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f), // Bottom-right
				// Left
				Vertex(-0.5f, 0.5f, -0.5f, 0.f, 1.f, -1.0f, 0.0f, 0.0f), // Top-left
				Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f), // Top-right
				Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f), // Bottom-left
				Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f), // Bottom-left
				Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f), // Top-right
				Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f) // Bottom-right
			}, true) {

			};
		};
	}
}
