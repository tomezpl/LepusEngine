#include "../Mesh.h"

using namespace LepusEngine::Lepus3D;

Mesh::Mesh()
{
	m_Indexed = false;
	m_Mat = new Material("Default", "PerVertexUnlit");
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
		for (unsigned int i = 0; i < vCount; i++)
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
		for (unsigned int i = 0; i < vCount; i++)
		{
			m_Indices.push_back(i);
		}
	}
	m_Indexed = !ignoreIndexing;
}

float* Mesh::GetVertexBuffer()
{
	size_t vertexCount = m_Vertices.size();
	float* ret = new float[vertexCount];
	memcpy(ret, m_Vertices.data(), vertexCount * sizeof(float));
	return ret;
}

unsigned int Mesh::GetVertexCount()
{
	return (m_Indexed) ? m_Indices.size() : m_Vertices.size() / 8;
}

unsigned int* Mesh::GetIndexBuffer()
{
	size_t indexCount = m_Indices.size();
	unsigned int* ret = new unsigned int[indexCount];
	memcpy(ret, m_Indices.data(), indexCount * sizeof(unsigned int));
	return ret;
}

unsigned int Mesh::GetIndexCount()
{
	return m_Indices.size();
}

void Mesh::SetIndices(std::vector<unsigned int> indices)
{
	m_Indices = indices;
	m_Indexed = true;
}

void Mesh::SetMaterial(Material& mat)
{
	m_Mat = &mat;
}

bool Mesh::IsIndexed()
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