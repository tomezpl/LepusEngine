#ifndef LTESTS_L3D_GRAPHICSENGINE_GRAPHICSAPI
#define LTESTS_L3D_GRAPHICSENGINE_GRAPHICSAPI

#include <L3D/GraphicsEngine/GraphicsApi.h>
#include "GraphicsApiOptionsTests.h"

class GraphicsApiStub : public LepusEngine::Lepus3D::GraphicsApi
{
private:
	/// @brief Pointer to a bool that will be set to true upon Shutdown being called.
	bool* m_ShutdownReceiver = nullptr;

public:
	GraphicsApiStub(GraphicsApiStubOptions* options, bool* shutdownReceiver = nullptr)
	{
		Init(options);

		m_ShutdownReceiver = shutdownReceiver;
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
		GraphicsApi::Shutdown();

		if (m_Options)
		{
			if (m_ShutdownReceiver)
			{
				*m_ShutdownReceiver = true;
			}

			// TODO: is this cast needed?
			delete reinterpret_cast<GraphicsApiStubOptions*>(m_Options);
			m_Options = nullptr;
		}
	}

	void ClearFrameBuffer(float r, float g, float b) override
	{
		// Do nothing.
	}
};

#endif