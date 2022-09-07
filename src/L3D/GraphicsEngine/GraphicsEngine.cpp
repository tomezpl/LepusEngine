#include <cassert>
#include <type_traits>

#include "../GraphicsEngine.h"
#include "Apis.h"

using namespace LepusEngine::Lepus3D;

void GraphicsEngine::InitApi(GraphicsApiOptions* options)
{
	// Check that an API wasn't initialised before. For now, only one API can be initialised at runtime.
	assert(m_Api == nullptr);

	switch (options->GetType())
	{
	case GraphicsApiType::GraphicsApiOpenGL:
		m_Api = new GraphicsApiGL(*reinterpret_cast<GraphicsApiGLOptions*>(options));
		break;
	default:
		// Assert if the API type is not part of the enum.
		assert(false);
		break;
	}
}