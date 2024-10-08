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
	template <class MeshType = lepus::engine::objects::Mesh>
	class Renderable : protected Transformable
	{
	    private:
	    const MeshType* m_Mesh;

	    public:
	    Renderable() = delete;

	    Renderable(const MeshType* mesh, const lepus::math::Transform& transform)
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

	    [[nodiscard]] lepus::math::Transform& GetTransform() const
	    {
		return const_cast<lepus::math::Transform&>(*reinterpret_cast<const lepus::math::Transform*>(m_Transform));
	    }

	    /// @brief Constructs a world transform for this Renderable by traversing up the scene hierarchy.
	    [[nodiscard]] lepus::math::Transform GetWorldTransform(const lepus::gfx::SceneNode* parent) const
	    {
		auto ownTransform = this->GetTransform();

		lepus::math::Transform worldTransform = lepus::math::Transform();

		return worldTransform;
	    }
	};
    } // namespace gfx
} // namespace lepus

#endif