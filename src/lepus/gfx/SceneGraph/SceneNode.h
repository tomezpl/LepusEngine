#ifndef LEPUS_GFX_SCENEGRAPH_SCENENODE
#define LEPUS_GFX_SCENEGRAPH_SCENENODE

#include <lepus/gfx/SceneGraph.h>
#include "Transformable.h"

namespace lepus
{
    namespace gfx
    {
	class SceneNode
	{
	    private:
	    friend class SceneGraph;

	    protected:
	    SceneNode* m_Sibling;
	    SceneNode* m_Child;
	    SceneNode* m_Parent;
	    const Transformable* m_Transformable;

	    public:
	    SceneNode()
	    {
		m_Sibling = nullptr;
		m_Child = nullptr;
		m_Parent = nullptr;
		m_Transformable = nullptr;
	    }

	    ~SceneNode()
	    {
	    }

	    [[nodiscard]] inline const Transformable* GetTransformable() const { return m_Transformable; }
	    [[nodiscard]] inline const SceneNode* NextSibling() const { return m_Sibling; }
	    [[nodiscard]] inline const SceneNode* Parent() const { return m_Parent; }
	    [[nodiscard]] inline const SceneNode* FirstChild() const { return m_Child; }

	    [[nodiscard]] inline bool IsRoot() const { return Parent() == nullptr; }

	    inline const SceneNode* AddChild(const Transformable* transformable)
	    {
		SceneNode* currentNode = m_Child;
		SceneNode* lastChild = m_Child;
		while (currentNode)
		{
		    lastChild = currentNode;
		    currentNode = currentNode->m_Sibling;
		}

		if (!currentNode)
		{
		    currentNode = new SceneNode();
		    currentNode->m_Transformable = transformable;
		    currentNode->m_Parent = this;
		}

		if (!m_Child)
		{
		    m_Child = currentNode;
		}
		else if (lastChild && !lastChild->m_Sibling)
		{
		    lastChild->m_Sibling = currentNode;
		}

		return currentNode;
	    }
	};
    } // namespace gfx
} // namespace lepus

#endif
