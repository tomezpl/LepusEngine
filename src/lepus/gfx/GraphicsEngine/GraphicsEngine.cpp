#include <cassert>
#include <type_traits>

#include "../GraphicsEngine.h"
#include "Apis.h"

using namespace lepus::gfx;

void GraphicsEngine::InitWindowing(std::shared_ptr<lepus::system::Windowing> windowing)
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
			m_Api = new GraphicsApiGL(*dynamic_cast<GraphicsApiGLOptions*>(options));
			break;
		case GraphicsApiType::GraphicsApiVulkan:
			// TODO
			break;
		case GraphicsApiType::GraphicsApiTest:
			// Ignore test/mock APIs.
			break;
		case GraphicsApiType::GraphicsApiUnknown:
		default:
			// Assert if the API type is not part of the enum.
			assert(false);
			break;
	}
}

void GraphicsEngine::Setup()
{
	m_Api->CreatePipeline();
}

void GraphicsEngine::Render(const float r, const float g, const float b)
{
	m_Api->ClearFrameBuffer(r, g, b);

	m_Api->UpdateUniforms();
	m_Api->Draw();

	m_Api->SwapBuffers();
	m_Windowing->SwapBuffers();
}