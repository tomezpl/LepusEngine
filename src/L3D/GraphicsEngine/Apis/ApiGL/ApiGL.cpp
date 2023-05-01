#include "../ApiGL.h"
#include <GL/gl3w.h>

using namespace LepusEngine::Lepus3D;

void GraphicsApiGL::Init(GraphicsApiOptions* options)
{
	InitInternal<GraphicsApiGLOptions>(reinterpret_cast<GraphicsApiGLOptions*>(options));
}

void GraphicsApiGL::ClearFrameBuffer(float r, float g, float b)
{
	glClearColor(r, g, b, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GraphicsApiGL::Shutdown()
{
	GraphicsApi::Shutdown();
}