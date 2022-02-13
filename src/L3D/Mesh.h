#pragma once

#include "Material.h"
#include "Vertex.h"
#include <vector>

namespace LepusEngine
{
	// Forward class declaration to avoid including Physics code in Renderer code.
	class PhysxColliderMeshData;
	
	namespace Lepus3D
	{
		class Mesh {
			friend class RenderEngine;
			friend class Renderable;
			friend class Scene;
		private:
			std::vector<float> m_Vertices;
			std::vector<unsigned int> m_Indices;
			bool m_Indexed;
			Material* m_Mat; // only one material per mesh now - this will be changed with submeshes

			float* m_VertexBufferCache;
			unsigned int* m_IndexBufferCache;

			bool m_VertexBufferCacheDirty;
			bool m_IndexBufferCacheDirty;

			bool m_HasGLBuffers;

			inline bool HasCachedVertexBuffer() { return m_VertexBufferCache != nullptr; }
			inline bool HasCachedIndexBuffer() { return m_IndexBufferCache != nullptr; }

			// OpenGL buffers
			GLuint m_IBO, m_VBO, m_VAO;

			void GLUpdateIBO();
			void GLUpdateVBO();
		public:
			Mesh();
			Mesh(VertexArray vertices, bool ignoreIndexing = false);
			float* GetVertexBuffer();
			size_t GetVertexCount() const;
			unsigned int* GetIndexBuffer();
			size_t GetIndexCount() const;
			void SetIndices(std::vector<unsigned int> indices);
			void SetMaterial(Material& mat);
			bool IsIndexed() const;
			void FlipNormals();
			void ScaleVertices(float scale);
			void ScaleVertices(Vector3 scale);

			inline GLuint& GLGetVAO() { return m_VAO; }
			inline GLuint& GLGetIBO() { return m_IBO; }
			inline GLuint& GLGetVBO() { return m_VBO; }

			inline void GLSetIBO(GLuint ibo) { m_IBO = ibo; m_HasGLBuffers = true; }
			inline void GLSetVBO(GLuint vbo) { m_VBO = vbo; m_HasGLBuffers = true;  }
			inline void GLSetVAO(GLuint vao) { m_VAO = vao; }

			inline bool HasGLBuffers() const { return m_HasGLBuffers; }
		};

		class PlaneMesh : public Mesh {
		public:
			PlaneMesh() : Mesh(VertexArray {
				Vertex(-0.5f, 0.f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f), // Top-left
				Vertex(0.5f, 0.f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f), // Top-right
				Vertex(-0.5f, 0.f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f), // Bottom-left
				Vertex(0.5f, 0.f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f) // Bottom-right
			}) {
				this->SetIndices(std::vector<unsigned int>{0, 1, 2, 1, 3, 2});
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
					2,1,0,2,3,1,
					6,5,4,6,7,5,
					8,9,10,9,11,10,
					12,13,14,13,15,14,
					18,17,16,18,19,17,
					22,21,20,22,23,21
				});
			};
		};

		class BoxMeshUnindexed : public Mesh {
		public:
			BoxMeshUnindexed() : Mesh(VertexArray{
				// Front
				Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f), // Bottom-left
				Vertex(0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f), // Top-right
				Vertex(-0.5f, 0.5f, 0.5f, 0.f, 1.f, 0.0f, 0.0f, 1.0f), // Top-left
				Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f), // Bottom-right
				Vertex(0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f), // Top-right
				Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f), // Bottom-left
				// Right
				Vertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f), // Bottom-left
				Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f), // Top-right
				Vertex(0.5f, 0.5f, 0.5f, 0.f, 1.f, 1.0f, 0.0f, 0.0f), // Top-left
				Vertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f), // Bottom-right
				Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f), // Top-right
				Vertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f), // Bottom-left
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
				Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f), // Bottom-left
				Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f), // Top-right
				Vertex(-0.5f, 0.5f, -0.5f, 0.f, 1.f, 0.0f, 1.0f, 0.0f), // Top-left
				Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f), // Bottom-right
				Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f), // Top-right
				Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f), // Bottom-left
				// Left
				Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f), // Bottom-left
				Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f), // Top-right
				Vertex(-0.5f, 0.5f, -0.5f, 0.f, 1.f, -1.0f, 0.0f, 0.0f), // Top-left
				Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f), // Bottom-right
				Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f), // Top-right
				Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f) // Bottom-left
			}, true) 
			{
			};
		};
	}
}
