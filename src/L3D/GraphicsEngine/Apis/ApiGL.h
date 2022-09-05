#ifndef L3D_GRAPHICSENGINE_APIS_APIGL
#define L3D_GRAPHICSENGINE_APIS_APIGL

#include "../GraphicsApi.h"

namespace LepusEngine
{
	namespace Lepus3D
	{
		struct GraphicsApiGLOptions : GraphicsApiOptions
		{
			GraphicsApiType type = GraphicsApiOpenGL;
		};

		class GraphicsApiGL : public GraphicsApi
		{
		public:
			GraphicsApiGL(GraphicsApiGLOptions options)
			{
				Init(&options);
			}

			void Init(GraphicsApiOptions* options) override;
			void Shutdown() override;
		};
	}
}

#endif