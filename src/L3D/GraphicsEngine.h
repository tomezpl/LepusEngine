#ifndef L3D_GRAPHICSENGINE
#define L3D_GRAPHICSENGINE

#include "GraphicsEngine/GraphicsApi.h"
#include <LUtility/Types/String.h>
#include <LUtility/Types/Viewport.h>

namespace LepusEngine
{
	namespace Lepus3D
	{
		class GraphicsEngine
		{
		protected:
			GraphicsApi* m_Api;

			struct {
				LepusUtility::Types::String windowName;
				LepusUtility::Types::Viewport viewport;
			} m_OutputInfo;
		public:
			GraphicsEngine()
			{
				m_Api = nullptr;
				m_OutputInfo = {};
			}

			GraphicsEngine(GraphicsApiOptions* options)
			{
				m_Api = nullptr;
				InitApi(options);
			}

			void InitApi(GraphicsApiOptions* options);
		};
	}
}

#endif