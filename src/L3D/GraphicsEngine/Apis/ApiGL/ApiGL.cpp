#include "../ApiGL.h"

using namespace LepusEngine::Lepus3D;

void GraphicsApiGL::Init(GraphicsApiOptions* options)
{
	InitInternal<GraphicsApiGLOptions>(reinterpret_cast<GraphicsApiGLOptions*>(options));
}

void GraphicsApiGL::Shutdown()
{

}