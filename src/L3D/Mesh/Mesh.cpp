#include "../Mesh.h"
#include <L3D/RenderEngine.h>

using namespace LepusEngine::Lepus3D;

Mesh::Mesh()
{
	m_Indexed = false;
	m_Mat = new Material("Default", "PerVertexUnlit");

	m_VertexBufferCache = nullptr;
	m_VertexBufferCacheDirty = false;

	m_IndexBufferCache = nullptr;
	m_IndexBufferCacheDirty = false;

	m_IBO = m_VBO = 0;
	m_HasGLBuffers = false;
}

Mesh::Mesh(VertexArray verts, bool ignoreIndexing) : Mesh()
{
	auto vCount = verts.size();
	bool repeated = false;
	unsigned int lastUnique = 0;
	if (!ignoreIndexing)
	{
		for (unsigned int i = 0; i < vCount; i++)
		{
			for (unsigned int j = 0; j < i && !repeated && !ignoreIndexing; j++)
			{
				if (verts[i] == verts[j])
				{
					m_Indices.push_back(j);
					repeated = true;
				}
			}
			if (!repeated)
			{
				// Convert to raw float array
				m_Vertices.push_back(verts[i].x);
				m_Vertices.push_back(verts[i].y);
				m_Vertices.push_back(verts[i].z);
				m_Vertices.push_back(verts[i].s);
				m_Vertices.push_back(verts[i].t);
				m_Vertices.push_back(verts[i].nX);
				m_Vertices.push_back(verts[i].nY);
				m_Vertices.push_back(verts[i].nZ);

				m_Indices.push_back(lastUnique++);
			}
			repeated = false;
		}
	}
	else
	{
		// Convert to raw float array
		for (size_t i = 0; i < vCount; i++)
		{
			m_Vertices.push_back(verts[i].x);
			m_Vertices.push_back(verts[i].y);
			m_Vertices.push_back(verts[i].z);
			m_Vertices.push_back(verts[i].s);
			m_Vertices.push_back(verts[i].t);
			m_Vertices.push_back(verts[i].nX);
			m_Vertices.push_back(verts[i].nY);
			m_Vertices.push_back(verts[i].nZ);
		}
		for (size_t i = 0; i < vCount; i++)
		{
			m_Indices.push_back(i);
		}
	}
	m_Indexed = !ignoreIndexing;

	m_VertexBufferCacheDirty = true;
	m_IndexBufferCacheDirty = true;

	GLUpdateIBO();
	GLUpdateVBO();
}

float* Mesh::GetVertexBuffer()
{
	if(!HasCachedVertexBuffer() || m_VertexBufferCacheDirty)
	{
		if (HasCachedVertexBuffer())
		{
			delete[] m_VertexBufferCache;
			m_VertexBufferCache = nullptr;
		}

		size_t vertexCount = m_Vertices.size();
		m_VertexBufferCache = new float[vertexCount];
		memcpy(m_VertexBufferCache, m_Vertices.data(), vertexCount * sizeof(float));

		m_VertexBufferCacheDirty = false;
	}

	return m_VertexBufferCache;
}

size_t Mesh::GetVertexCount() const
{
	return (m_Indexed) ? m_Indices.size() : m_Vertices.size() / 8;
}

unsigned int* Mesh::GetIndexBuffer()
{
	if (!HasCachedIndexBuffer() || m_IndexBufferCacheDirty)
	{
		if (HasCachedIndexBuffer())
		{
			delete[] m_IndexBufferCache;
			m_IndexBufferCache = nullptr;
		}

		size_t indexCount = m_Indices.size();
		m_IndexBufferCache = new unsigned int[indexCount];
		memcpy(m_IndexBufferCache, m_Indices.data(), indexCount * sizeof(unsigned int));

		m_IndexBufferCacheDirty = false;
	}

	return m_IndexBufferCache;
}

size_t Mesh::GetIndexCount() const
{
	return m_Indices.size();
}

void Mesh::SetIndices(std::vector<unsigned int> indices)
{
	m_Indices = indices;
	m_Indexed = true;

	GLUpdateIBO();
}

void Mesh::GLUpdateIBO()
{
	GLuint& vao = m_VAO;
	if (IsIndexed() && HasGLBuffers() && vao != 0)
	{
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetIndexCount() * sizeof(unsigned int), GetIndexBuffer(), GL_STATIC_DRAW); // pass elements/indices to GPU
		glBindVertexArray(0);
	}
}

void Mesh::GLUpdateVBO()
{
	GLuint& vao = m_VAO;
	if (HasGLBuffers() && vao != 0)
	{
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		// Set vertex positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * Vertex::ComponentCount(), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Set texture coords
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * Vertex::ComponentCount(), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// Set normal vectors
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * Vertex::ComponentCount(), (GLvoid*)(5 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), GetVertexBuffer(), GL_STATIC_DRAW); // pass vertices to GPU
		glBindVertexArray(0);
	}
}

void Mesh::SetMaterial(Material& mat)
{
	m_Mat = &mat;
}

bool Mesh::IsIndexed() const
{
	return m_Indexed;
}

void Mesh::FlipNormals()
{
	size_t nbVerts = m_Vertices.size();

	// The last 3 floats of a vertex should be the normals.
	for (size_t i = Vertex::ComponentCount() - 1; i < nbVerts; i += Vertex::ComponentCount())
	{
		m_Vertices[i - 2] *= -1.f;
		m_Vertices[i - 1] *= -1.f;
		m_Vertices[i] *= -1.f;
	}
}

void Mesh::ScaleVertices(float scale)
{
	ScaleVertices(Lepus3D::Vector3(scale, scale, scale));
}

void Mesh::ScaleVertices(Lepus3D::Vector3 scale)
{
	size_t nbVerts = m_Vertices.size();

	// The first 3 floats of a vertex should be the vertex position.
	for (size_t i = 0; i < nbVerts; i += Vertex::ComponentCount())
	{
		m_Vertices[i] *= scale.x;
		m_Vertices[i + 1] *= scale.y;
		m_Vertices[i + 2] *= scale.z;
	}
}