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

#include "ApiVk/Types/VkBufferAlloc.h"

namespace lepus::gfx
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
	VkDevice m_vkDevice{VK_NULL_HANDLE};
	VkQueue m_vkQueue{VK_NULL_HANDLE};
	VkSurfaceKHR m_vkSurface{VK_NULL_HANDLE};
	VkInstance m_vkInstance{VK_NULL_HANDLE};
	VkSwapchainKHR m_vkSwapchain{VK_NULL_HANDLE};
	VkCommandBuffer m_CommandBuffer{VK_NULL_HANDLE};
	VkCommandPool m_CommandPool{VK_NULL_HANDLE};
	VkFramebuffer* m_FrameBuffers{nullptr};
	uint32_t m_FrameBufferCount{0};
	uint32_t m_CurrentImageIndex{0};
	uint32_t m_SwapChainImageCount{0};
	VkImageView* m_ImageViews{nullptr};
	VkImage* m_Images{nullptr};
	VkFence m_vkFence{VK_NULL_HANDLE}, m_vkCmdBufFence{VK_NULL_HANDLE};
	PFN_vkCmdBeginRenderingKHR vkCmdBeginRenderingKHR{nullptr};
	PFN_vkCmdEndRenderingKHR vkCmdEndRenderingKHR{nullptr};
	utility::List<VkBufferAlloc> m_vkVertBuffers{utility::List<VkBufferAlloc>()};
	utility::List<VkBufferAlloc> m_vkIndexBuffers{utility::List<VkBufferAlloc>()};
	VkDeviceMemory m_vkMemory{VK_NULL_HANDLE};
	VmaAllocator m_vmaAllocator{nullptr};
	VmaAllocation m_vmaAllocation{nullptr};
	VmaAllocation m_vmaDepthBufAllocation{nullptr};
	VkRenderingInfo m_vkRenderingInfo{{}};
	VkRenderingAttachmentInfo m_vkColourAttachmentInfo{{}}, m_vkDepthAttachmentInfo{{}};
	VkImage m_vkDepthBuffer{VK_NULL_HANDLE};
	VkImageView m_vkDepthBufferView{VK_NULL_HANDLE};

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

	    /// @brief A default Vulkan pipeline layout for basic triangle rendering. Multiple pipelines can use this.
	    /// @remarks When cleaning up resources, make sure this only gets disposed once
	    /// (on each disposed pipeline, before disposing the layout check that it isn't this default one)
	    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
	} m_Defaults;

	utility::List<VkPipeline> m_vkGraphicsPipelines{utility::List<VkPipeline>()};
	utility::List<VkPipelineLayout> m_vkGraphicsPipelineLayouts{utility::List<VkPipelineLayout>()};
	utility::List<const lepus::gfx::VkShader*> m_vkGraphicsPipelineMetadata{utility::List<const lepus::gfx::VkShader*>()};
	size_t m_GraphicsPipelineCount{0};

	inline void* GetUniformInternal(const char* name) override
	{
	    return nullptr;
	}

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
	}

	explicit GraphicsApiVk(GraphicsApiOptions* options)
	{
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
} // namespace lepus::gfx

#endif
