#ifndef L3D_GRAPHICSENGINE_APIS_APIVK
#define L3D_GRAPHICSENGINE_APIS_APIVK

#include "lepus/gfx/GraphicsEngine/GraphicsApi.h"

#ifndef WIN32
#include "vk_mem_alloc.h"
#include <VkBootstrap.h>
#else
#define VK_USE_PLATFORM_WIN32_KHR
#include <VkBootstrap.h>
#include <vma/vk_mem_alloc.h>
#endif

#include "ApiVk/Types/VkShader.h"
#include "lepus/system/Windowing/GLFW.h"
#include "lepus/utility/types/List.h"

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
	    const VkBuffer* m_vkVertBuffers;
	    const VkBuffer* m_vkIndexBuffers;
	    size_t m_vkVertBufferCount;
	    size_t m_vkIndexBufferCount;
	    VkDeviceMemory m_vkMemory;
	    VmaAllocator m_vmaAllocator;
	    VmaAllocation m_vmaAllocation;
	    VmaAllocation m_vmaDepthBufAllocation;
	    VkRenderingInfo m_vkRenderingInfo;
	    VkRenderingAttachmentInfo m_vkColourAttachmentInfo, m_vkDepthAttachmentInfo;
	    VkImage m_vkDepthBuffer;
	    VkImageView m_vkDepthBufferView;

	    struct VulkanDefaults
	    {
		VkFormat colourFormat = VK_FORMAT_B8G8R8A8_SRGB;
		VkPipelineRenderingCreateInfoKHR pipelineRenderingCreateInfo = {};
		VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
		VkVertexInputBindingDescription vertexBinding = {};
		VkVertexInputAttributeDescription vertexAttributes = {};
		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {};
		VkPipelineTessellationStateCreateInfo tessStateCreateInfo = {};
		VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
		VkPipelineMultisampleStateCreateInfo msStateCreateInfo = {};
		VkPipelineRasterizationStateCreateInfo rasterStateCreateInfo = {};
		VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {};
		VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = {};
		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		VkPushConstantRange pushConstantRange = {};
		VkViewport viewport = {};
		VkRect2D scissor = {};
		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
	    } m_Defaults;

	    utility::List<VkPipeline> m_vkGraphicsPipelines;
	    utility::List<VkPipelineLayout> m_vkGraphicsPipelineLayouts;
	    utility::List<const VkShader*> m_vkGraphicsPipelineMetadata;
	    size_t m_GraphicsPipelineCount;

	    const char* m_TempShaderNameOut = nullptr;

	    inline void* GetUniformInternal(const char* name) override
	    {
		return nullptr;
	    }

	    /**
	     * Checks that the vertex buffer array has enough capacity to insert new buffer(s).
	     * @param newCount New count (or capacity) for the vertex buffer array.
	     * @return Newly allocated number of vertex buffers.
	     */
	    size_t _EnsureVertBufferCapacity(size_t newCount);

	    size_t _EnsureIndexBufferCapacity(size_t newCount);

	    inline size_t findPipelineIndex(const ShaderInfo* shaderInfo)
	    {
		for (size_t i = 0; i < m_GraphicsPipelineCount; i++)
		{
		    if (m_vkGraphicsPipelineMetadata.Get(i) == shaderInfo)
		    {
			return i;
		    }
		}

		return SIZE_MAX;
	    }

	    public:
	    enum BufferType
	    {
		VertexBuffer,
		IndexBuffer
	    };

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
		m_vkVertBufferCount = 0;
		m_vkIndexBufferCount = 0;
		m_vkVertBuffers = new VkBuffer[m_vkVertBufferCount];
		m_vkIndexBuffers = new VkBuffer[m_vkIndexBufferCount];
		m_vkMemory = VK_NULL_HANDLE;
		m_vmaAllocator = VK_NULL_HANDLE;
		m_vmaAllocation = VK_NULL_HANDLE;
		m_vkRenderingInfo = {};
		m_vkColourAttachmentInfo = {};
		m_vkDepthAttachmentInfo = {};
		m_vkDepthBuffer = VK_NULL_HANDLE;
		m_vkDepthBufferView = VK_NULL_HANDLE;
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
		m_vkRenderingInfo = {};
		m_vkVertBufferCount = 0;
		m_vkIndexBufferCount = 0;
		m_vkVertBuffers = new VkBuffer[m_vkVertBufferCount];
		m_vkIndexBuffers = new VkBuffer[m_vkIndexBufferCount];
		m_vkDepthAttachmentInfo = {};
		m_vkDepthBuffer = VK_NULL_HANDLE;
		m_vkDepthBufferView = VK_NULL_HANDLE;
		GraphicsApiVk::Init(options);
	    }

	    VkBuffer CreateBuffer(BufferType type, const void* data, uint64_t elementCount);

	    void Init(GraphicsApiOptions* options) override;

	    void Draw(const SceneGraph& scene) override;

	    void StartDrawing() override;
	    void EndDrawing() override;

	    void CreatePipeline() override;

	    void UpdateUniforms(const SceneGraph& scene) override;

	    void SwapBuffers() override;

	    void ClearFrameBuffer(float r, float g, float b) override;

	    engine::objects::Mesh* WrapMesh(engine::objects::Mesh* mesh) override;

	    [[nodiscard]] engine::ShaderAssetType GetShaderAssetType() const override
	    {
		return engine::ShaderAssetTypeSPV;
	    }

	    const char* GetShaderFileName(const char* shaderName, ShaderStage stage) const override;

	    inline void AddShader(const VkShader* shader)
	    {
		m_GraphicsPipelineCount = m_vkGraphicsPipelineMetadata.Push(shader);
	    }

	    void Shutdown() override;
	};
    } // namespace gfx
} // namespace lepus

#endif
