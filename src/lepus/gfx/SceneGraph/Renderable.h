#ifndef LEPUS_GFX_SCENEGRAPH_RENDERABLE
#define LEPUS_GFX_SCENEGRAPH_RENDERABLE

#include <lepus/engine/Objects/Mesh.h>
#include <lepus/utility/types/Transform.h>
#include "SceneNode.h"
#include "Transformable.h"
#include "lepus/gfx/Material.h"

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
	    const Material* m_Material;
	    bool m_OwnsMaterial;

	    public:
	    Renderable() = delete;

	    Renderable(const MeshType* mesh, lepus::math::Transform& transform, bool createMaterial = true)
	    {
		m_Mesh = mesh;
		m_Transform = &transform;
		m_OwnsTransform = false;

		if (createMaterial)
		{
		    m_Material = new Material();
		    m_OwnsMaterial = true;
		}
	    }

	    Renderable(const MeshType* mesh, lepus::math::Transform& transform, const Material& material)
	        : Renderable(mesh, transform, false)
	    {
		m_Material = &material;
		m_OwnsMaterial = false;
	    }

	    Renderable(const MeshType* mesh, lepus::math::Transform&& transform, bool createMaterial = true)
	    {
		m_Mesh = mesh;
		m_Transform = new lepus::math::Transform(transform);
		m_OwnsTransform = true;

		if (createMaterial)
		{
		    m_Material = new Material();
		    m_OwnsMaterial = true;
		}
	    }

	    Renderable(const MeshType* mesh, lepus::math::Transform&& transform, const Material& material)
	        : Renderable(mesh, transform, false)
	    {
		m_Material = &material;
		m_OwnsMaterial = false;
	    }

	    ~Renderable()
	    {
		m_Mesh = nullptr;

		if (m_OwnsTransform)
		{
		    delete m_Transform;
		    m_Transform = nullptr;
		}

		if (m_OwnsMaterial)
		{
		    delete m_Material;
		    m_Material = nullptr;
		}
	    }

	    [[nodiscard]] const MeshType* GetMesh() const
	    {
		return m_Mesh;
	    }

	    [[nodiscard]] const Material* GetMaterial() const
	    {
		return m_Material;
	    }
	};
    } // namespace gfx
} // namespace lepus

#endif