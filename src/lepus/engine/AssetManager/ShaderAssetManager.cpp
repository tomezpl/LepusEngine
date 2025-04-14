#include "ShaderAssetManager.h"

#include "lepus/system/IO/FileSystem.h"

#include <cassert>
#include <cstdint>

using namespace lepus::engine;
const char* const ShaderAssetManager::_DefaultBasePath = "../../Content/GLSL/";
size_t ShaderAssetManager::_NumDefaultBasePathChars = strlen(ShaderAssetManager::_DefaultBasePath);

const char* LoadGLSLStringFromDisk(const char* path, size_t& size)
{
    std::string loadedString = lepus::system::FileSystem::Read(path);
    size_t stringLength = loadedString.length();
    const char* const stringCopy = new char[stringLength + 1];
    loadedString.copy((char*)stringCopy, stringLength);
    memset((void*)(stringCopy + stringLength * sizeof(char)), 0, sizeof(char));
    size = (stringLength + 1) * sizeof(char);
    return stringCopy;
}

const void* LoadSPVBinaryFromDisk(const char* path, size_t& size)
{
    return lepus::system::FileSystem::ReadBinary<uint32_t>(path, size);
}

const void* const ShaderAssetManager::_LoadShader(const char* path, ShaderAssetType type, size_t& szData)
{
    assert(type != ShaderAssetTypeInvalid);

    const void* data = nullptr;
    switch (type)
    {
    case ShaderAssetTypeGLSL:
	data = static_cast<const void*>(LoadGLSLStringFromDisk(path, szData));
	break;
    case ShaderAssetTypeSPV:
	data = LoadSPVBinaryFromDisk(path, szData);
	break;
    case ShaderAssetTypeInvalid:
    default:
	// Invalid type
	assert(false);
	break;
    }

    assert(data);
    assert(szData);

    return data;
}
