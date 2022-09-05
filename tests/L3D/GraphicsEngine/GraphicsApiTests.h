#ifndef LTESTS_L3D_GRAPHICSENGINE_GRAPHICSAPI
#define LTESTS_L3D_GRAPHICSENGINE_GRAPHICSAPI

#include <L3D/GraphicsEngine/GraphicsApi.h>

struct GraphicsApiStubOptions : LepusEngine::Lepus3D::GraphicsApiOptions
{
	LepusEngine::Lepus3D::GraphicsApiType type = LepusEngine::Lepus3D::GraphicsApiType::GraphicsApiTest;

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

	void Shutdown()
	{
		// TODO: is this cast needed?
		delete reinterpret_cast<GraphicsApiStubOptions*>(m_Options);
	}
};

#endif