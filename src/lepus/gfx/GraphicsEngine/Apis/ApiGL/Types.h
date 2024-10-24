#ifndef L3D_GRAPHICSENGINE_APIS_APIGL_TYPES
#define L3D_GRAPHICSENGINE_APIS_APIGL_TYPES

#include "../../GraphicsApi/BaseBindings.h"

typedef unsigned int GLuint;
typedef int GLint;

namespace lepus
{
    namespace gfx
    {
	template <typename TUniformValue>
	class GLUniformBinding : public lepus::gfx::UniformBinding<GLint, TUniformValue>
	{
	    public:
	    explicit GLUniformBinding(GLint location)
	        : UniformBinding<GLint, TUniformValue>(location)
	    {
	    }

	    const GLint& Location() const override
	    {
		return this->m_Location;
	    }
	};
    } // namespace gfx
} // namespace lepus

#endif