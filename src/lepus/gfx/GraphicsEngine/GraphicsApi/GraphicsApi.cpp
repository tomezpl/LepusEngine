#include "../GraphicsApi.h"

#include "lepus/gfx/GraphicsEngine/Apis/ApiGL.h"

using namespace lepus::gfx;

std::shared_ptr<lepus::system::WindowingGLFW> toWindowingGlfw(const lepus::system::Windowing* windowing)
{
    return std::shared_ptr<lepus::system::WindowingGLFW>(dynamic_cast<lepus::system::WindowingGLFW*>(const_cast<lepus::system::Windowing*>(windowing)));
}

GraphicsApiOptions* GraphicsApiOptions::Create(const lepus::system::Windowing* windowing, GraphicsApiType type)
{
    switch (type)
    {
    case GraphicsApiOpenGL:
    case GraphicsApiVulkan:
	if (type == GraphicsApiVulkan)
	{
	    auto vkApiOptions = new GraphicsApiVkOptions();
	    vkApiOptions->windowingPtr = toWindowingGlfw(windowing);
	    return vkApiOptions;
	}

	return new GraphicsApiGLOptions(toWindowingGlfw(windowing));
    case GraphicsApiTest:
    case GraphicsApiUnknown:
    default:
	assert(false);
    }

    return nullptr;
}