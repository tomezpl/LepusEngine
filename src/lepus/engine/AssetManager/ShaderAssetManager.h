#ifndef LEPUS_ENGINE_ASSETMANAGER_SHADERASSETMANAGER
#define LEPUS_ENGINE_ASSETMANAGER_SHADERASSETMANAGER
#include "IAssetManager.h"

#include <forward_list>
#include <cstring>
#include <cstdlib>

namespace lepus
{
    namespace engine
    {
	enum ShaderAssetType
	{
	    ShaderAssetTypeInvalid = 0,
	    ShaderAssetTypeGLSL,
	    ShaderAssetTypeSPV
	};

	struct ShaderAsset
	{
	    ShaderAssetType type = ShaderAssetTypeInvalid;
	    const char* const path = nullptr;
	    const void* const data = nullptr;
	    size_t szData = 0;
	};

	class ShaderAssetManager : public IAssetManager
	{
	    private:
	    std::forward_list<ShaderAsset> m_Shaders;

	    static const char* const _DefaultBasePath;
	    static size_t _NumDefaultBasePathChars;

	    protected:
	    const void* _LoadShader(const char* path, ShaderAssetType type, size_t& szData);

	    public:
	    void Init() override
	    {
	    }

	    const ShaderAsset AddShader(const char* const shaderFileName, ShaderAssetType type, const char* const basePath = nullptr)
	    {
		const char* path;
		size_t numBasePathChars = (basePath ? strlen(basePath) : _NumDefaultBasePathChars);
		size_t numPathChars = numBasePathChars + strlen(shaderFileName);
		path = new char[numPathChars + 1];
		// Copy the base path
		memcpy((void*)path, (void const*)(basePath ? basePath : _DefaultBasePath), numBasePathChars * sizeof(char));
		// Copy the filename
		memcpy((void*)(path + numBasePathChars * sizeof(char)), shaderFileName, sizeof(char) * (numPathChars - numBasePathChars));
		// Set last character to null terminating
		memset((void*)(path + numPathChars * sizeof(char)), 0, sizeof(char));
		size_t szData = 0;
		ShaderAsset newShader = {
		    type,
		    path,
		    _LoadShader(path, type, szData),
		    szData};
		// We're pushing a memberwise copy of the struct to the list. This is fine,
		// but just be careful about what you do with the data and path members since they're heap-allocated,
		// and so we assume that the pushed copy will own the data.
		m_Shaders.push_front(newShader);
		return newShader;
	    }

	    void Dispose() override
	    {
		for (auto it = m_Shaders.begin(); it != m_Shaders.end(); ++it)
		{
		    free((void*)it->data);
		    free((void*)it->path);
		}

		m_Shaders.clear();
	    }
	};
    } // namespace engine
} // namespace lepus

#endif
