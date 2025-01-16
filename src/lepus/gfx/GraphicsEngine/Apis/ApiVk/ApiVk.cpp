// #include <Windows.h>

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
    // const float gamma = 1.f / 2.2f;
    colour.float32[0] = powf(r, gamma);
    colour.float32[1] = powf(g, gamma);
    colour.float32[2] = powf(b, gamma);
    colour.float32[3] = 0.f;
    // colour.int32[0] = 100;
    // colour.int32[1] = 149;
    // colour.int32[2] = 237;
    // colour.int32[3] = 255;
    // colour.uint32[0] = 100;
    // colour.uint32[1] = 149;
    // colour.uint32[2] = 237;
    // colour.uint32[3] = 255;
    uint32_t swapchainImageCount = 0;
    // vkGetSwapchainImagesKHR(m_vkDevice, m_vkSwapchain, &swapchainImageCount, nullptr);
    if (swapchainImageCount > 0)
    {
	VkImage* swapchainImages = new VkImage[swapchainImageCount];
	vkGetSwapchainImagesKHR(m_vkDevice, m_vkSwapchain, &swapchainImageCount, swapchainImages);

	for (uint32_t i = 0; i < swapchainImageCount; i++)
	{

	    // vkCmdClearColorImage(m_CommandBuffer, swapchainImages[i], VK_IMAGE_LAYOUT_GENERAL, &colour, 1, &ranges);
	}

	// delete[] swapchainImages;
    }

    VkRenderingAttachmentInfo colourAttachment = {};
    colourAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    colourAttachment.clearValue = {};
    colourAttachment.clearValue.depthStencil = {};
    colourAttachment.clearValue.color = colour;
    colourAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colourAttachment.resolveMode = VK_RESOLVE_MODE_NONE;
    colourAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colourAttachment.imageView = m_ImageViews[m_CurrentImageIndex];
    colourAttachment.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

    VkRenderingInfo renderingInfo = {};
    renderingInfo.pNext = VK_NULL_HANDLE;
    renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
    renderingInfo.layerCount = 1;
    renderingInfo.viewMask = 0;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments = &colourAttachment;
    VkRenderingAttachmentInfo depthAttachment = {};
    depthAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.imageView = VK_NULL_HANDLE;
    depthAttachment.resolveMode = VK_RESOLVE_MODE_NONE;
    VkRenderingAttachmentInfo stencilAttachment = {};
    stencilAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    stencilAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    stencilAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    stencilAttachment.imageView = VK_NULL_HANDLE;
    stencilAttachment.resolveMode = VK_RESOLVE_MODE_NONE;
    // renderingInfo.pDepthAttachment = &depthAttachment;
    // renderingInfo.pStencilAttachment = &stencilAttachment;
    VkRect2D renderArea = {};
    renderArea.offset = {0, 0};
    renderArea.extent = {800, 600};
    renderingInfo.renderArea = renderArea;

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

    vkCmdBeginRenderingKHR(m_CommandBuffer, &renderingInfo);
    vkCmdEndRenderingKHR(m_CommandBuffer);

    imgMemBarrier = {
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
    vkDestroyDevice(m_vkDevice, nullptr);
    vkDestroySurfaceKHR(m_vkInstance, m_vkSurface, nullptr);
}
