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
	unsigned short lastUnique = 0;
	for (unsigned short i = 0; i < vCount; i++)
	{
		for (unsigned short j = 0; j < i && !repeated && !ignoreIndexing; j++)
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
	m_Indexed = !ignoreIndexing;
}

VertexPack Mesh::GetVertexBuffer()
{
	VertexPack ret = VertexPack(m_Vertices);
	return ret;
}

unsigned int* Mesh::GetIndexBuffer(unsigned int& indexCount)
{
	indexCount = m_Indices.size();
	unsigned int* ret = new unsigned int[indexCount];
	for (auto i = 0; i < indexCount; i++)
	{
		ret[i] = m_Indices[i];
	}
	auto last = ret[indexCount - 2];
	return ret;
}

void Mesh::SetIndices(std::vector<unsigned int> indices)
{
	m_Indices = indices;
	m_Indexed = true;
}

bool Mesh::IsIndexed()
{
	return m_Indexed;
}