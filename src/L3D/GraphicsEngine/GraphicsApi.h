#ifndef L3D_GRAPHICSENGINE_GRAPHICSAPI
#define L3D_GRAPHICSENGINE_GRAPHICSAPI

#include <LUtility/Types/Viewport.h>
#include <memory>

namespace LepusEngine
{
	namespace Lepus3D
	{
		enum GraphicsApiType
		{
			GraphicsApiUnknown = 0,
			GraphicsApiOpenGL,
			GraphicsApiVulkan
		};

		struct GraphicsApiOptions
		{
			// Type of API to instantiate.
			GraphicsApiType type = GraphicsApiType::GraphicsApiUnknown;

			// Main viewport used by the application.
			LepusUtility::Types::Viewport mainViewport = {};
		};

		class GraphicsApi
		{
		private:
			GraphicsApiOptions* m_Options;
		protected:
			template <class TGraphicsApiOptions>
			void InitInternal(TGraphicsApiOptions* options)
			{
				const size_t optionsSz = sizeof(TGraphicsApiOptions);
				m_Options = new TGraphicsApiOptions();
				memcpy(m_Options, options, optionsSz);
			}

		public:
			virtual void Init(GraphicsApiOptions* options) = 0;

			virtual void Shutdown() = 0;

			virtual ~GraphicsApi()
			{
				Shutdown();
			}
		};
	}
}

#endif