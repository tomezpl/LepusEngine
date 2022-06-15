#pragma once

#include "Material.h"
#include "Vertex.h"
#include "Geometry.h"
#include <vector>

namespace LepusEngine
{
	// Forward class declaration to avoid including Physics code in Renderer code.
	class PhysxColliderMeshData;
	
	namespace Lepus3D
	{
		class Mesh : protected Geometry {
			friend class RenderEngine;
			friend class Renderable;
			friend class Scene;
		private:
			std::vector<unsigned int> m_Indices;
			bool m_Indexed;
			Material* m_Mat; // only one material per mesh now - this will be changed with submeshes
			bool m_OwnsMaterial;

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
			Mesh(const Mesh&);
			~Mesh();
			float* GetVertexBuffer();
			virtual size_t GetVertexCount() const;
			unsigned int* GetIndexBuffer();
			size_t GetIndexCount() const;
			void SetIndices(std::vector<unsigned int> indices);
			void SetMaterial(Material& mat);
			bool IsIndexed() const;
			void FlipNormals();
			void ScaleVertices(float scale);
			void ScaleVertices(Vector3 scale);

			virtual float* CopyXYZ();

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
				Vertex(-0.5f, 0.5f, 0.5f, 0.f, 1.f, 0.f, 0.f, 1.f), // Top-left
				Vertex(0.5f, 0.5f, 0.5f, 1.f, 1.f, 0.f, 0.f, 1.f), // Top-right
				Vertex(-0.5f, -0.5f, 0.5f, 0.f, 0.f, 0.f, 0.f, 1.f), // Bottom-left
				Vertex(0.5f, -0.5f, 0.5f, 1.f, 0.f, 0.f, 0.f, 1.f), // Bottom-right
				// Right
				Vertex(0.5f, 0.5f, 0.5f, 0.f, 1.f, 1.f, 0.f, 0.f), // Top-left
				Vertex(0.5f, 0.5f, -0.5f, 1.f, 1.f, 1.f, 0.f, 0.f), // Top-right
				Vertex(0.5f, -0.5f, 0.5f, 0.f, 0.f, 1.f, 0.f, 0.f), // Bottom-left
				Vertex(0.5f, -0.5f, -0.5f, 1.f, 0.f, 1.f, 0.f, 0.f), // Bottom-right
				// Bottom
				Vertex(-0.5f, -0.5f, -0.5f, 0.f, 1.f, 0.f, -1.f, 0.f), // Top-left
				Vertex(0.5f, -0.5f, -0.5f, 1.f, 1.f, 0.f, -1.f, 0.f), // Top-right
				Vertex(-0.5f, -0.5f, 0.5f, 0.f, 0.f, 0.f, -1.f, 0.f), // Bottom-left
				Vertex(0.5f, -0.5f, 0.5f, 1.f, 0.f, 0.f, -1.f, 0.f), // Bottom-right
				// Back
				Vertex(-0.5f, 0.5f, -0.5f, 0.f, 1.f, 0.f, 0.f, -1.f), // Top-left
				Vertex(0.5f, 0.5f, -0.5f, 1.f, 1.f, 0.f, 0.f, -1.f), // Top-right
				Vertex(-0.5f, -0.5f, -0.5f, 0.f, 0.f, 0.f, 0.f, -1.f), // Bottom-left
				Vertex(0.5f, -0.5f, -0.5f, 1.f, 0.f, 0.f, 0.f, -1.f), // Bottom-right
				// Top
				Vertex(-0.5f, 0.5f, -0.5f, 0.f, 1.f, 0.f, 1.f, 0.f), // Top-left
				Vertex(0.5f, 0.5f, -0.5f, 1.f, 1.f, 0.f, 1.f, 0.f), // Top-right
				Vertex(-0.5f, 0.5f, 0.5f, 0.f, 0.f, 0.f, 1.f, 0.f), // Bottom-left
				Vertex(0.5f, 0.5f, 0.5f, 1.f, 0.f, 0.f, 1.f, 0.f), // Bottom-right
				// Left
				Vertex(-0.5f, 0.5f, -0.5f, 0.f, 1.f, -1.f, 0.f, 0.f), // Top-left
				Vertex(-0.5f, 0.5f, 0.5f, 1.f, 1.f, -1.f, 0.f, 0.f), // Top-right
				Vertex(-0.5f, -0.5f, -0.5f, 0.f, 0.f, -1.f, 0.f, 0.f), // Bottom-left
				Vertex(-0.5f, -0.5f, 0.5f, 1.f, 0.f, -1.f, 0.f, 0.f) // Bottom-right
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

		class BoxGeometry : protected Geometry
		{
		public:
			BoxGeometry()
			{
				VertexArray vertices = VertexArray{
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
				};

				unsigned char nbVertices = vertices.size();
				unsigned short nbVertexFloats = (unsigned short)nbVertices * Vertex::ComponentCount();
				m_Vertices.resize(nbVertexFloats);

				for (size_t i = 0; i < nbVertices; i++)
				{
					m_Vertices[i+0] = vertices[i].x;
					m_Vertices[i + 1] = vertices[i].y;
					m_Vertices[i + 2] = vertices[i].z;
					m_Vertices[i + 3] = vertices[i].s;
					m_Vertices[i + 4] = vertices[i].t;
					m_Vertices[i + 5] = vertices[i].nX;
					m_Vertices[i + 6] = vertices[i].nY;
					m_Vertices[i + 7] = vertices[i].nZ;
				}
			}

			float* CopyXYZ()
			{
				size_t nbVertices = GetVertexCount();

				float* pointsData = new float[GetVertexCount() * 3];
				float* vertices = m_Vertices.data();

				// Copy first 3 floats (xyz) from each Vertex
				for (unsigned int i = 0, j = 0; i < nbVertices * 3, j < nbVertices * 8; i += 3, j += 8)
				{
					memcpy(pointsData + i, vertices + j, sizeof(float) * 3);
				}

				return pointsData;
			}

			size_t GetVertexCount() const
			{
				return m_Vertices.size() / 8;
			}
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
