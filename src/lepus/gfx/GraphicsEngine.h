#ifndef L3D_GRAPHICSENGINE
#define L3D_GRAPHICSENGINE

#include "GraphicsEngine/GraphicsApi.h"
#include <lepus/system/Windowing.h>
#include <lepus/utility/types/String.h>
#include <lepus/utility/types/Viewport.h>

namespace lepus
{
	namespace gfx
	{
		class GraphicsEngine
		{
			protected:
			/// @brief Graphics API wrapper (GL, Vk, D3D).
			GraphicsApi* m_Api;

			/// @brief Windowing interface wrapper. This can be shared by multiple systems, not just graphics,
			/// and implemented through many platform-specific libraries.
			std::shared_ptr<lepus::system::Windowing> m_Windowing;

			struct
			{
				lepus::types::String windowName;
				lepus::types::Viewport viewport;
			} m_OutputInfo;
			public:
#define INIT_DEFAULT() \
m_Api = nullptr; \
m_Windowing = nullptr; \
m_OutputInfo = {}; \

			/// @brief Creates a default GraphicsEngine. InitWindowing and InitApi need to be called manually.
			GraphicsEngine()
			{
				INIT_DEFAULT();
			}

			/// @brief Creates a GraphicsEngine using the provided API options and a windowing context.
			/// @param options API options. This will be used to create a GraphicsApi instance of the right type. The engine has ownership over the created API wrapper.
			/// @param windowing Windowing context containing a window handle, dimensions, image format, etc.
			GraphicsEngine(GraphicsApiOptions* options, std::shared_ptr<lepus::system::Windowing> windowing)
			{
				INIT_DEFAULT();

				InitWindowing(windowing);
				InitApi(options);
			}

			/// @brief Assigns a windowing context to use with this GraphicsEngine. This usually needs to be done before InitApi.
			/// @param windowing Created windowing context to use.
			void InitWindowing(std::shared_ptr<lepus::system::Windowing> windowing);

			/// @brief Initialises the API library and creates an instance of the wrapper for the right API type.
			/// @param options API options used to initialise and create the API library wrapper. These are copied, so it is recommended to create options in stack.
			void InitApi(GraphicsApiOptions* options);

#undef INIT_DEFAULT

			enum PixelFormat
			{
				RGBA32 = 256
			};

			template <class TGraphicsApi>
			inline TGraphicsApi& GetApi()
			{
				return *((TGraphicsApi*)m_Api);
			}

			void Setup();

			/// @brief Renders the scene and performs buffer swap to display the results in the window.
			/// @tparam T Numerical type used for the solid clear colour.
			/// @tparam bits Number of allowed integer values for a single colour channel in a pixel format, e.g. 256 for RGBA32
			/// @param r Solid clear colour value (red)
			/// @param g Solid clear colour value (green)
			/// @param b Solid clear colour value (blue)
			template<typename T, PixelFormat bits>
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
		};
	}
}

#endif