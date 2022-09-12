#ifndef L3D_GRAPHICSENGINE_APIS_APIGL
#define L3D_GRAPHICSENGINE_APIS_APIGL

#include "../GraphicsApi.h"

namespace LepusEngine
{
	namespace Lepus3D
	{
		class GraphicsApiGLOptions : public GraphicsApiOptions
		{
		public:
			GraphicsApiType GetType() override { return GraphicsApiOpenGL; }
		};

		class GraphicsApiGL : public GraphicsApi
		{
		public:
			GraphicsApiGL(GraphicsApiGLOptions options)
			{
				Init(&options);
			}

			void Init(GraphicsApiOptions* options) override;

			void ClearFrameBuffer(float r, float g, float b) override;
			
			/// @brief Dummy method as OpenGL itself doesn't need to do anything for the swap chain to work.
			void SwapBuffers() override {}

			void Shutdown() override;
		};
	}
}

#endif