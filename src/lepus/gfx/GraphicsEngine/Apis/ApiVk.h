#ifndef L3D_GRAPHICSENGINE_APIS_APIVK
#define L3D_GRAPHICSENGINE_APIS_APIVK

#include "lepus/gfx/GraphicsEngine/GraphicsApi.h"

#define VK_USE_PLATFORM_WIN32_KHR
#include <VkBootstrap.h>
#include <vma/vk_mem_alloc.h>

#include "lepus/system/Windowing/GLFW.h"

namespace lepus
{
    namespace gfx
    {
	class GraphicsApiVkOptions : public GraphicsApiOptions
	{
	    public:
	    inline GraphicsApiType GetType() override
	    {
		return GraphicsApiType::GraphicsApiVulkan;
	    }

	    std::shared_ptr<system::WindowingGLFW> windowingPtr;
	};

	template GraphicsApiVkOptions& GraphicsApi::GetOptions<GraphicsApiVkOptions>();

	class GraphicsApiVk : public GraphicsApi
	{
	    private:
	    VkDevice m_vkDevice;
	    VkQueue m_vkQueue;
	    VkSurfaceKHR m_vkSurface;
	    VkInstance m_vkInstance;
	    VkSwapchainKHR m_vkSwapchain;
	    VkCommandBuffer m_CommandBuffer;
	    VkCommandPool m_CommandPool;
	    VkFramebuffer* m_FrameBuffers;
	    uint32_t m_FrameBufferCount;
	    uint32_t m_CurrentImageIndex;
	    uint32_t m_SwapChainImageCount;
	    VkImageView* m_ImageViews;
	    VkImage* m_Images;
	    VkFence m_vkFence, m_vkCmdBufFence;
	    PFN_vkCmdBeginRenderingKHR vkCmdBeginRenderingKHR;
	    PFN_vkCmdEndRenderingKHR vkCmdEndRenderingKHR;
	    VkPipeline m_vkGraphicsPipeline;
	    VkPipelineLayout m_vkGraphicsPipelineLayout;
	    VkShaderModule m_vkVertShader, m_vkFragShader;
	    VkBuffer m_vkVertBuffer;
	    VkDeviceMemory m_vkMemory;
	    VmaAllocator m_vmaAllocator;
	    VmaAllocation m_vmaAllocation;

	    inline void* GetUniformInternal(const char* name) override
	    {
		return nullptr;
	    }

	    public:
	    GraphicsApiVk()
	        : GraphicsApi()
	    {
		m_FrameBuffers = nullptr;
		m_FrameBufferCount = 0;
		m_CurrentImageIndex = 0;
		m_SwapChainImageCount = 0;
		m_ImageViews = nullptr;
		m_Images = nullptr;
		m_vkFence = VK_NULL_HANDLE;
		m_vkCmdBufFence = VK_NULL_HANDLE;
		m_vkGraphicsPipeline = VK_NULL_HANDLE;
		m_vkGraphicsPipelineLayout = VK_NULL_HANDLE;
		m_vkVertShader = VK_NULL_HANDLE;
		m_vkFragShader = VK_NULL_HANDLE;
		m_vkVertBuffer = VK_NULL_HANDLE;
		m_vkMemory = VK_NULL_HANDLE;
		m_vmaAllocator = VK_NULL_HANDLE;
		m_vmaAllocation = VK_NULL_HANDLE;
	    }

	    GraphicsApiVk(GraphicsApiOptions* options)
	    {
		m_FrameBuffers = nullptr;
		m_FrameBufferCount = 0;
		m_CurrentImageIndex = 0;
		m_SwapChainImageCount = 0;
		m_ImageViews = nullptr;
		m_Images = nullptr;
		m_vkFence = VK_NULL_HANDLE;
		m_vkCmdBufFence = VK_NULL_HANDLE;
		m_vkGraphicsPipeline = VK_NULL_HANDLE;
		m_vkVertShader = VK_NULL_HANDLE;
		m_vkFragShader = VK_NULL_HANDLE;
		GraphicsApiVk::Init(options);
	    }

	    void Init(GraphicsApiOptions* options) override;

	    void Draw() override {}

	    void CreatePipeline() override {}

	    void UpdateUniforms() override
	    {
	    }

	    void SwapBuffers() override;

	    void ClearFrameBuffer(float r, float g, float b) override;

	    engine::objects::Mesh* WrapMesh(engine::objects::Mesh* mesh) override;

	    void Shutdown() override;
	};
    } // namespace gfx
} // namespace lepus

#endif
