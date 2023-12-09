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
	inline void* GetUniformInternal(char*) override
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

	void Draw() override
	{

	}

	void UpdateUniforms() override
	{

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
};


template const lepus::gfx::UniformBinding<void*>* lepus::gfx::GraphicsApi::GetUniform<lepus::gfx::UniformBinding<void*>*>(char* name);

#endif