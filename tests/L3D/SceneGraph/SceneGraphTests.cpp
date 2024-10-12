#include <gtest/gtest.h>
#include "SceneGraphTests.h"

class SceneGraphTest : public testing::Test
{
};

TEST(SceneGraphTest, SceneGraphIsInitialisedCorrectly)
{
    lepus::gfx::SceneGraph sceneGraph = lepus::gfx::SceneGraph();

    ASSERT_TRUE(sceneGraph.Root()->IsRoot());
    ASSERT_EQ(sceneGraph.Root()->FirstChild(), nullptr);
    ASSERT_EQ(sceneGraph.Root()->NextSibling(), nullptr);
    ASSERT_EQ(sceneGraph.Root()->Parent(), nullptr);
    ASSERT_EQ(sceneGraph.Root()->GetTransformable(), nullptr);
}

TEST(SceneGraphTest, SceneGraphChildIsAddedCorrectly)
{
    lepus::gfx::SceneGraph sceneGraph = lepus::gfx::SceneGraph();
    lepus::math::Transform someTransform = lepus::math::Transform();
    lepus::engine::objects::Mesh someMesh = lepus::engine::objects::Mesh();
    lepus::gfx::Renderable someRenderable = lepus::gfx::Renderable(&someMesh, someTransform);

    sceneGraph.AddChild(&someRenderable);
    ASSERT_EQ(sceneGraph.Root()->FirstChild()->GetTransformable(), &someRenderable);
    ASSERT_EQ(sceneGraph.Root()->NextSibling(), nullptr);
    ASSERT_EQ(sceneGraph.Root()->FirstChild()->FirstChild(), nullptr);
    ASSERT_EQ(sceneGraph.Root()->FirstChild()->NextSibling(), nullptr);
}