#include "../GLMesh.h"
using namespace lepus::gfx;

void GLMesh::_CreateVBO()
{
    if (GetFormat() != lepus::engine::MeshVertexFormat::Invalid)
    {
        glCreateBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, 0);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)VertexBufferSize(), GetVertices(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void GLMesh::_CreateIBO()
{
    // Create an IBO and upload index data to it.
    glCreateBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)IndexBufferSize(), GetIndices(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}