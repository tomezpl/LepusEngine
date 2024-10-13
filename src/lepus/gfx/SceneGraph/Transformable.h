#ifndef LEPUS_GFX_SCENEGRAPH_TRANSFORMABLE
#define LEPUS_GFX_SCENEGRAPH_TRANSFORMABLE

#include <lepus/utility/types/Transform.h>

namespace lepus
{
    namespace gfx
    {
	class SceneNode;

	/// @brief A base class for any node that has a transform in a scene.
	class Transformable
	{
	    protected:
	    lepus::math::Transform* m_Transform;
	    bool m_OwnsTransform;

	    public:
	    Transformable()
	    {
		m_Transform = new lepus::math::Transform();
		m_OwnsTransform = true;
	    }

	    [[nodiscard]] lepus::math::Transform* GetTransform() const
	    {
		return m_Transform;
	    }

	    /// @brief Constructs a world transform matrix for this Transformable by traversing up the scene hierarchy.
	    [[nodiscard]] lepus::math::Matrix4x4 GetWorldMatrix(const lepus::gfx::SceneNode* node) const
	    {
		auto ownTransform = this->GetTransform();

		// If this is already the root of the scene subtree then this is our world matrix.
		if (!node->Parent() || node->Parent()->IsRoot())
		{
		    return ownTransform->BuildMatrix();
		}

		// Double work but this should be lighter than using a vector for this.
		// We're traversing up the tree from direct parent to root, and counting how deep in the hierarchy we are.
		auto* root = const_cast<SceneNode*>(node);
		const SceneNode** leaves = nullptr;
		uint8_t depth = 1;
		while (root->Parent() != nullptr && !root->Parent()->IsRoot())
		{
		    root = const_cast<SceneNode*>(root->Parent());
		    depth++;
		}

		leaves = new const SceneNode*[depth];
		leaves[depth - 1] = node;
		auto currentLeaf = const_cast<SceneNode*>(node);
		for (int i = (int)depth - 2; depth > 1 && i >= 0; i--)
		{
		    currentLeaf = const_cast<SceneNode*>(node->Parent());
		    leaves[i] = currentLeaf;

		    if (i == 0)
		    {
			break;
		    }
		}

		// Accumulate position and rotation
		lepus::types::Vector3 accPos(0.f, 0.f, 0.f);
		lepus::types::Quaternion accRot = this->m_Transform->Rotation();
		lepus::types::Vector3 accScale = this->m_Transform->Scale();

		for (uint8_t i = 1; i < depth; i++)
		{
		    lepus::types::Vector4 rotated(leaves[i]->GetTransformable()->GetTransform()->Origin());
		    rotated.w(1.f);

		    auto parentTransform = leaves[i - 1]->GetTransformable()->GetTransform();
		    lepus::math::Matrix4x4 mat = parentTransform->BuildMatrix();

		    accRot = accRot * (parentTransform->Rotation());
		    rotated = mat.Multiply(rotated);

		    accPos.x(accPos.x() + rotated.x());
		    accPos.y(accPos.y() + rotated.y());
		    accPos.z(accPos.z() + rotated.z());

		    accScale.Multiply(parentTransform->Scale());
		}

		lepus::math::Transform worldTransform = lepus::math::Transform();

		worldTransform.Origin(accPos);
		worldTransform.Rotate(accRot);
		worldTransform.SetScale(accScale.x(), accScale.y(), accScale.z());

		if (leaves)
		{
		    delete[] leaves;
		}

		return worldTransform.BuildMatrix();
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
