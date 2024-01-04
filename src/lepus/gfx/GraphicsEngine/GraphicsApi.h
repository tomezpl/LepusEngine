#ifndef L3D_GRAPHICSENGINE_GRAPHICSAPI
#define L3D_GRAPHICSENGINE_GRAPHICSAPI

#include <lepus/utility/types/Viewport.h>
#include <memory>
#include <cstring>
#include <cassert>
#include "GraphicsApi/BaseBindings.h"

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

			/// @brief Main viewport used by the application.
			lepus::types::Viewport mainViewport = {};

			virtual ~GraphicsApiOptions()
			{

			}
		};

		/// @brief API wrapper to be used by GraphicsEngine.
		/// The purpose of this class is to obfuscate API-specific calls,
		/// so that GraphicsEngine code can focus on implementing drawing techniques,
		/// not a variety of D3D/GL/VK methods.
		class GraphicsApi
		{
			private:
			bool m_ShutdownCalled;

			protected:
			GraphicsApiOptions* m_Options;

			protected:
			/// @brief Performs internal, boilerplate setup for all API wrappers.
			/// @tparam TGraphicsApiOptions An options type derived from GraphicsApiOptions for a specific graphics API.
			/// @param options Pointer to an options object (using the type matching the requested graphics API).
			template <class TGraphicsApiOptions>
			void InitInternal(TGraphicsApiOptions* options)
			{
				// TODO: This might be a bad idea actually. If options contain pointers, they will be reused, and ownership conflicts may emerge.
				const size_t optionsSz = sizeof(TGraphicsApiOptions);
				m_Options = new TGraphicsApiOptions();
				memcpy(m_Options, options, optionsSz);
			}

			/// @brief Internal API-specific method for retrieving a native handle to a uniform object.
			/// @param name The name of the uniform to fetch.
			/// @return API-specific handle for a uniform of a given type.
			virtual void* GetUniformInternal(char* name) = 0;

			public:
			/// @brief Default constructor. Does nothing, so Init(GraphicsApiOptions*) needs to be called manually.
			GraphicsApi()
			{
				m_Options = nullptr;
				m_ShutdownCalled = false;
			}

			GraphicsApi(GraphicsApiOptions* options)
			{
				Init(options);
				m_ShutdownCalled = false;
			}

			/// @brief Initialises the API with the provided options.
			/// @param options An options object using a GraphicsApiOptions type for the requested API.
			/// Make sure you don't pass a pointer to a new object here. Implementations of this class must copy the options,
			/// not reference a pointer, so there's a potential risk of a memory leak there.
			virtual void Init(GraphicsApiOptions* options) = 0;

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

			virtual void CreatePipeline() = 0;

			/// @brief Gets a Lepus UniformBinding wrapper for an API-specific uniform with the given name.
			/// @tparam TUniformHandle API-specific handle type used for this type of uniform.
			/// @tparam TUniformBinding UniformBinding implementation
			/// @param name Name of the uniform to fetch.
			/// @return A UniformBinding wrapper for the named uniform object.
			template<typename TUniformHandle = void*, class TUniformBinding = lepus::gfx::UniformBinding<TUniformHandle>>
			inline const TUniformBinding* GetUniform(char* name)
			{
				return reinterpret_cast<const TUniformBinding*>(GetUniformInternal(name));
			}

			/// @brief Applies uniforms in the shader.
			/// Implementations can fire & forget by issuing this before every draw, but it might be worth having a mechanism to invalidate uniforms
			/// and only update them once they're marked as dirty.
			virtual void UpdateUniforms() = 0;

			virtual void Draw() = 0;

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

			virtual ~GraphicsApi()
			{
				assert(m_ShutdownCalled == true);
				Shutdown();
			}
		};
	}
}

#endif