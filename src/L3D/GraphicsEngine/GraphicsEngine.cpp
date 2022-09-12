#include <cassert>
#include <type_traits>

#include "../GraphicsEngine.h"
#include "Apis.h"

using namespace LepusEngine::Lepus3D;

void GraphicsEngine::InitWindowing(std::shared_ptr<LepusSystem::Windowing> windowing)
{
	m_Windowing = windowing;
}

void GraphicsEngine::InitApi(GraphicsApiOptions* options)
{
	// Check that an API wasn't initialised before. For now, only one API can be initialised at runtime.
	assert(m_Api == nullptr);

	// Check that windowing has been initialised. In most cases we'll need windowing to make use of the API.
	assert(m_Windowing != nullptr);

	switch (options->GetType())
	{
	case GraphicsApiType::GraphicsApiOpenGL:
		m_Api = new GraphicsApiGL(*reinterpret_cast<GraphicsApiGLOptions*>(options));
		break;
	default:
		// Assert if the API type is not part of the enum.
		assert(false);
		break;
	}
}

void GraphicsEngine::RenderInternal(float r = 0.f, float g = 0.f, float b = 0.f)
{
	m_Api->ClearFrameBuffer(r, g, b);
}