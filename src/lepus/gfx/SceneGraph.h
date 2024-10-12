#ifndef LEPUS_GFX_SCENEGRAPH
#define LEPUS_GFX_SCENEGRAPH
#include "SceneGraph/Renderable.h"

namespace lepus
{
    namespace gfx
    {
	class SceneGraph
	{
	    public:
	    typedef SceneNode Node;

	    protected:
	    Node* m_Root;

	    public:
	    SceneGraph()
	    {
		m_Root = new Node();
	    }

	    template <class TransformableType = Transformable>
	    Node* AddChild(const TransformableType* transformable)
	    {
		return m_Root->AddChild((const Transformable*)transformable);
	    }

	    ~SceneGraph()
	    {
		Node* currentNode = m_Root->m_Child;
		Node* parent = m_Root;

		// Delete all children
		while (currentNode)
		{
		    if (currentNode->m_Child)
		    {
			parent = currentNode;
			currentNode = currentNode->m_Child;
		    }
		    else
		    {
			bool wasRoot = currentNode->IsRoot();
			parent->m_Child = currentNode->m_Sibling;
			delete currentNode;
			currentNode = wasRoot ? nullptr : parent;
		    }
		}
	    }

	    [[nodiscard]] const Node* Root() const
	    {
		return m_Root;
	    }
	};
    } // namespace gfx
} // namespace lepus
#endif