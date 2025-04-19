// #include <Windows.h>

#include "lepus/system/IO/FileSystem.h"
#include "lepus/utility/types/Matrix4x4.h"

#define VMA_IMPLEMENTATION
#include "Types/VkBufferAlloc.h"
#include "Types/VkMesh.h"
#include "Types/VkShader.h"
#include "lepus/gfx/GraphicsEngine/ShaderCompilers/ShaderCompilerVk.h"

#include <iostream>
#include <lepus/gfx/GraphicsEngine/Apis/ApiVk.h>

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
    ShaderCompilerVk::Singleton().m_vkDevice = device;
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

    m_Defaults.colourFormat = VK_FORMAT_B8G8R8A8_SRGB;
    m_Defaults.pipelineRenderingCreateInfo = {};
    m_Defaults.pipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
    m_Defaults.pipelineRenderingCreateInfo.colorAttachmentCount = 1;
    m_Defaults.pipelineRenderingCreateInfo.pColorAttachmentFormats = &m_Defaults.colourFormat;
    m_Defaults.pipelineRenderingCreateInfo.depthAttachmentFormat = VK_FORMAT_D32_SFLOAT;

    m_Defaults.pipelineCreateInfo = {};
    m_Defaults.pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    m_Defaults.pipelineCreateInfo.stageCount = 2;
    m_Defaults.pipelineCreateInfo.pStages = nullptr;
    m_Defaults.pipelineCreateInfo.renderPass = VK_NULL_HANDLE; // dynamic rendering

    m_Defaults.vertexAttributes = {
        0,
        0};
    m_Defaults.vertexAttributes.format = (VkFormat)(VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT | VK_FORMAT_R32G32B32_SFLOAT);
    m_Defaults.vertexAttributes.offset = 0;

    m_Defaults.vertexBinding = {
        0,
        sizeof(float) * 3,
        VK_VERTEX_INPUT_RATE_VERTEX};
    m_Defaults.vertexInputStateCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        VK_NULL_HANDLE,
        0,
        1,
        &m_Defaults.vertexBinding,
        1,
        &m_Defaults.vertexAttributes};
    m_Defaults.pipelineCreateInfo.pVertexInputState = &m_Defaults.vertexInputStateCreateInfo;
    m_Defaults.inputAssemblyStateCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        VK_NULL_HANDLE,
        0,
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        false};
    m_Defaults.pipelineCreateInfo.pInputAssemblyState = &m_Defaults.inputAssemblyStateCreateInfo;
    m_Defaults.tessStateCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
        VK_NULL_HANDLE,
        0,
        1};
    m_Defaults.pipelineCreateInfo.pTessellationState = &m_Defaults.tessStateCreateInfo;
    int width = 1, height = 1;
    glfwGetWindowSize(window, &width, &height);
    m_Defaults.viewport = {
        0,
        0,
        static_cast<float>(width) * 1.f,
        static_cast<float>(height) * 1.f,
        0.f,
        1.f};
    m_Defaults.scissor = {};
    m_Defaults.scissor.offset = {0, 0};
    m_Defaults.scissor.extent = {(uint32_t)width, (uint32_t)height};
    m_Defaults.viewportStateCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        VK_NULL_HANDLE,
        0,
        1,
        &m_Defaults.viewport,
        1,
        &m_Defaults.scissor};
    m_Defaults.pipelineCreateInfo.pViewportState = &m_Defaults.viewportStateCreateInfo;
    m_Defaults.msStateCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        VK_NULL_HANDLE,
        0,
        VK_SAMPLE_COUNT_1_BIT,
        false,
        1.f,
        VK_NULL_HANDLE,
        false,
        false};
    m_Defaults.pipelineCreateInfo.pMultisampleState = &m_Defaults.msStateCreateInfo;
    m_Defaults.rasterStateCreateInfo = {
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
    m_Defaults.pipelineCreateInfo.pRasterizationState = &m_Defaults.rasterStateCreateInfo;
    m_Defaults.colorBlendAttachment = {
        false,
    };
    m_Defaults.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_A_BIT;
    m_Defaults.colorBlendStateCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        VK_NULL_HANDLE,
        0,
        // true,
        false,
        VK_LOGIC_OP_OR,
        1,
        &m_Defaults.colorBlendAttachment,
        {1.f, 1.f, 1.f, 1.f}};
    m_Defaults.pipelineCreateInfo.pColorBlendState = &m_Defaults.colorBlendStateCreateInfo;
    m_Defaults.depthStencilStateCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        VK_NULL_HANDLE,
        0,
        true,
        true,
        VK_COMPARE_OP_LESS,
        false,
        false};
    m_Defaults.pipelineCreateInfo.pDepthStencilState = &m_Defaults.depthStencilStateCreateInfo;

    m_Defaults.pushConstantRange = {
        VK_SHADER_STAGE_ALL,
        0,
        sizeof(float) * 4 * 4 * 3};
    m_Defaults.pipelineLayoutCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        VK_NULL_HANDLE,
        0,
        0,
        VK_NULL_HANDLE,
        1,
        &m_Defaults.pushConstantRange};
    vkCreatePipelineLayout(m_vkDevice, &m_Defaults.pipelineLayoutCreateInfo, nullptr, &m_Defaults.pipelineLayout);
    m_Defaults.pipelineCreateInfo.layout = m_Defaults.pipelineLayout;
    m_Defaults.pipelineCreateInfo.pNext = &m_Defaults.pipelineRenderingCreateInfo;

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

VkBuffer GraphicsApiVk::CreateBuffer(GraphicsApiVk::BufferType type, const void* vertexOrIndexData, uint64_t vertexCount)
{
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

    VkBuffer bufferHandle = VK_NULL_HANDLE;
    utility::List<VkBufferAlloc>* buffers = nullptr;

    switch (type)
    {
    case IndexBuffer:
	bufferCreateInfo.size *= sizeof(uint64_t);
	buffers = &m_vkIndexBuffers;
	bufferCreateInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	break;
    case VertexBuffer:
	buffers = &m_vkVertBuffers;
	bufferCreateInfo.size *= sizeof(float) * 3;
	bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	break;
    default:
	assert(false);
	break;
    }

    assert(buffers);
    VmaAllocation allocation = nullptr;
    vmaCreateBuffer(m_vmaAllocator, &bufferCreateInfo, &allocCreateInfo, &bufferHandle, &allocation, &allocInfo);
    buffers->Push({bufferHandle, allocation});
    m_vkMemory = allocation->GetMemory();

    // Get pointer to the mapped memory and upload the vertex/index data
    void* data;
    vkMapMemory(m_vkDevice, m_vkMemory, allocInfo.offset, allocInfo.size, 0, &data);
    memcpy(data, vertexOrIndexData, allocInfo.size);
    vkUnmapMemory(m_vkDevice, m_vkMemory);

    return bufferHandle;
}

lepus::engine::objects::Mesh* GraphicsApiVk::WrapMesh(engine::objects::Mesh* mesh)
{
    return new lepus::gfx::VkMesh((float*)mesh->GetVertices(), mesh->VertexCount(), mesh->GetIndices(), (uint32_t)mesh->IndexCount(), *this);
}

void GraphicsApiVk::ClearFrameBuffer(float r, float g, float b)
{
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.pNext = VK_NULL_HANDLE;
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = VK_NULL_HANDLE;

    vkResetFences(m_vkDevice, 1, &m_vkFence);
    vkResetFences(m_vkDevice, 1, &m_vkCmdBufFence);
    vkBeginCommandBuffer(m_CommandBuffer, &beginInfo);

    vkAcquireNextImageKHR(m_vkDevice, m_vkSwapchain, UINT64_MAX, nullptr, m_vkFence, &m_CurrentImageIndex);
    vkWaitForFences(m_vkDevice, 1, &m_vkFence, VK_TRUE, UINT64_MAX);

    // Map the clear colour from linear RGB to sRGB
    const float gamma = 2.2f;
    m_vkColourAttachmentInfo.clearValue.color.float32[0] = powf(r, gamma);
    m_vkColourAttachmentInfo.clearValue.color.float32[1] = powf(g, gamma);
    m_vkColourAttachmentInfo.clearValue.color.float32[2] = powf(b, gamma);
    m_vkColourAttachmentInfo.clearValue.color.float32[3] = 0.f;

    m_vkDepthAttachmentInfo.clearValue.depthStencil.depth = 1.0f;
    m_vkDepthAttachmentInfo.clearValue.depthStencil.stencil = 1;

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
}

void GraphicsApiVk::UpdateUniforms(const SceneGraph& scene)
{
    auto camera = scene.Camera();

    // TODO(perf): cache the projection matrix and only update if invalidated
    auto proj = camera->BuildPerspectiveMatrix();
    float aspectRatio = (1.f * (float)m_vkRenderingInfo.renderArea.extent.width) / (1.f * (float)m_vkRenderingInfo.renderArea.extent.height);
    proj.set<0, 0>(proj.get(0, 0) / aspectRatio);

    auto* view = camera->BuildViewMatrix().data();

    // Update global uniforms (view & projection matrix, lights etc.) in all pipelines
    for (size_t i = 0; i < m_GraphicsPipelineCount; i++)
    {
	vkCmdBindPipeline(m_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_vkGraphicsPipelines.Get(i));
	auto pipelineLayout = m_vkGraphicsPipelineLayouts.Get(i);
	vkCmdPushConstants(m_CommandBuffer, pipelineLayout, VK_SHADER_STAGE_ALL, 0, sizeof(float) * 4 * 4, proj.data());
	vkCmdPushConstants(m_CommandBuffer, pipelineLayout, VK_SHADER_STAGE_ALL, sizeof(float) * 4 * 4, sizeof(float) * 4 * 4, view);
    }
}

void GraphicsApiVk::StartDrawing()
{
    vkCmdBeginRenderingKHR(m_CommandBuffer, &m_vkRenderingInfo);
}

void GraphicsApiVk::Draw(const SceneGraph& scene)
{
    // TODO(refactor): this scene traversal code is almost exactly the same as in GraphicsApiGL. Move it out to an iterator instead?
    auto currentNode = scene.Root();

    bool branchComplete = false;

    while (currentNode)
    {
	if (!branchComplete && !currentNode->IsRoot())
	{
	    auto* renderable = (Renderable<VkMesh>*)(currentNode->GetTransformable());
	    auto pipelineIndex = this->findPipelineIndex(renderable->GetMaterial()->GetShader<VkShader>());
	    assert(pipelineIndex != SIZE_MAX);
	    vkCmdBindPipeline(m_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_vkGraphicsPipelines.Get(pipelineIndex));
	    vkCmdPushConstants(m_CommandBuffer, m_vkGraphicsPipelineLayouts.Get(pipelineIndex), VK_SHADER_STAGE_ALL, 2 * (sizeof(float) * 4 * 4), sizeof(float) * 4 * 4, renderable->GetWorldMatrix(currentNode).data());
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
    VkPresentInfoKHR presentInfo = {};

    presentInfo.pNext = VK_NULL_HANDLE;
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
    vkDestroyFence(m_vkDevice, m_vkCmdBufFence, VK_NULL_HANDLE);

    vkDestroyImageView(m_vkDevice, m_vkDepthBufferView, VK_NULL_HANDLE);
    for (uint32_t i = 0; i < m_SwapChainImageCount; i++)
    {
	vkDestroyImageView(m_vkDevice, m_ImageViews[i], VK_NULL_HANDLE);
    }

    vkDestroyCommandPool(m_vkDevice, m_CommandPool, nullptr);
    vkDestroySwapchainKHR(m_vkDevice, m_vkSwapchain, nullptr);
    for (size_t i = 0; i < m_GraphicsPipelineCount; i++)
    {
	vkDestroyPipeline(m_vkDevice, m_vkGraphicsPipelines.Get(i), nullptr);
	auto shader = m_vkGraphicsPipelineMetadata.Get(i);
	for (size_t shaderStage = 0; shaderStage < ShaderCompilerVk::ShaderStageCount(); shaderStage++)
	{
	    if (shader->m_ShaderStageCreateInfo[shaderStage].sType == VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO)
	    {
		vkDestroyShaderModule(m_vkDevice, shader->m_ShaderStageCreateInfo[shaderStage].module, nullptr);
	    }
	}
    }
    for (size_t i = 0; i < m_GraphicsPipelineCount; i++)
    {
	auto pipelineLayout = m_vkGraphicsPipelineLayouts.Get(i);
	if (pipelineLayout != m_Defaults.pipelineLayout && pipelineLayout != VK_NULL_HANDLE)
	{
	    vkDestroyPipelineLayout(m_vkDevice, pipelineLayout, nullptr);
	}
    }
    vkDestroyPipelineLayout(m_vkDevice, m_Defaults.pipelineLayout, nullptr);
    delete[] m_Images;
    delete[] m_ImageViews;
    vmaDestroyImage(m_vmaAllocator, m_vkDepthBuffer, m_vmaDepthBufAllocation);
    for (size_t i = 0; i < m_vkVertBuffers.Count(); i++)
    {
	const VkBufferAlloc& vertBufAlloc = m_vkVertBuffers.Get(i);
	if (vertBufAlloc.buffer != VK_NULL_HANDLE)
	{
	    vmaDestroyBuffer(m_vmaAllocator, vertBufAlloc.buffer, vertBufAlloc.allocation);
	}
    }
    for (size_t i = 0; i < m_vkIndexBuffers.Count(); i++)
    {
	const VkBufferAlloc& indexBufAlloc = m_vkIndexBuffers.Get(i);
	if (indexBufAlloc.buffer != VK_NULL_HANDLE)
	{
	    vmaDestroyBuffer(m_vmaAllocator, indexBufAlloc.buffer, indexBufAlloc.allocation);
	}
    }
    vmaDestroyAllocator(m_vmaAllocator);
    vkDestroyDevice(m_vkDevice, nullptr);
    vkDestroySurfaceKHR(m_vkInstance, m_vkSurface, nullptr);
    std::cout << "Shutdown complete\n";
}

static char shaderFileNameBuffer[UINT16_MAX];

const char* GraphicsApiVk::GetShaderFileName(const char* shaderName, ShaderStage stage) const
{
    assert(stage != ShaderStage::ShaderStageInvalid);

    const char vertShaderSuffix[] = ".vert.spv";
    const char fragShaderSuffix[] = ".frag.spv";

    const char* suffixPtr = nullptr;
    size_t suffixBytes = 0;
    switch (stage)
    {
    case ShaderStageVertex:
	suffixPtr = vertShaderSuffix;
	suffixBytes = sizeof(vertShaderSuffix);
	break;
    case ShaderStageFragment:
	suffixPtr = fragShaderSuffix;
	suffixBytes = sizeof(fragShaderSuffix);
	break;
    case ShaderStageInvalid:
    default:
	assert(false);
	break;
    }

    size_t shaderNameLength = strlen(shaderName);
    memcpy(shaderFileNameBuffer, shaderName, sizeof(char) * shaderNameLength);
    memcpy((shaderFileNameBuffer + shaderNameLength), suffixPtr, suffixBytes);

    return shaderFileNameBuffer;
}

void GraphicsApiVk::CreatePipeline()
{
    for (size_t i = 0; i < m_GraphicsPipelineCount; i++)
    {
	VkPipeline tempPipeline = VK_NULL_HANDLE;
	VkGraphicsPipelineCreateInfo pipelineCreateInfo = m_Defaults.pipelineCreateInfo;
	auto* shader = m_vkGraphicsPipelineMetadata.Get(i);
	pipelineCreateInfo.pStages = shader->GetApiHandle();
	// TODO: yes this is very naive, ideally we should be calling vkCreateGraphicsPipelines once to create them "in bulk"
	vkCreateGraphicsPipelines(m_vkDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, VK_NULL_HANDLE, &tempPipeline);
	m_vkGraphicsPipelineLayouts.Push(m_Defaults.pipelineLayout);
	m_vkGraphicsPipelines.Push(tempPipeline);
    }
}
