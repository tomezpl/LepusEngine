#include "../GLMesh.h"
using namespace lepus::gfx;

void GLMesh::_CreateVBO()
{
    if (GetFormat() != lepus::engine::MeshVertexFormat::Invalid)
    {
	if (GetFormat() == engine::MeshVertexFormat::VVV || GetFormat() == engine::MeshVertexFormat::VVVST)
	{
	    glBindBuffer(GL_ARRAY_BUFFER, 0);
	    glCreateBuffers(1, &m_VBO);
	    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	    glEnableVertexAttribArray(0);
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, GetFormat() == engine::MeshVertexFormat::VVV ? 0 : (5 * sizeof(float)), nullptr);
	    if (GetFormat() == engine::MeshVertexFormat::VVVST)
	    {
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	    }
	    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)VertexBufferSize(), GetVertices(), GL_STATIC_DRAW);
	    glBindBuffer(GL_ARRAY_BUFFER, 0);

	    m_HasVBO = true;
	}
    }
}

void GLMesh::_CreateIBO()
{
    // Create an IBO and upload index data to it.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glCreateBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)IndexBufferSize(), GetIndices(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    m_HasIBO = true;
}