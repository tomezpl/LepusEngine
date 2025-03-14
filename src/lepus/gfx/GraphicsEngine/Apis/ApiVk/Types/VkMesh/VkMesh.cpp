#include "../VkMesh.h"

using namespace lepus::gfx;

void VkMesh::_AllocateVertBuffer(const float* vertices, uint32_t vertexCount, GraphicsApiVk& api)
{
    m_VertCount = vertexCount;
    m_vkVertBuffer = api.CreateBuffer(GraphicsApiVk::BufferType::VertexBuffer, vertices, vertexCount);
}

void VkMesh::_AllocateIndexBuffer(const uint32_t* indices, uint32_t indexCount, GraphicsApiVk& api)
{
    m_vkIndexBuffer = api.CreateBuffer(GraphicsApiVk::BufferType::IndexBuffer, indices, indexCount);
}
