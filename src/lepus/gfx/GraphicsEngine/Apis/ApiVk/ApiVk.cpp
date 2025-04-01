// #include <Windows.h>

#include "lepus/system/IO/FileSystem.h"
#include "lepus/utility/types/Matrix4x4.h"

#define VMA_IMPLEMENTATION
#include "Types/VkMesh.h"

#include <lepus/gfx/GraphicsEngine/Apis/ApiVk.h>

// #include <vulkan/vulkan_win32.h>

using namespace lepus::gfx;

void GraphicsApiVk::Init(GraphicsApiOptions* options)
{

    GraphicsApiVkOptions* vkOptions = static_cast<GraphicsApiVkOptions*>(options);
    InitInternal(vkOptions);

    vkb::InstanceBuilder builder;
    auto vkbInstanceWrapper = builder.set_app_name("Lepus Engine").request_validation_layers().use_default_debug_messenger().build();
    assert(vkbInstanceWrapper);
    vkb::Instance vkbInstance = vkbInstanceWrapper.value();

    // Create surface from window
    GLFWwindow* window = static_cast<GLFWwindow*>(vkOptions->windowingPtr->GetWindowPtr());
    glfwCreateWindowSurface(vkbInstance.instance, window, nullptr, &m_vkSurface);
    // HWND hwnd = glfwGetWin32Window(window);
    // HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE);
    /*    VkWin32SurfaceCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.flags = 0;
        createInfo.pNext = nullptr;
        createInfo.hinstance = hInstance;
        createInfo.hwnd = hwnd;
        vkCreateWin32SurfaceKHR(vkbInstance.instance, &createInfo, nullptr, &_vkSurface);*/

    vkb::PhysicalDeviceSelector deviceSelector(vkbInstance);

    VkPhysicalDeviceVulkan13Features features = {};
    features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    features.dynamicRendering = VK_TRUE;
    auto physDevWrapper = deviceSelector
                              .set_surface(m_vkSurface)
                              .set_minimum_version(1, 3)
                              .add_required_extension(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME)
                              .add_required_extension(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME)
                              .add_required_extension(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)
                              .add_required_extension_features(features)
                              .require_dedicated_transfer_queue()
                              .select();
    assert(physDevWrapper);
    vkb::DeviceBuilder deviceBuilder(physDevWrapper.value());
    auto devWrapper = deviceBuilder.build();
    assert(devWrapper);
    vkb::Device vkbDevice = devWrapper.value();

    VkDevice device = vkbDevice.device;
    auto graphicsQueueWrapper = vkbDevice.get_queue(vkb::QueueType::graphics);
    assert(graphicsQueueWrapper);
    VkQueue graphicsQueue = graphicsQueueWrapper.value();

    vkb::SwapchainBuilder swapchainBuilder(vkbDevice);
    auto swapchainResult = swapchainBuilder.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR).build();
    assert(swapchainResult);

    m_vkDevice = device;
    m_vkQueue = graphicsQueue;
    m_vkInstance = vkbInstance.instance;
    m_vkSwapchain = swapchainResult.value();

    VmaAllocatorCreateInfo allocatorCreateInfo = {};
    allocatorCreateInfo.device = m_vkDevice;
    allocatorCreateInfo.instance = m_vkInstance;
    allocatorCreateInfo.physicalDevice = physDevWrapper.value().physical_device;
    vmaCreateAllocator(&allocatorCreateInfo, &m_vmaAllocator);

    VkCommandPoolCreateInfo cmdPoolCreateInfo;
    cmdPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    cmdPoolCreateInfo.pNext = 0;
    cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolCreateInfo.queueFamilyIndex = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();
    assert(vkCreateCommandPool(m_vkDevice, &cmdPoolCreateInfo, nullptr, &m_CommandPool) == VK_SUCCESS);

    VkCommandBufferAllocateInfo cmdBufferAllocInfo;
    cmdBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBufferAllocInfo.pNext = 0;
    cmdBufferAllocInfo.commandPool = m_CommandPool;
    cmdBufferAllocInfo.commandBufferCount = 1;
    assert(vkAllocateCommandBuffers(m_vkDevice, &cmdBufferAllocInfo, &m_CommandBuffer) == VK_SUCCESS);

    vkGetSwapchainImagesKHR(m_vkDevice, m_vkSwapchain, &m_SwapChainImageCount, VK_NULL_HANDLE);
    if (m_SwapChainImageCount)
    {
	VkImageViewUsageCreateInfo usageCreateInfo = {VK_STRUCTURE_TYPE_IMAGE_VIEW_USAGE_CREATE_INFO, VK_NULL_HANDLE, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT};
	m_Images = new VkImage[m_SwapChainImageCount];
	vkGetSwapchainImagesKHR(m_vkDevice, m_vkSwapchain, &m_SwapChainImageCount, m_Images);
	m_ImageViews = new VkImageView[m_SwapChainImageCount];
	VkComponentMapping componentMapping = {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY};
	for (uint32_t i = 0; i < m_SwapChainImageCount; i++)
	{
	    VkImageViewCreateInfo createInfo = {};
	    createInfo.pNext = &usageCreateInfo;
	    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	    createInfo.image = m_Images[i];
	    createInfo.flags = 0;
	    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	    createInfo.format = VK_FORMAT_B8G8R8A8_SRGB;
	    createInfo.components = componentMapping;
	    createInfo.subresourceRange = {
	        VK_IMAGE_ASPECT_COLOR_BIT,
	        0,
	        1,
	        0,
	        1};
	    vkCreateImageView(m_vkDevice, &createInfo, nullptr, &m_ImageViews[i]);
	}
    }

    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = 0;

    vkCreateFence(m_vkDevice, &fenceCreateInfo, nullptr, &m_vkFence);
    vkCreateFence(m_vkDevice, &fenceCreateInfo, VK_NULL_HANDLE, &m_vkCmdBufFence);
    vkCmdBeginRenderingKHR = (PFN_vkCmdBeginRenderingKHR)vkGetInstanceProcAddr(m_vkInstance, "vkCmdBeginRenderingKHR");
    vkCmdEndRenderingKHR = (PFN_vkCmdEndRenderingKHR)vkGetInstanceProcAddr(m_vkInstance, "vkCmdEndRenderingKHR");

    size_t szFragShaderCode = 0, szVertShaderCode = 0;
    uint32_t *fragShaderCode = system::FileSystem::ReadBinary<uint32_t>("../../Content/GLSL/Unlit/RGBVertex.frag.spv", szFragShaderCode), *vertShaderCode = system::FileSystem::ReadBinary<uint32_t>("../../Content/GLSL/Unlit/RGBVertex.vert.spv", szVertShaderCode);

    szFragShaderCode = szFragShaderCode + (szFragShaderCode % 4);
    szVertShaderCode = szVertShaderCode + (szVertShaderCode % 4);

    VkShaderModuleCreateInfo fragShaderModuleCreateInfo = {};
    fragShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    fragShaderModuleCreateInfo.codeSize = szFragShaderCode;
    fragShaderModuleCreateInfo.pCode = fragShaderCode;
    vkCreateShaderModule(m_vkDevice, &fragShaderModuleCreateInfo, VK_NULL_HANDLE, &m_vkFragShader);
    VkShaderModuleCreateInfo vertShaderModuleCreateInfo = {};
    vertShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vertShaderModuleCreateInfo.codeSize = szVertShaderCode;
    vertShaderModuleCreateInfo.pCode = vertShaderCode;
    vkCreateShaderModule(m_vkDevice, &vertShaderModuleCreateInfo, VK_NULL_HANDLE, &m_vkVertShader);

    VkPipelineShaderStageCreateInfo pipelineStages[2] = {};
    pipelineStages[0] = {};
    pipelineStages[1] = {};
    pipelineStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    pipelineStages[0].module = m_vkVertShader;
    pipelineStages[0].pName = "main";

    pipelineStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    pipelineStages[1].module = m_vkFragShader;
    pipelineStages[1].pName = "main";

    pipelineStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    pipelineStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;

    // pipelineStages[1].

    VkFormat colourFormat = VK_FORMAT_B8G8R8A8_SRGB;
    VkPipelineRenderingCreateInfoKHR pipelineRenderingCreateInfo = {};
    pipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
    pipelineRenderingCreateInfo.colorAttachmentCount = 1;
    pipelineRenderingCreateInfo.pColorAttachmentFormats = &colourFormat;
    pipelineRenderingCreateInfo.depthAttachmentFormat = VK_FORMAT_D32_SFLOAT;

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stageCount = 2;
    pipelineCreateInfo.pStages = pipelineStages;
    pipelineCreateInfo.renderPass = VK_NULL_HANDLE; // dynamic rendering
    VkVertexInputAttributeDescription vertexAttrib = {
        0,
        0};
    vertexAttrib.format = (VkFormat)(VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT | VK_FORMAT_R32G32B32_SFLOAT);
    vertexAttrib.offset = 0;
    VkVertexInputBindingDescription vertexBinding = {
        0,
        sizeof(float) * 3,
        VK_VERTEX_INPUT_RATE_VERTEX};
    VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        VK_NULL_HANDLE,
        0,
        1,
        &vertexBinding,
        1,
        &vertexAttrib};
    pipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        VK_NULL_HANDLE,
        0,
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        false};
    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
    VkPipelineTessellationStateCreateInfo tessStateCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
        VK_NULL_HANDLE,
        0,
        1};
    pipelineCreateInfo.pTessellationState = &tessStateCreateInfo;
    int width = 1, height = 1;
    glfwGetWindowSize(window, &width, &height);
    VkViewport viewport = {
        0,
        0,
        static_cast<float>(width) * 1.f,
        static_cast<float>(height) * 1.f,
        0.f,
        1.f};
    VkRect2D scissor = {};
    scissor.offset = {0, 0};
    scissor.extent = {(uint32_t)width, (uint32_t)height};
    VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        VK_NULL_HANDLE,
        0,
        1,
        &viewport,
        1,
        &scissor};
    pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
    VkPipelineMultisampleStateCreateInfo msStateCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        VK_NULL_HANDLE,
        0,
        VK_SAMPLE_COUNT_1_BIT,
        false,
        1.f,
        VK_NULL_HANDLE,
        false,
        false};
    pipelineCreateInfo.pMultisampleState = &msStateCreateInfo;
    VkPipelineRasterizationStateCreateInfo rasterStateCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        VK_NULL_HANDLE,
        0,
        false,
        false,
        VK_POLYGON_MODE_FILL,
        VK_CULL_MODE_FRONT_BIT,
        VK_FRONT_FACE_CLOCKWISE,
        false,
        0,
        0,
        0,
        1};
    pipelineCreateInfo.pRasterizationState = &rasterStateCreateInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {
        false,
    };
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_A_BIT;
    VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        VK_NULL_HANDLE,
        0,
        // true,
        false,
        VK_LOGIC_OP_OR,
        1,
        &colorBlendAttachment,
        {1.f, 1.f, 1.f, 1.f}};
    pipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        VK_NULL_HANDLE,
        0,
        true,
        true,
        VK_COMPARE_OP_LESS,
        false,
        false};
    pipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;
    // VkDescriptorSetLayoutBinding setLayoutBinding = {
    //     0,
    //     VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
    //     1,
    //     VK_SHADER_STAGE_ALL,
    //
    // } VkDescriptorSetLayoutCreateInfo setLayoutCreateInfo = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO, VK_NULL_HANDLE, 0, 1, &setLayoutBinding};
    VkPushConstantRange pushConstantRange = {
        VK_SHADER_STAGE_ALL,
        0,
        sizeof(float) * 4 * 4 * 3};
    VkPipelineLayoutCreateInfo layoutCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        VK_NULL_HANDLE,
        0,
        0,
        VK_NULL_HANDLE,
        1,
        &pushConstantRange};
    vkCreatePipelineLayout(m_vkDevice, &layoutCreateInfo, nullptr, &m_vkGraphicsPipelineLayout);
    pipelineCreateInfo.layout = m_vkGraphicsPipelineLayout;
    pipelineCreateInfo.pNext = &pipelineRenderingCreateInfo;

    vkCreateGraphicsPipelines(m_vkDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, VK_NULL_HANDLE, &m_vkGraphicsPipeline);

    m_vkColourAttachmentInfo = {};
    m_vkColourAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    m_vkColourAttachmentInfo.clearValue = {};
    m_vkColourAttachmentInfo.clearValue.depthStencil = {};
    m_vkColourAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    m_vkColourAttachmentInfo.resolveMode = VK_RESOLVE_MODE_NONE;
    m_vkColourAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    m_vkColourAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

    m_vkRenderingInfo.renderArea = {};
    m_vkRenderingInfo.renderArea.offset = {0, 0};
    m_vkRenderingInfo.renderArea.extent = {(uint32_t)width, (uint32_t)height};

    m_vkColourAttachmentInfo.clearValue.color = {{0.f, 0.f, 0.f, 0.f}};

    m_vkDepthAttachmentInfo = {};
    m_vkDepthAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    m_vkDepthAttachmentInfo.clearValue = {};
    m_vkDepthAttachmentInfo.clearValue.color = {{0.f, 0.f, 0.f, 0.f}};
    m_vkDepthAttachmentInfo.clearValue.depthStencil = {1.f, 1};
    m_vkDepthAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    m_vkDepthAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    m_vkDepthAttachmentInfo.resolveMode = VK_RESOLVE_MODE_NONE;
    m_vkDepthAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkImageCreateInfo depthBufferCreateInfo = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, nullptr};
    depthBufferCreateInfo.format = VK_FORMAT_D32_SFLOAT;
    depthBufferCreateInfo.extent = {(uint32_t)width, (uint32_t)height, 1};
    depthBufferCreateInfo.flags = 0;
    depthBufferCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    depthBufferCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    depthBufferCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    depthBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    depthBufferCreateInfo.arrayLayers = 1;
    depthBufferCreateInfo.mipLevels = 1;
    depthBufferCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    depthBufferCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthBufferCreateInfo.queueFamilyIndexCount = 1;
    uint32_t queueFamilyIndex = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();
    depthBufferCreateInfo.pQueueFamilyIndices = &queueFamilyIndex;
    VmaAllocationCreateInfo depthBufAllocCreateInfo = {};
    depthBufAllocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
    vmaCreateImage(m_vmaAllocator, &depthBufferCreateInfo, &depthBufAllocCreateInfo, &m_vkDepthBuffer, &m_vmaDepthBufAllocation, VMA_NULL);
    // vkCreateImage(m_vkDevice, &depthBufferCreateInfo, VK_NULL_HANDLE, &m_vkDepthBuffer);

    VkImageViewCreateInfo depthBufferViewCreateInfo = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, VK_NULL_HANDLE, 0};
    depthBufferViewCreateInfo.components = {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY};
    depthBufferViewCreateInfo.format = VK_FORMAT_D32_SFLOAT;
    depthBufferViewCreateInfo.image = m_vkDepthBuffer;
    depthBufferViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    depthBufferViewCreateInfo.subresourceRange = {
        VK_IMAGE_ASPECT_DEPTH_BIT,
        0,
        1,
        0,
        1};

    vkCreateImageView(m_vkDevice, &depthBufferViewCreateInfo, nullptr, &m_vkDepthBufferView);
}

size_t GraphicsApiVk::_EnsureVertBufferCapacity(size_t newCount)
{
    if (m_vkVertBufferCount < newCount)
    {
	const VkBuffer* temp = m_vkVertBuffers;

	m_vkVertBuffers = new VkBuffer[newCount];

	memset((void*)m_vkVertBuffers, 0, newCount * sizeof(VkBuffer));
	memcpy((void*)m_vkVertBuffers, temp, m_vkVertBufferCount * sizeof(VkBuffer));
	m_vkVertBufferCount = newCount;

	delete[] temp;
    }

    return m_vkVertBufferCount;
}

size_t GraphicsApiVk::_EnsureIndexBufferCapacity(size_t newCount)
{
    if (m_vkIndexBufferCount < newCount)
    {
	const VkBuffer* temp = m_vkIndexBuffers;

	m_vkIndexBuffers = new VkBuffer[newCount];

	memset((void*)m_vkIndexBuffers, 0, newCount * sizeof(VkBuffer));
	memcpy((void*)m_vkIndexBuffers, temp, m_vkIndexBufferCount * sizeof(VkBuffer));
	m_vkIndexBufferCount = newCount;

	delete[] temp;
    }

    return m_vkIndexBufferCount;
}

VkBuffer GraphicsApiVk::CreateBuffer(GraphicsApiVk::BufferType type, const void* vertexOrIndexData, uint64_t vertexCount)
{
    size_t bufferIndex = 0;
    VkBufferCreateInfo bufferCreateInfo = {
        VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        VK_NULL_HANDLE,
        0,
        vertexCount,
        VK_BUFFER_USAGE_FLAG_BITS_MAX_ENUM,
        VK_SHARING_MODE_EXCLUSIVE,
        0,
        VK_NULL_HANDLE};

    VmaAllocationCreateInfo allocCreateInfo = {};
    allocCreateInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    VmaAllocationInfo allocInfo = {};

    const VkBuffer* buffers = nullptr;

    switch (type)
    {
    case IndexBuffer:
	buffers = m_vkIndexBuffers;
	bufferIndex = m_vkIndexBufferCount;
	_EnsureIndexBufferCapacity(bufferIndex + 1);

	bufferCreateInfo.size *= sizeof(uint64_t);
	bufferCreateInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	break;
    case VertexBuffer:
	buffers = m_vkVertBuffers;
	bufferIndex = m_vkVertBufferCount;
	_EnsureVertBufferCapacity(bufferIndex + 1);

	bufferCreateInfo.size *= sizeof(float) * 3;
	bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	break;
    default:
	assert(false);
	break;
    }

    assert(buffers);
    vmaCreateBuffer(m_vmaAllocator, &bufferCreateInfo, &allocCreateInfo, (VkBuffer*)&buffers[bufferIndex], &m_vmaAllocation, &allocInfo);
    m_vkMemory = m_vmaAllocation->GetMemory();
    void* data;
    vkMapMemory(m_vkDevice, m_vkMemory, allocInfo.offset, allocInfo.size, 0, &data);
    memcpy(data, vertexOrIndexData, allocInfo.size);
    vkUnmapMemory(m_vkDevice, m_vkMemory);
    return buffers[bufferIndex];
}

lepus::engine::objects::Mesh* GraphicsApiVk::WrapMesh(engine::objects::Mesh* mesh)
{
    // TODO: create IBO, VBO etc
    return new lepus::gfx::VkMesh((float*)mesh->GetVertices(), mesh->VertexCount(), mesh->GetIndices(), (uint32_t)mesh->IndexCount(), *this);
}

void GraphicsApiVk::ClearFrameBuffer(float r, float g, float b)
{
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.pNext = 0;
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = 0;

    vkResetFences(m_vkDevice, 1, &m_vkFence);
    vkResetFences(m_vkDevice, 1, &m_vkCmdBufFence);
    vkBeginCommandBuffer(m_CommandBuffer, &beginInfo);

    vkAcquireNextImageKHR(m_vkDevice, m_vkSwapchain, UINT64_MAX, nullptr, m_vkFence, &m_CurrentImageIndex);
    vkWaitForFences(m_vkDevice, 1, &m_vkFence, VK_TRUE, UINT64_MAX);

    const float gamma = 2.2f;
    m_vkColourAttachmentInfo.clearValue.color.float32[0] = powf(r, gamma);
    m_vkColourAttachmentInfo.clearValue.color.float32[1] = powf(g, gamma);
    m_vkColourAttachmentInfo.clearValue.color.float32[2] = powf(b, gamma);
    m_vkColourAttachmentInfo.clearValue.color.float32[3] = 0.f;

    m_vkDepthAttachmentInfo.clearValue.depthStencil.depth = 1.0f;
    m_vkDepthAttachmentInfo.clearValue.depthStencil.stencil = 1;

    // m_vkColourAttachmentInfo.clearValue.color = colour;
    m_vkColourAttachmentInfo.imageView = m_ImageViews[m_CurrentImageIndex];
    m_vkDepthAttachmentInfo.imageView = m_vkDepthBufferView;

    m_vkRenderingInfo.pNext = VK_NULL_HANDLE;
    m_vkRenderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
    m_vkRenderingInfo.layerCount = 1;
    m_vkRenderingInfo.viewMask = 0;
    m_vkRenderingInfo.colorAttachmentCount = 1;
    m_vkRenderingInfo.pColorAttachments = &m_vkColourAttachmentInfo;
    m_vkRenderingInfo.pDepthAttachment = &m_vkDepthAttachmentInfo;

    VkImageMemoryBarrier imgMemBarrier = {
        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        VK_NULL_HANDLE,
        0,
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        0,
        0,
        m_Images[m_CurrentImageIndex],
        {VK_IMAGE_ASPECT_COLOR_BIT,
         0,
         1,
         0,
         1}};

    vkCmdPipelineBarrier(m_CommandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &imgMemBarrier);

    // vkCmdBindPipeline(m_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VK_NULL_HANDLE);
    // vkCmdBindVertexBuffers(m_CommandBuffer, 0, 0, VK_NULL_HANDLE, VK_NULL_HANDLE);

    // delete[] matrixData;
}

void GraphicsApiVk::UpdateUniforms(const SceneGraph& scene)
{
    lepus::math::Matrix4x4 model = lepus::math::Matrix4x4::Identity();
    auto camera = scene.Camera();

    vkCmdBindPipeline(m_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_vkGraphicsPipeline);
    auto proj = camera->BuildPerspectiveMatrix();
    float aspectRatio = (1.f * (float)m_vkRenderingInfo.renderArea.extent.width) / (1.f * (float)m_vkRenderingInfo.renderArea.extent.height);
    proj.set<0, 0>(proj.get(0, 0) / aspectRatio);
    vkCmdPushConstants(m_CommandBuffer, m_vkGraphicsPipelineLayout, VK_SHADER_STAGE_ALL, 0, sizeof(float) * 4 * 4, proj.data());
    vkCmdPushConstants(m_CommandBuffer, m_vkGraphicsPipelineLayout, VK_SHADER_STAGE_ALL, sizeof(float) * 4 * 4, sizeof(float) * 4 * 4, camera->BuildViewMatrix().data());
}

void GraphicsApiVk::StartDrawing()
{

    // vkCmdUpdateBuffer(m_CommandBuffer, m_vkVertBuffer, 0, sizeof(float) * 3 * 3, verts);
    vkCmdBeginRenderingKHR(m_CommandBuffer, &m_vkRenderingInfo);
}

void GraphicsApiVk::Draw(const SceneGraph& scene)
{
    auto currentNode = scene.Root();

    bool branchComplete = false;

    while (currentNode)
    {
	if (!branchComplete && !currentNode->IsRoot())
	{
	    const lepus::gfx::Renderable<VkMesh>* renderable = (const lepus::gfx::Renderable<VkMesh>*)(currentNode->GetTransformable());
	    vkCmdPushConstants(m_CommandBuffer, m_vkGraphicsPipelineLayout, VK_SHADER_STAGE_ALL, 2 * (sizeof(float) * 4 * 4), sizeof(float) * 4 * 4, renderable->GetWorldMatrix(currentNode).data());
	    size_t offsets = 0;

	    const VkBuffer& vertBuffer = renderable->GetMesh()->GetVkVertBuffer();
	    vkCmdBindVertexBuffers(m_CommandBuffer, 0, 1, &vertBuffer, &offsets);
	    if (renderable->GetMesh()->IndexCount() > 0)
	    {
		vkCmdBindIndexBuffer(m_CommandBuffer, renderable->GetMesh()->GetVkIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(m_CommandBuffer, (uint32_t)renderable->GetMesh()->IndexCount(), 1, 0, 0, 0);
	    }
	    else
	    {
		vkCmdDraw(m_CommandBuffer, renderable->GetMesh()->GetVertCount(), 1, 0, 0);
	    }
	}

	if (!branchComplete && currentNode->FirstChild())
	{
	    currentNode = currentNode->FirstChild();
	}
	else if (currentNode->NextSibling())
	{
	    currentNode = currentNode->NextSibling();
	    branchComplete = false;
	}
	else
	{
	    branchComplete = true;
	    currentNode = currentNode->Parent();
	}
    }
}

void GraphicsApiVk::EndDrawing()
{
    vkCmdEndRenderingKHR(m_CommandBuffer);
    VkImageMemoryBarrier imgMemBarrier = {
        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        VK_NULL_HANDLE,
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        VK_ACCESS_NONE,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        0,
        0,
        m_Images[m_CurrentImageIndex],
        {VK_IMAGE_ASPECT_COLOR_BIT,
         0,
         1,
         0,
         1}};

    vkCmdPipelineBarrier(m_CommandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &imgMemBarrier);
    vkEndCommandBuffer(m_CommandBuffer);

    // assert(acquireResult == VK_SUCCESS);

    VkSubmitInfo submitInfo;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = 0;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_CommandBuffer;
    submitInfo.pSignalSemaphores = 0;
    submitInfo.pWaitSemaphores = 0;
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitDstStageMask = 0;
    vkQueueSubmit(m_vkQueue, 1, &submitInfo, m_vkCmdBufFence);
    vkWaitForFences(m_vkDevice, 1, &m_vkCmdBufFence, VK_TRUE, UINT64_MAX);
}

void GraphicsApiVk::SwapBuffers()
{
    VkPresentInfoKHR presentInfo;

    presentInfo.pNext = 0;
    presentInfo.pSwapchains = &m_vkSwapchain;
    presentInfo.pImageIndices = &m_CurrentImageIndex;
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pResults = nullptr;
    presentInfo.swapchainCount = 1;
    presentInfo.pWaitSemaphores = nullptr;
    presentInfo.waitSemaphoreCount = 0;

    vkQueuePresentKHR(m_vkQueue, &presentInfo);
}

void GraphicsApiVk::Shutdown()
{
    vkDestroyFence(m_vkDevice, m_vkFence, VK_NULL_HANDLE);
    for (uint32_t i = 0; i < m_SwapChainImageCount; i++)
    {
	vkDestroyImageView(m_vkDevice, m_ImageViews[i], VK_NULL_HANDLE);
    }

    vkDestroyCommandPool(m_vkDevice, m_CommandPool, nullptr);
    vkDestroySwapchainKHR(m_vkDevice, m_vkSwapchain, nullptr);
    vkDestroyPipelineLayout(m_vkDevice, m_vkGraphicsPipelineLayout, nullptr);
    vkDestroyPipeline(m_vkDevice, m_vkGraphicsPipeline, nullptr);
    vmaDestroyImage(m_vmaAllocator, m_vkDepthBuffer, m_vmaDepthBufAllocation);
    for (size_t i = 0; i < m_vkVertBufferCount; i++)
    {
	if (m_vkVertBuffers[i] != VK_NULL_HANDLE)
	{
	    vmaDestroyBuffer(m_vmaAllocator, m_vkVertBuffers[i], m_vmaAllocation);
	}
    }
    delete[] m_vkVertBuffers;
    vmaDestroyAllocator(m_vmaAllocator);
    vkDestroyDevice(m_vkDevice, nullptr);
    vkDestroySurfaceKHR(m_vkInstance, m_vkSurface, nullptr);
}
