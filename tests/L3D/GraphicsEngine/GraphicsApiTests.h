#ifndef LTESTS_L3D_GRAPHICSENGINE_GRAPHICSAPI
#define LTESTS_L3D_GRAPHICSENGINE_GRAPHICSAPI

#include <L3D/GraphicsEngine/GraphicsApi.h>

class GraphicsApiStubOptions : public LepusEngine::Lepus3D::GraphicsApiOptions
{
public:
	LepusEngine::Lepus3D::GraphicsApiType GetType() override { return LepusEngine::Lepus3D::GraphicsApiType::GraphicsApiTest; }

	long long testValue = 0;
};

class GraphicsApiStub : public LepusEngine::Lepus3D::GraphicsApi
{
public:
	GraphicsApiStub(GraphicsApiStubOptions* options)
	{
		Init(options);
	}

	void Init(LepusEngine::Lepus3D::GraphicsApiOptions* options) override
	{
		InitInternal<GraphicsApiStubOptions>(reinterpret_cast<GraphicsApiStubOptions*>(options));
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
		if (m_Options)
		{
			// TODO: is this cast needed?
			delete reinterpret_cast<GraphicsApiStubOptions*>(m_Options);
			m_Options = nullptr;
		}
	}

	void ClearFrameBuffer(float r, float g, float b) override
	{
		// Do nothing.
	}

	L3D_GRAPHICSAPI_IMPL(GraphicsApiStub);
};

#endif