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
    auto physDevWrapper = deviceSelector.set_surface(m_vkSurface).set_minimum_version(1, 1).require_dedicated_transfer_queue().select();
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
    auto swapchainResult = swapchainBuilder.build();
    assert(swapchainResult);

    m_vkDevice = device;
    m_vkQueue = graphicsQueue;
    m_vkInstance = vkbInstance.instance;
    m_vkSwapchain = swapchainResult.value();

    VkCommandPoolCreateInfo cmdPoolCreateInfo;
    cmdPoolCreateInfo.flags = 0;
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
}

lepus::engine::objects::Mesh* GraphicsApiVk::WrapMesh(engine::objects::Mesh* mesh)
{
    // TODO: create IBO, VBO etc
    return mesh;
}

void GraphicsApiVk::ClearFrameBuffer(float r, float g, float b)
{
    VkCommandBufferBeginInfo beginInfo;
    beginInfo.pNext = 0;
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = 0;

    vkBeginCommandBuffer(m_CommandBuffer, &beginInfo);
    VkImageSubresourceRange ranges;
    ranges.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    ranges.layerCount = 1;
    ranges.levelCount = 1;
    ranges.baseArrayLayer = 0;
    ranges.baseMipLevel = 0;
    VkClearColorValue colour;
    const float gamma = 2.2f;
    colour.float32[0] = powf(r, gamma);
    colour.float32[1] = powf(g, gamma);
    colour.float32[2] = powf(b, gamma);
    colour.float32[3] = 1.f;
    // colour.int32[0] = 100;
    // colour.int32[1] = 149;
    // colour.int32[2] = 237;
    // colour.int32[3] = 255;
    // colour.uint32[0] = 100;
    // colour.uint32[1] = 149;
    // colour.uint32[2] = 237;
    // colour.uint32[3] = 255;
    uint32_t swapchainImageCount = 0;
    vkGetSwapchainImagesKHR(m_vkDevice, m_vkSwapchain, &swapchainImageCount, nullptr);
    if (swapchainImageCount > 0)
    {
	VkImage* swapchainImages = new VkImage[swapchainImageCount];
	vkGetSwapchainImagesKHR(m_vkDevice, m_vkSwapchain, &swapchainImageCount, swapchainImages);

	for (uint32_t i = 0; i < swapchainImageCount; i++)
	{

	    vkCmdClearColorImage(m_CommandBuffer, swapchainImages[i], VK_IMAGE_LAYOUT_GENERAL, &colour, 1, &ranges);
	}

	// delete[] swapchainImages;
    }
    vkEndCommandBuffer(m_CommandBuffer);

    vkAcquireNextImageKHR(m_vkDevice, m_vkSwapchain, UINT64_MAX, nullptr, nullptr, &m_CurrentImageIndex);
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
    vkQueueSubmit(m_vkQueue, 1, &submitInfo, nullptr);
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
    vkDestroyCommandPool(m_vkDevice, m_CommandPool, nullptr);
    vkDestroySwapchainKHR(m_vkDevice, m_vkSwapchain, nullptr);
    vkDestroyDevice(m_vkDevice, nullptr);
    vkDestroySurfaceKHR(m_vkInstance, m_vkSurface, nullptr);
}
