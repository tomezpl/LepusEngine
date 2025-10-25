#ifndef LEPUS_ENGINE_ASSETMANAGER_TEXTUREASSETMANAGER
#define LEPUS_ENGINE_ASSETMANAGER_TEXTUREASSETMANAGER
#include "IAssetManager.h"
#include "TextureAssetManager/ImageLoader.h"
#include "lepus/engine/ConsoleLogger.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <fstream>
#include <string>
#include <vector>

namespace lepus
{
    namespace engine
    {
	typedef std::byte* TextureData;

	typedef struct TextureAsset
	{
	    TextureData data;
	    uint16_t width;
	    uint16_t height;
	} TextureAsset;

	class TextureAssetManager : public IAssetManager
	{
	    public:
	    void Init() override {}

	    TextureAsset LoadTexture(std::istream& textureStream)
	    {
		std::vector<std::byte> bytes = {};
		constexpr uint16_t bufferSize = 1024;
		const auto buffer = std::make_unique<std::byte[]>(bufferSize);
		while (textureStream.good())
		{
		    memset(buffer.get(), 0, bufferSize);
		    textureStream.read(reinterpret_cast<char*>(buffer.get()), bufferSize);
		    bytes.insert(bytes.end(), buffer.get(), buffer.get() + textureStream.gcount());
		}
		std::string message = "loaded ";
		message.append(std::to_string(bytes.size()));
		message.append(" bytes");
		ConsoleLogger::Global().LogInfo("TextureAssetManager", "LoadTexture", message.c_str(), "textureStream&");

		uint16_t width;
		uint16_t height;
		uint8_t comps;
		uint8_t compSize;
		const auto pixels = ImageLoader::Load(bytes.data(), bytes.size(), width, height, comps, compSize);
		auto nbFinal = static_cast<size_t>(width) * static_cast<size_t>(height) * 3;
		auto rgba32Pixels = new uint8_t[nbFinal];
		memset(rgba32Pixels, 255, nbFinal * sizeof(uint8_t));
		// memset(rgba32Pixels + nbFinal / 2, 128, ((nbFinal / 2) - 1) * sizeof(uint8_t));
		uint32_t rgba32Counter = 0;
		for (uint32_t i = 0; i < static_cast<uint32_t>(width * height * comps); i += comps)
		{
		    memcpy(rgba32Pixels + rgba32Counter, pixels + i, comps);
		    rgba32Counter += 3;
		}
		return {
		    reinterpret_cast<TextureData>(rgba32Pixels),
		    width,
		    height};
	    }

	    TextureAsset LoadTextureFromContent(const char* contentPath)
	    {
		std::string fullPath = "../../Content/";
		fullPath.append(contentPath);
		std::ifstream stream;
		stream.open(fullPath, std::fstream::in | std::fstream::binary);
		assert(stream.is_open());
		auto asset = LoadTexture(stream);

		if (stream.is_open())
		{
		    stream.close();
		}

		return asset;
	    }

	    void Dispose() override
	    {
	    }
	};
    } // namespace engine
} // namespace lepus

#endif