#ifndef LEPUS_GFX_SCENEGRAPH_RENDERABLE
#define LEPUS_GFX_SCENEGRAPH_RENDERABLE

#include <lepus/engine/Objects/Mesh.h>
#include <lepus/utility/types/Transform.h>
#include "SceneNode.h"
#include "Transformable.h"

namespace lepus
{
    namespace gfx
    {
	/// @brief A base class for any renderable node that can appear in a scene.
	template <class MeshType = lepus::engine::objects::Mesh>
	class Renderable : public Transformable
	{
	    private:
	    const MeshType* m_Mesh;

	    public:
	    Renderable() = delete;

	    Renderable(const MeshType* mesh, lepus::math::Transform& transform)
	    {
		m_Mesh = mesh;
		m_Transform = &transform;
		m_OwnsTransform = false;
	    }

	    Renderable(const MeshType* mesh, lepus::math::Transform&& transform)
	    {
		m_Mesh = mesh;
		m_Transform = new lepus::math::Transform(transform);
		m_OwnsTransform = true;
	    }

	    ~Renderable()
	    {
		m_Mesh = nullptr;

		if (m_OwnsTransform)
		{
		    delete m_Transform;
		    m_Transform = nullptr;
		}
	    }

	    const MeshType* GetMesh() const
	    {
		return m_Mesh;
	    }
	};
    } // namespace gfx
} // namespace lepus

#endif