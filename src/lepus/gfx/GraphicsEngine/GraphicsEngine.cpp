#include <cassert>
#include <type_traits>

#include "../GraphicsEngine.h"
#include "Apis.h"
#include "Apis/ApiVk.h"

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
	m_Api = new GraphicsApiGL(*static_cast<GraphicsApiGLOptions*>(options));
	break;
    case GraphicsApiType::GraphicsApiVulkan:
	m_Api = new GraphicsApiVk(options);
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

    m_Api->UpdateUniforms(m_Scene);
    m_Api->StartDrawing();
    m_Api->Draw(m_Scene);
    m_Api->EndDrawing();

    m_Api->SwapBuffers();
    m_Windowing->SwapBuffers();
}

lepus::engine::objects::Mesh* GraphicsEngine::CreateMesh(const utility::Primitive& geometry)
{
    switch (m_Api->GetOptions<GraphicsApiOptions>().GetType())
    {
    case GraphicsApiUnknown:
	assert(false);
	break;
    case GraphicsApiTest:
	return nullptr;
	break;
    case GraphicsApiOpenGL:
    case GraphicsApiVulkan:
	return m_Api->WrapMesh(new lepus::engine::objects::Mesh(geometry, true));
	break;
    }

    return 0;
}
