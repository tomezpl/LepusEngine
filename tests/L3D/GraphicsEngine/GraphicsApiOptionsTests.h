#ifndef LTESTS_L3D_GRAPHICSENGINE_GRAPHICSAPIOPTIONS
#define LTESTS_L3D_GRAPHICSENGINE_GRAPHICSAPIOPTIONS

#include <L3D/GraphicsEngine/GraphicsApi.h>

class GraphicsApiStubOptions : public LepusEngine::Lepus3D::GraphicsApiOptions
{
public:
	LepusEngine::Lepus3D::GraphicsApiType GetType() override { return LepusEngine::Lepus3D::GraphicsApiType::GraphicsApiTest; }

	long long testValue = 0;
};

#endif