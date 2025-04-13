#ifndef LEPUS_ENGINE_ASSETMANAGER
#define LEPUS_ENGINE_ASSETMANAGER

#include "AssetManager/ShaderAssetManager.h"

#include <cassert>

namespace lepus
{
    namespace engine
    {
	class AssetManager
	{
	    private:
	    ShaderAssetManager m_ShaderAssetManager;

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

	    inline void Dispose()
	    {
		assert(!m_Disposed);
		m_ShaderAssetManager.Dispose();
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
