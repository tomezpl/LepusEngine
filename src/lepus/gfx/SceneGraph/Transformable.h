#ifndef LEPUS_GFX_SCENEGRAPH_TRANSFORMABLE
#define LEPUS_GFX_SCENEGRAPH_TRANSFORMABLE

#include <lepus/utility/types/Transform.h>

namespace lepus
{
    namespace gfx
    {
	class Transformable
	{
	    protected:
	    const lepus::math::Transform* m_Transform;
	    bool m_OwnsTransform;

	    public:
	    Transformable()
	    {
		m_Transform = new lepus::math::Transform();
		m_OwnsTransform = true;
	    }

	    ~Transformable()
	    {
		if (m_OwnsTransform)
		{
		    delete m_Transform;
		    m_Transform = nullptr;
		}
	    }
	};
    } // namespace gfx
} // namespace lepus

#endif
