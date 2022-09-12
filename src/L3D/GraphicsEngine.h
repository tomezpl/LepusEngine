#ifndef L3D_GRAPHICSENGINE
#define L3D_GRAPHICSENGINE

#include "GraphicsEngine/GraphicsApi.h"
#include <LSystem/Windowing.h>
#include <LUtility/Types/String.h>
#include <LUtility/Types/Viewport.h>

namespace LepusEngine
{
	namespace Lepus3D
	{
		class GraphicsEngine
		{
		protected:
			/// @brief Graphics API wrapper (GL, Vk, D3D).
			GraphicsApi* m_Api;

			/// @brief Windowing interface wrapper. This can be shared by multiple systems, not just graphics,
			/// and implemented through many platform-specific libraries.
			std::shared_ptr<LepusSystem::Windowing> m_Windowing;

			struct {
				LepusUtility::Types::String windowName;
				LepusUtility::Types::Viewport viewport;
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
			GraphicsEngine(GraphicsApiOptions* options, std::shared_ptr<LepusSystem::Windowing> windowing)
			{
				INIT_DEFAULT();

				InitWindowing(windowing);
				InitApi(options);
			}

			/// @brief Assigns a windowing context to use with this GraphicsEngine. This usually needs to be done before InitApi.
			/// @param windowing Created windowing context to use.
			void InitWindowing(std::shared_ptr<LepusSystem::Windowing> windowing);

			/// @brief Initialises the API library and creates an instance of the wrapper for the right API type.
			/// @param options API options used to initialise and create the API library wrapper. These are copied, so it is recommended to create options in stack.
			void InitApi(GraphicsApiOptions* options);

#undef INIT_DEFAULT()

			enum PixelFormat
			{
				RGBA32 = 256
			};

			template<typename T, PixelFormat bits>
			void Render(T r, T g, T b)
			{
				const float max = bits - 1.f;
				Render((float)r / max, (float)g / max, (float)b / max);
			}

			void Render(float r, float g, float b);
		};
	}
}

#endif