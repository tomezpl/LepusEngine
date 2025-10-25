#define STB_IMAGE_IMPLEMENTATION
#include "ImageLoader.h"

#include "lepus/engine/3rdparty/stb_image.h"

uint8_t* lepus::engine::ImageLoader::Load(std::byte* imageFileBytes, size_t nbImageFileBytes, uint16_t& width, uint16_t& height, uint8_t& componentsPerPixel, uint8_t& nbComponentBytes)
{
    int _width, _height, _comps;
    uint8_t* pixels = stbi_load_from_memory(reinterpret_cast<stbi_uc*>(imageFileBytes), static_cast<int>(nbImageFileBytes), &_width, &_height, &_comps, 3);
    width = static_cast<uint16_t>(_width);
    height = static_cast<uint16_t>(_height);
    componentsPerPixel = static_cast<uint8_t>(_comps);

    return pixels;
}
