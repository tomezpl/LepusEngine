#ifndef L3D_GRAPHICSENGINE_API_VK_MESH
#define L3D_GRAPHICSENGINE_API_VK_MESH
#include "lepus/engine/Objects/Mesh.h"
#include "lepus/gfx/GraphicsEngine/Apis/ApiVk.h"

#include <vulkan/vulkan_core.h>

namespace lepus
{
    namespace gfx
    {
	class ApiVk;
	class VkMesh : public lepus::engine::objects::Mesh
	{
	    private:
	    friend class GraphicsApiVk;
	    VkBuffer m_vkVertBuffer, m_vkIndexBuffer;
	    uint32_t m_VertCount;

	    void _AllocateVertBuffer(const float* vertices, uint32_t vertexCount, GraphicsApiVk& api);
	    void _AllocateIndexBuffer(const uint32_t* indices, uint32_t indexCount, GraphicsApiVk& api);

	    public:
	    VkMesh()
	    {
		m_vkVertBuffer = VK_NULL_HANDLE;
		m_vkIndexBuffer = VK_NULL_HANDLE;
		m_VertCount = 0;
	    }

	    inline VkMesh(const float* vertices, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount, GraphicsApiVk& api)
	        : Mesh((float*)vertices, vertexCount * sizeof(float) * 3, lepus::engine::MeshVertexFormat::VVV, (uint32_t*)indices, indexCount, true)
	    {
		m_vkVertBuffer = VK_NULL_HANDLE;
		m_vkIndexBuffer = VK_NULL_HANDLE;
		m_VertCount = vertexCount;
		_AllocateVertBuffer(static_cast<const float*>(m_Vertices), vertexCount, api);
		_AllocateIndexBuffer(m_Indices, indexCount, api);
	    }

	    ~VkMesh()
	    {
		if (m_VertCount)
		{
		    m_VertCount = 0;
		}
	    }

	    inline const VkBuffer& GetVkVertBuffer() const
	    {
		return m_vkVertBuffer;
	    }

	    inline const VkBuffer& GetVkIndexBuffer() const
	    {
		return m_vkIndexBuffer;
	    }

	    inline const uint32_t& GetVertCount() const
	    {
		return m_VertCount;
	    }
	};
    } // namespace gfx
} // namespace lepus

#endif
