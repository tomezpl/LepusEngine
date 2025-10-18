#ifndef L3D_GRAPHICSENGINE
#define L3D_GRAPHICSENGINE

#include "Camera.h"
#include "SceneGraph.h"
#include "GraphicsEngine/GraphicsApi.h"
#include "GraphicsEngine/Apis/ApiVk.h"
#include "GraphicsEngine/Apis/ApiGL/Types/GLShader.h"
#include "GraphicsEngine/Apis/ApiVk/Types/VkShader.h"
#include "GraphicsEngine/ShaderCompilers/ShaderCompilerGLSL.h"
#include "GraphicsEngine/ShaderCompilers/ShaderCompilerVk.h"
#include "lepus/utility/types/List.h"

#include <forward_list>
#include <lepus/system/Windowing.h>
#include <lepus/utility/types/Viewport.h>

namespace lepus
{
    namespace gfx
    {
	class GraphicsEngine
	{
	    protected:
	    /// @brief Graphics API wrapper (GL, Vk, D3D).
#if LEPUS_FORCE_API_OPENGL
	    friend class GraphicsApiGL;
	    using GraphicsApiClass = GraphicsApiGL;
#elif LEPUS_FORCE_API_VK
	    friend class GraphicsApiVk;
	    using GraphicsApiClass = GraphicsApiVk;
#elif LEPUS_USE_DYNAMIC_API
	    using GraphicsApiClass = GraphicsApi;
#endif
	    GraphicsApiClass* m_Api;

	    /// @brief Windowing interface wrapper. This can be shared by multiple systems, not just graphics,
	    /// and implemented through many platform-specific libraries.
	    lepus::system::Windowing* m_Windowing;

	    struct
	    {
		std::string windowName;
		lepus::types::Viewport viewport;
	    } m_OutputInfo;

	    SceneGraph m_Scene;

	    struct
	    {
		std::forward_list<lepus::engine::objects::Mesh*> meshes;
		utility::List<AnyShader*>* shaders;
	    } m_Resources;

	    void _InitDefault()
	    {
		m_Api = nullptr;
		m_Windowing = nullptr;
		m_OutputInfo = {};
	    }

	    public:
	    /// @brief Creates a default GraphicsEngine. InitWindowing and InitApi need to be called manually.
	    GraphicsEngine()
	    {
		_InitDefault();
	    }

	    /// @brief Creates a GraphicsEngine using the provided API options and a windowing context.
	    /// @param options API options. This will be used to create a GraphicsApi instance of the right type. The engine has ownership over the created API wrapper.
	    /// @param windowing Windowing context containing a window handle, dimensions, image format, etc.
	    GraphicsEngine(GraphicsApiOptions* options, lepus::system::Windowing* windowing)
	    {
		_InitDefault();

		InitWindowing(windowing);
		InitApi(options);
	    }

	    /// @brief Assigns a windowing context to use with this GraphicsEngine. This usually needs to be done before InitApi.
	    /// @param windowing Created windowing context to use.
	    void InitWindowing(lepus::system::Windowing* windowing);

	    /// @brief Initialises the API library and creates an instance of the wrapper for the right API type.
	    /// @param options API options used to initialise and create the API library wrapper. These are copied, so it is recommended to create options in stack.
	    void InitApi(GraphicsApiOptions* options);

	    enum PixelFormat
	    {
		RGBA32 = 256
	    };

	    // 	    template <class TGraphicsApi>
	    // 	    inline TGraphicsApi& GetApi()
	    // 	    {
	    // #if LEPUS_USE_DYNAMIC_API
	    // 		return *((TGraphicsApi*)m_Api);
	    // #else
	    // 		return *reinterpret_cast<GraphicsApiClass*>(&m_Api);
	    // #endif
	    // 	    }

	    inline GraphicsApiClass& GetApi()
	    {
		return *m_Api;
	    }

	    void Setup();

	    // TODO: methods to get uniform buffer object (this should be mapped memory) and then flush it (unmap)
	    // TODO: also allow specifying uniform buffer size at runtime

	    template <class TUniformModel = PushConstantModelBase>
	    const AnyShader* RegisterShader(const char* name, const engine::ShaderAsset& vertexShader, const engine::ShaderAsset& fragmentShader)
	    {
		auto apiKind = GetApi().GetType();
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
#if !LEPUS_FORCE_API_OPENGL
		    newShader = (AnyShader*)(new VkShader(info));
		    ((VkShader*)newShader)->SetShaderModule(spirvCompiler.CompileShader(fragmentShader.data, fragmentShader.szData, ShaderStageFragment).ShaderHandle, ShaderStageFragment);
		    ((VkShader*)newShader)->SetShaderModule(spirvCompiler.CompileShader(vertexShader.data, vertexShader.szData, ShaderStageVertex).ShaderHandle, ShaderStageVertex);
		    static_cast<GraphicsApiVk*>(&GetApi())->AddShader((VkShader*)newShader);
#endif
		    break;
		case GraphicsApiTest:
		case GraphicsApiUnknown:
		default:
		    assert(false);
		    break;
		}

		assert(newShader != nullptr);
		m_Resources.shaders->Push(newShader);
		return reinterpret_cast<AnyShader*>(newShader);
	    }

	    /// @brief Renders the scene and performs buffer swap to display the results in the window.
	    /// @tparam T Numerical type used for the solid clear colour.
	    /// @tparam bits Number of allowed integer values for a single colour channel in a pixel format, e.g. 256 for RGBA32
	    /// @param r Solid clear colour value (red)
	    /// @param g Solid clear colour value (green)
	    /// @param b Solid clear colour value (blue)
	    template <typename T, PixelFormat bits>
	    void Render(T r, T g, T b)
	    {
		const float max = bits - 1.f;
		Render((float)r / max, (float)g / max, (float)b / max);
	    }

	    /// @brief Renders the scene and performs buffer swap to display the results in the window.
	    /// @param r Solid clear colour value (red, range [0, 1])
	    /// @param g Solid clear colour value (green, range [0, 1])
	    /// @param b Solid clear colour value (blue, range [0, 1])
	    void Render(float r, float g, float b);

	    inline SceneGraph& GetSceneGraph() { return m_Scene; }

	    /// @brief Creates a renderable mesh from provided geometry. The geometry data is copied and an API-specific resource is created for the returned object.
	    /// @param geometry Primitive object to copy geometry data from.
	    lepus::engine::objects::Mesh* CreateMesh(const utility::Primitive& geometry);

	    void Dispose();

	    static GraphicsEngine Create(GraphicsApiOptions* options)
	    {
		return {options, options->GetWindowing()};
	    }
	};
    } // namespace gfx
} // namespace lepus

#endif