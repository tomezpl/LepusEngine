#ifndef LTESTS_L3D_GRAPHICSENGINE_GRAPHICSAPIOPTIONS
#define LTESTS_L3D_GRAPHICSENGINE_GRAPHICSAPIOPTIONS

#include <lepus/gfx/GraphicsEngine/GraphicsApi.h>

class GraphicsApiStubOptions : public lepus::gfx::GraphicsApiOptions
{
	public:
	lepus::gfx::GraphicsApiType GetType() override { return lepus::gfx::GraphicsApiType::GraphicsApiTest; }

	long long testValue = 0;
};

#endif