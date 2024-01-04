#ifndef LEPUS_GFX_SCENEGRAPH
#define LEPUS_GFX_SCENEGRAPH
#include "SceneGraph/Renderable.h"

namespace lepus
{
    namespace gfx
    {
        template<class MeshType = lepus::engine::objects::Mesh, class RenderableType = lepus::gfx::Renderable<MeshType>>
        class SceneGraph
        {
            public:

            class Node
            {
                private:
                friend class SceneGraph;

                protected:
                Node* m_Sibling;
                Node* m_Child;
                Node* m_Parent;
                const RenderableType* m_Renderable;

                public:
                Node()
                {
                    m_Sibling = nullptr;
                    m_Child = nullptr;
                    m_Parent = nullptr;
                    m_Renderable = nullptr;
                }

                ~Node()
                {

                }

                inline const RenderableType* GetRenderable() const { return m_Renderable; }
                inline const Node* NextSibling() const { return m_Sibling; }
                inline const Node* Parent() const { return m_Parent; }
                inline const Node* FirstChild() const { return m_Child; }

                inline bool IsRoot() const { return Parent() == nullptr; }

                inline const Node* AddChild(const RenderableType* renderable)
                {
                    Node* currentNode = m_Child;
                    Node* lastChild = m_Child;
                    while (currentNode)
                    {
                        lastChild = currentNode;
                        currentNode = currentNode->m_Sibling;
                    }

                    if (!currentNode)
                    {
                        currentNode = new Node();
                        currentNode->m_Renderable = renderable;
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

            protected:
            Node* m_Root;

            public:

            SceneGraph()
            {
                m_Root = new Node();
            }

            const Node* AddChild(const RenderableType* renderable)
            {
                return m_Root->AddChild(renderable);
            }

            ~SceneGraph()
            {
                Node* currentNode = m_Root->m_Child;
                Node* parent = currentNode;

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
                        parent->m_Child = currentNode->m_Sibling;
                        delete currentNode;
                        currentNode = parent;
                    }
                }
            }

            const Node* Root() const
            {
                return m_Root;
            }
        };
    }
}
#endif