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
	unsigned long long lastUnique = 0;
	if (!ignoreIndexing)
	{
		for (unsigned long long i = 0; i < vCount; i++)
		{
			for (unsigned long long j = 0; j < i && !repeated && !ignoreIndexing; j++)
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
		for (unsigned long long i = 0; i < vCount; i++)
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
		for (unsigned long long i = 0; i < vCount; i++)
		{
			m_Indices.push_back(i);
		}
	}
	m_Indexed = !ignoreIndexing;
}

float* Mesh::GetVertexBuffer()
{
	float* ret = m_Vertices.data();
	return ret;
}

unsigned long long Mesh::GetVertexCount()
{
	return (m_Indexed) ? m_Indices.size() : m_Vertices.size() / 8;
}

unsigned long long* Mesh::GetIndexBuffer(unsigned long long& indexCount)
{
	indexCount = m_Indices.size();
	return m_Indices.data();
}

unsigned long long Mesh::GetIndexCount()
{
	return m_Indices.size();
}

void Mesh::SetIndices(std::vector<unsigned long long> indices)
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
