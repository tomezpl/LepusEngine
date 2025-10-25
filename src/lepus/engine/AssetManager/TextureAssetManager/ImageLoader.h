#ifndef LEPUS_ENGINE_ASSETMANAGER_TEXTUREASSETMANAGER_IMAGELOADER
#define LEPUS_ENGINE_ASSETMANAGER_TEXTUREASSETMANAGER_IMAGELOADER
#include <memory>

namespace lepus
{
    namespace engine
    {
	class ImageLoader
	{
	    public:
	    static uint8_t* Load(std::byte* imageFileBytes, size_t nbImageFileBytes, uint16_t& width, uint16_t& height, uint8_t& componentsPerPixel, uint8_t& nbComponentBytes);
	};
    } // namespace engine
} // namespace lepus

#endif
