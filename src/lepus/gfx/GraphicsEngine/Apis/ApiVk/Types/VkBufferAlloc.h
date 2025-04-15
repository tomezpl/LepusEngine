#ifndef L3D_GRAPHICSENGINNE_API_VK_BUFFERALLOC
#define L3D_GRAPHICSENGINNE_API_VK_BUFFERALLOC

#ifndef WIN32
#include "vk_mem_alloc.h"
#include <VkBootstrap.h>
#else
#define VK_USE_PLATFORM_WIN32_KHR
#include <VkBootstrap.h>
#include <vma/vk_mem_alloc.h>
#endif

namespace lepus::gfx
{
    struct VkBufferAlloc
    {
	VkBuffer buffer{VK_NULL_HANDLE};
	VmaAllocation allocation{nullptr};
    };
} // namespace lepus::gfx

#endif