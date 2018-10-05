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
				m_Vertices.push_back(verts[i]);
				m_Indices.push_back(lastUnique++);
			}
			repeated = false;
		}
	}
	else
	{
		m_Vertices = verts;
		for (unsigned long long i = 0; i < vCount; i++)
		{
			m_Indices.push_back(i);
		}
	}
	m_Indexed = !ignoreIndexing;
}

VertexPack Mesh::GetVertexBuffer()
{
	VertexPack ret = VertexPack(m_Vertices);
	return ret;
}

unsigned long long* Mesh::GetIndexBuffer(unsigned long long& indexCount)
{
	indexCount = m_Indices.size();
	unsigned long long* ret = new unsigned long long[indexCount];
	for (auto i = 0; i < indexCount; i++)
	{
		ret[i] = m_Indices[i];
	}
	auto last = ret[indexCount - 2];
	return ret;
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
