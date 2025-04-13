#ifndef LEPUS_ENGINE_ASSETMANAGER_IASSETMANAGER
#define LEPUS_ENGINE_ASSETMANAGER_IASSETMANAGER

namespace lepus
{
    namespace engine
    {
	class IAssetManager
	{
	    public:
	    virtual ~IAssetManager() = default;
	    virtual void Init() = 0;
	    virtual void Dispose() = 0;
	};
    } // namespace engine
} // namespace lepus

#endif
