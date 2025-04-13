#include <cassert>
#include <type_traits>

#include "../GraphicsEngine.h"
#include "Apis.h"
#include "Apis/ApiVk.h"
#include "Apis/ApiGL/Types/GLShader.h"
#include "Apis/ApiVk/Types/VkShader.h"
#include "ShaderCompilers/ShaderCompilerVk.h"

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
	m_Resources.shaders = (utility::List<AnyShader*>*)(new utility::List<GLShader*>());
	break;
    case GraphicsApiType::GraphicsApiVulkan:
	m_Api = new GraphicsApiVk(options);
	m_Resources.shaders = (utility::List<AnyShader*>*)(new utility::List<VkShader*>());
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
    lepus::engine::objects::Mesh* createdMesh = nullptr;

    // Need to wrap geometry in a mesh object first (it's OK for this to be on stack, APIs need to copy the data from it anyway)
    lepus::engine::objects::Mesh tempMesh = lepus::engine::objects::Mesh(geometry, false);
    switch (m_Api->GetOptions<GraphicsApiOptions>().GetType())
    {
    case GraphicsApiUnknown:
	assert(false);
	break;
    case GraphicsApiTest:
	createdMesh = nullptr;
	break;
    case GraphicsApiOpenGL:
    case GraphicsApiVulkan:
	createdMesh = m_Api->WrapMesh(&tempMesh);
	break;
    }

    // Store for resource management
    if (createdMesh)
    {
	m_Resources.meshes.push_front(createdMesh);
    }

    return createdMesh;
}

const AnyShader* GraphicsEngine::RegisterShader(const char* name, const engine::ShaderAsset& vertexShader, const engine::ShaderAsset& fragmentShader)
{
    auto apiKind = m_Api->GetOptions<GraphicsApiOptions>().GetType();
    ShaderInfo info = ShaderInfo(name, static_cast<ShaderStage>(ShaderStageVertex | ShaderStageFragment));

    // Both shader assets need to be of the same target API type
    assert(vertexShader.type == fragmentShader.type);
    // Shader target API needs to match the engine's backend
    assert((apiKind == GraphicsApiVulkan && vertexShader.type == engine::ShaderAssetTypeSPV) || (apiKind == GraphicsApiOpenGL && vertexShader.type == engine::ShaderAssetTypeGLSL));

    auto& spirvCompiler = ShaderCompilerVk::Singleton();

    AnyShader* newShader = nullptr;

    switch (apiKind)
    {
    case GraphicsApiOpenGL:
	newShader = (AnyShader*)(new GLShader(info));
	((GLShader*)newShader)->SetGLProgram(ShaderCompilerGLSL::Singleton().BuildProgram(ShaderCompilerGLSL::Singleton().CompileShader(fragmentShader.data, fragmentShader.szData, ShaderStageFragment), ShaderCompilerGLSL::Singleton().CompileShader(vertexShader.data, vertexShader.szData, ShaderStageVertex)));
	break;
    case GraphicsApiVulkan:
	newShader = (AnyShader*)(new VkShader(info));
	((VkShader*)newShader)->SetShaderModule(spirvCompiler.CompileShader(fragmentShader.data, fragmentShader.szData, ShaderStageFragment).ShaderHandle, ShaderStageFragment);
	((VkShader*)newShader)->SetShaderModule(spirvCompiler.CompileShader(vertexShader.data, vertexShader.szData, ShaderStageVertex).ShaderHandle, ShaderStageVertex);
	static_cast<GraphicsApiVk*>(m_Api)->AddShader((VkShader*)newShader);
	break;
    case GraphicsApiTest:
    case GraphicsApiUnknown:
    default:
	assert(false);
	break;
    }

    assert(newShader != nullptr);
    m_Resources.shaders->Push(newShader);
    return newShader;
}

void GraphicsEngine::Dispose()
{
    for (auto it = m_Resources.meshes.begin(); it != m_Resources.meshes.end(); ++it)
    {
	lepus::engine::objects::Mesh* mesh = *it;
	mesh->Dispose();
	delete mesh;
    }

    m_Resources.meshes.clear();
}
