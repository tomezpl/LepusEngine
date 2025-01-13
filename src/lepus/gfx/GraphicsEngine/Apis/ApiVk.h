#ifndef L3D_GRAPHICSENGINE_APIS_APIVK
#define L3D_GRAPHICSENGINE_APIS_APIVK

#include "lepus/gfx/GraphicsEngine/GraphicsApi.h"

#define VK_USE_PLATFORM_WIN32_KHR
#include <VkBootstrap.h>

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
	    }

	    GraphicsApiVk(GraphicsApiOptions* options)
	    {
		m_FrameBuffers = nullptr;
		m_FrameBufferCount = 0;
		m_CurrentImageIndex = 0;
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
