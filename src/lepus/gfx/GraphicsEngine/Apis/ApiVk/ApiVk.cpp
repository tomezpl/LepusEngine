// #include <Windows.h>

#include "lepus/system/IO/FileSystem.h"
#include "lepus/utility/types/Matrix4x4.h"

#define VMA_IMPLEMENTATION
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
        VK_CULL_MODE_NONE,
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

    VkBufferCreateInfo vertBufferCreateInfo = {
        VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        VK_NULL_HANDLE,
        0,
        sizeof(float) * 3 * 3,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        0,
        VK_NULL_HANDLE};

    VmaAllocationCreateInfo allocCreateInfo = {};
    allocCreateInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
    vmaCreateBuffer(m_vmaAllocator, &vertBufferCreateInfo, &allocCreateInfo, &m_vkVertBuffer, &m_vmaAllocation, VK_NULL_HANDLE);
    m_vkMemory = m_vmaAllocation->GetMemory();
    float verts[3 * 3] = {
        -0.75f, 0.75f, 0.5f,
        0.75f, 0.75f, 0.5f,
        0.f, -0.75f, 0.5f};
    void* data;
    vkMapMemory(m_vkDevice, m_vkMemory, 0, sizeof(float) * 3 * 3, 0, &data);
    memcpy(data, verts, sizeof(float) * 3 * 3);

    vkUnmapMemory(m_vkDevice, m_vkMemory);

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
}

lepus::engine::objects::Mesh* GraphicsApiVk::WrapMesh(engine::objects::Mesh* mesh)
{
    // TODO: create IBO, VBO etc
    return mesh;
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

    VkImageSubresourceRange ranges;
    ranges.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    ranges.layerCount = 1;
    ranges.levelCount = 1;
    ranges.baseArrayLayer = 0;
    ranges.baseMipLevel = 0;
    VkClearColorValue colour = {};
    const float gamma = 2.2f;
    m_vkColourAttachmentInfo.clearValue.color.float32[0] = powf(r, gamma);
    m_vkColourAttachmentInfo.clearValue.color.float32[1] = powf(g, gamma);
    m_vkColourAttachmentInfo.clearValue.color.float32[2] = powf(b, gamma);
    m_vkColourAttachmentInfo.clearValue.color.float32[3] = 0.f;

    // m_vkColourAttachmentInfo.clearValue.color = colour;
    m_vkColourAttachmentInfo.imageView = m_ImageViews[m_CurrentImageIndex];

    m_vkRenderingInfo.pNext = VK_NULL_HANDLE;
    m_vkRenderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
    m_vkRenderingInfo.layerCount = 1;
    m_vkRenderingInfo.viewMask = 0;
    m_vkRenderingInfo.colorAttachmentCount = 1;
    m_vkRenderingInfo.pColorAttachments = &m_vkColourAttachmentInfo;

    VkImageMemoryBarrier imgMemBarrier = {
        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        VK_NULL_HANDLE,
        VK_ACCESS_NONE,
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
    lepus::math::Matrix4x4 proj = lepus::math::Matrix4x4::Identity(), view = lepus::math::Matrix4x4::Identity(), model = lepus::math::Matrix4x4::Identity();

    vkCmdBindPipeline(m_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_vkGraphicsPipeline);
    vkCmdPushConstants(m_CommandBuffer, m_vkGraphicsPipelineLayout, VK_SHADER_STAGE_ALL, 0, sizeof(float) * 4 * 4, proj.data());
    vkCmdPushConstants(m_CommandBuffer, m_vkGraphicsPipelineLayout, VK_SHADER_STAGE_ALL, sizeof(float) * 4 * 4, sizeof(float) * 4 * 4, view.data());
    vkCmdPushConstants(m_CommandBuffer, m_vkGraphicsPipelineLayout, VK_SHADER_STAGE_ALL, 2 * (sizeof(float) * 4 * 4), sizeof(float) * 4 * 4, model.data());
    size_t offsets = 0;
    vkCmdBindVertexBuffers(m_CommandBuffer, 0, 1, &m_vkVertBuffer, &offsets);
}

void GraphicsApiVk::StartDrawing()
{

    // vkCmdUpdateBuffer(m_CommandBuffer, m_vkVertBuffer, 0, sizeof(float) * 3 * 3, verts);
    vkCmdBeginRenderingKHR(m_CommandBuffer, &m_vkRenderingInfo);
}

void GraphicsApiVk::Draw(const SceneGraph& scene)
{
    vkCmdDraw(m_CommandBuffer, 3, 1, 0, 0);
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
    vmaDestroyBuffer(m_vmaAllocator, m_vkVertBuffer, m_vmaAllocation);
    vmaDestroyAllocator(m_vmaAllocator);
    vkDestroyDevice(m_vkDevice, nullptr);
    vkDestroySurfaceKHR(m_vkInstance, m_vkSurface, nullptr);
}
