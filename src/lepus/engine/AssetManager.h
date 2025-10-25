#ifndef LEPUS_ENGINE_ASSETMANAGER
#define LEPUS_ENGINE_ASSETMANAGER

#include "AssetManager/ShaderAssetManager.h"
#include "AssetManager/TextureAssetManager.h"

#include <cassert>

namespace lepus
{
    namespace engine
    {
	class AssetManager
	{
	    private:
	    ShaderAssetManager m_ShaderAssetManager;
	    TextureAssetManager m_TextureAssetManager;

	    static AssetManager m_Instance;

	    bool m_Disposed;

	    AssetManager()
	    {
		m_Disposed = false;
	    }

	    public:
	    [[nodiscard]] inline ShaderAssetManager& Shaders()
	    {
		return m_ShaderAssetManager;
	    }

	    [[nodiscard]] inline TextureAssetManager& Textures()
	    {
		return m_TextureAssetManager;
	    }

	    inline void Dispose()
	    {
		assert(!m_Disposed);
		m_ShaderAssetManager.Dispose();
		m_TextureAssetManager.Dispose();

		m_Disposed = true;
	    }

	    ~AssetManager()
	    {
		if (!m_Disposed)
		{
		    Dispose();
		}
	    }

	    inline static AssetManager& Singleton()
	    {
		return m_Instance;
	    }
	};
    } // namespace engine
} // namespace lepus

#endif
