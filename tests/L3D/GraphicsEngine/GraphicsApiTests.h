#ifndef LTESTS_L3D_GRAPHICSENGINE_GRAPHICSAPI
#define LTESTS_L3D_GRAPHICSENGINE_GRAPHICSAPI

#include <lepus/gfx/GraphicsEngine/GraphicsApi.h>
#include "GraphicsApiOptionsTests.h"

class GraphicsApiStub : public lepus::gfx::GraphicsApi
{
    private:
    /// @brief Pointer to a bool that will be set to true upon Shutdown being called.
    bool* m_ShutdownReceiver = nullptr;

    private:
    inline void* GetUniformInternal(const char*) override
    {
	return (void*)nullptr;
    }

    public:
    GraphicsApiStub(GraphicsApiStubOptions* options, bool* shutdownReceiver = nullptr)
    {
	Init(options);

	m_ShutdownReceiver = shutdownReceiver;
    }

    void Init(lepus::gfx::GraphicsApiOptions* options) override
    {
	InitInternal<GraphicsApiStubOptions>((GraphicsApiStubOptions*)options);
    }

    void CreatePipeline() override
    {
    }

    void Draw(const lepus::gfx::SceneGraph& scene) override
    {
    }

    void UpdateUniforms(const lepus::gfx::SceneGraph& scene) override
    {
    }

    lepus::engine::objects::Mesh* WrapMesh(lepus::engine::objects::Mesh* mesh) override
    {
	return nullptr;
    }

    void setOptionsTestValue(long long testValue)
    {
	GetOptions<GraphicsApiStubOptions>().testValue = testValue;
    }

    long long getOptionsTestValue()
    {
	return GetOptions<GraphicsApiStubOptions>().testValue;
    }

    void Shutdown() override
    {
	GraphicsApi::Shutdown();

	if (m_Options)
	{
	    if (m_ShutdownReceiver)
	    {
		*m_ShutdownReceiver = true;
	    }

	    // TODO: is this cast needed?
	    delete (GraphicsApiStubOptions*)m_Options;
	    m_Options = nullptr;
	}
    }

    void ClearFrameBuffer(float r, float g, float b) override
    {
	// Do nothing.
    }

    const char* GetShaderFileName(const char* shaderName, lepus::gfx::ShaderStage stage) const override
    {
	return shaderName;
    }

    lepus::engine::ShaderAssetType GetShaderAssetType() const override
    {
	return lepus::engine::ShaderAssetTypeInvalid;
    }
};

template lepus::gfx::UniformBinding<void*>* const lepus::gfx::GraphicsApi::GetUniform<lepus::gfx::UniformBinding<void*>*>(const char* name);

#endif