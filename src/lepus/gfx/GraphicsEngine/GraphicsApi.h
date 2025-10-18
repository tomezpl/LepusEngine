#ifndef L3D_GRAPHICSENGINE_GRAPHICSAPI
#define L3D_GRAPHICSENGINE_GRAPHICSAPI

// #ifndef LEPUS_FORCE_API_OPENGL
// #define LEPUS_FORCE_API_OPENGL 0
// #endif
//
// #ifndef LEPUS_FORCE_API_VK
// #define LEPUS_FORCE_API_VK 0
// #endif
//
// #ifndef LEPUS_FORCE_API_TEST
// #define LEPUS_FORCE_API_TEST 0
// #endif
//
#if LEPUS_FORCE_API_OPENGL == 1 || LEPUS_FORCE_API_VK == 1 || LEPUS_FORCE_API_TEST == 1
#define LEPUS_USE_DYNAMIC_API 0
#else
#define LEPUS_USE_DYNAMIC_API 1
#endif

#include <lepus/utility/types/Viewport.h>
#include <memory>
#include <cstring>
#include <cassert>
#include "GraphicsApi/BaseBindings.h"
#include "lepus/engine/AssetManager/ShaderAssetManager.h"
#include "lepus/engine/Objects/Mesh.h"
#include "lepus/gfx/SceneGraph.h"
#include "lepus/system/Windowing.h"

namespace lepus
{
    namespace gfx
    {
	/// @brief Types of graphics APIs.
	/// These are passed in GraphicsApiOptions structs to indicate which API should be initialised by the GraphicsEngine.
	enum GraphicsApiType
	{
	    /// @brief Do not use outside of unit tests.
	    GraphicsApiTest = -1,

	    /// @brief Unspecified graphics API. Results in an exception, or undefined behaviour.
	    GraphicsApiUnknown = 0,

	    /// @brief Modern OpenGL implementation.
	    GraphicsApiOpenGL,

	    /// @brief Not implemented.
	    GraphicsApiVulkan
	};

	class GraphicsApiOptions
	{
	    public:
	    /// @brief Indicates what API type this options object is used for.
	    /// @return The GraphicsApiType enum value for this GraphicsApiOptions.
	    virtual GraphicsApiType GetType() { return GraphicsApiUnknown; }

	    virtual system::Windowing* GetWindowing() = 0;

	    /// @brief Main viewport used by the application.
	    lepus::types::Viewport mainViewport = {};

	    GraphicsApiOptions()
	        : mainViewport() {}

	    GraphicsApiOptions(const GraphicsApiOptions& copy)
	    {
		CopyInternal(copy);
	    }

	    GraphicsApiOptions& operator=(const GraphicsApiOptions& copy)
	    {
		CopyInternal(copy);
		return *this;
	    }

	    virtual ~GraphicsApiOptions() {}

	    private:
	    void CopyInternal(const GraphicsApiOptions& copy)
	    {
		this->mainViewport = copy.mainViewport;
	    }

	    public:
#if LEPUS_FORCE_API_OPENGL == 1
	    friend class GraphicsApiGLOptions;
	    using GraphicsApiOptionsClass = GraphicsApiGLOptions;
	    // static GraphicsApiGLOptions Create(const system::Windowing* windowing);
#elif LEPUS_FORCE_API_VK == 1
	    friend class GraphicsApiVkOptions;
	    using GraphicsApiOptionsClass = GraphicsApiVkOptions;
	    // static GraphicsApiVkOptions Create(const system::Windowing* windowing);
#elif LEPUS_USE_DYNAMIC_API == 1
	    // TODO: template?
	    using GraphicsApiOptionsClass = GraphicsApiOptions;
#endif
	    static GraphicsApiOptions* Create(const system::Windowing* windowing, GraphicsApiType type);
	};

	/// @brief API wrapper to be used by GraphicsEngine.
	/// The purpose of this class is to obfuscate API-specific calls,
	/// so that GraphicsEngine code can focus on implementing drawing techniques,
	/// not a variety of D3D/GL/VK methods.
	class GraphicsApi
	{
	    private:
	    bool m_ShutdownCalled{false};

	    protected:
	    GraphicsApiOptions* m_Options{nullptr};

	    protected:
	    /// @brief Performs internal, boilerplate setup for all API wrappers.
	    /// @tparam TGraphicsApiOptions An options type derived from GraphicsApiOptions for a specific graphics API.
	    /// @param options Pointer to an options object (using the type matching the requested graphics API).
	    template <class TGraphicsApiOptions>
	    void InitInternal(TGraphicsApiOptions* options)
	    {
		// TODO: This might be a bad idea actually. If options contain pointers, they will be reused, and ownership conflicts may emerge.
		const size_t optionsSz = sizeof(TGraphicsApiOptions);
		m_Options = reinterpret_cast<TGraphicsApiOptions*>(new std::byte[optionsSz]);
		memcpy(m_Options, options, optionsSz);
	    }

	    /// @brief Internal API-specific method for retrieving a native handle to a uniform object.
	    /// @param name The name of the uniform to fetch.
	    /// @return API-specific handle for a uniform of a given type.
	    virtual void* GetUniformInternal(const char* name) = 0;

	    public:
	    /// @brief Default constructor. Does nothing, so Init(GraphicsApiOptions*) needs to be called manually.
	    GraphicsApi() = default;

	    explicit GraphicsApi(GraphicsApiOptions* options)
	    {
		GraphicsApi::Init(options);
	    }

	    /// @brief Initialises the API with the provided
	    /// options.
	    /// @param options An options object using a
	    /// GraphicsApiOptions type for the requested API. Make
	    /// sure you don't pass a pointer to a new object here.
	    /// Implementations of this class must copy the options,
	    /// not reference a pointer, so there's a potential risk
	    /// of a memory leak there.
	    virtual inline void Init(GraphicsApiOptions* options)
	    {
		assert(options == nullptr);

		InitInternal(options);
	    }

	    /// @brief Obtains the options object this GraphicsApi was initialised with.
	    ///
	    /// @tparam TGraphicsApiOptions An options type derived from GraphicsApiOptions
	    /// (typically a GraphicsApiOptions derived class matching the API).
	    ///
	    /// @return A reference to the internal options object.
	    /// cast to the requested API-specific TGraphicsApiOptions type.
	    template <class TGraphicsApiOptions>
	    TGraphicsApiOptions& GetOptions()
	    {
		// The internal options object NEEDS to have been allocated.
		assert(m_Options != nullptr);

		return *(TGraphicsApiOptions*)m_Options;
	    }

	    [[nodiscard]] inline GraphicsApiType GetType() const
	    {
		return m_Options->GetType();
	    }

	    virtual void CreatePipeline() = 0;

	    /// @brief Gets a Lepus UniformBinding wrapper for an API-specific uniform with the given name.
	    /// @tparam TUniformHandle API-specific handle type used for this type of uniform.
	    /// @tparam TUniformBinding UniformBinding implementation
	    /// @param name Name of the uniform to fetch.
	    /// @return A UniformBinding wrapper for the named uniform object.
	    template <typename TUniformHandle = void*, class TUniformBinding = lepus::gfx::UniformBinding<TUniformHandle>>
	    inline TUniformBinding* GetUniform(const char* name)
	    {
		return static_cast<TUniformBinding* const>(GetUniformInternal(name));
	    }

	    /// @brief Applies uniforms in the shader.
	    /// Implementations can fire & forget by issuing this before every draw, but it might be worth having a mechanism to invalidate uniforms
	    /// and only update them once they're marked as dirty.
	    virtual void UpdateUniforms(const lepus::gfx::SceneGraph& scene) = 0;

	    virtual void StartDrawing(){};
	    virtual void Draw(const lepus::gfx::SceneGraph& scene) = 0;
	    virtual void EndDrawing(){};

	    virtual void ClearFrameBuffer(float r, float g, float b) = 0;

	    /// @brief Invokes any API-specific code for back/front buffer swap.
	    /// This is usually a task for the Windowing class; GraphicsApi provides a dummy method
	    /// that can perform the API-specific swap chain operations before the call to Windowing,
	    /// but it does not need to be used or implemented.
	    virtual void SwapBuffers() {}

	    virtual void Shutdown()
	    {
		m_ShutdownCalled = true;
	    }

	    /// @brief Wraps the provided mesh object in an API-specific subclass.
	    /// @remarks The API implementation MUST by default copy the data from the mesh rather than reference it.
	    virtual engine::objects::Mesh* WrapMesh(engine::objects::Mesh* mesh) = 0;

	    /// @brief Returns the API-and-shaderstage-specific filename for a given shader name.
	    /// @remarks The return value of this function is transient; it is designed to be used for loading shaders using ShaderAssetManager, which copies the filename string.
	    /// DO NOT use this return value for more than one function call, or if you do, make your own copy of it. This method is not thread-safe.
	    [[nodiscard]] virtual const char* GetShaderFileName(const char* shaderName, ShaderStage stage) const = 0;

	    [[nodiscard]] virtual engine::ShaderAssetType GetShaderAssetType() const = 0;

	    virtual ~GraphicsApi()
	    {
		assert(m_ShutdownCalled == true);
		Shutdown();
	    }
	};
    } // namespace gfx
} // namespace lepus

#endif