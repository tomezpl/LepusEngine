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

/// @brief Test world transforms in a scene graph.
///
/// This scene sets up transform A at (0, 0, -2), B at (2, 0, 0) relative to A, C at (-3, 0, -3) relative to B, D at (0, 0, 6) relative to C.
/// B has a scale of (1.0/1.5). A is rotated counter-clockwise around the Y axis by 90 degrees.
/// A's anchor point is its center of mass so it remains in the same spot after the rotation.
/// B ends up at (0, 0, 0) in world space.
/// C's local coords are scaled by (1.0/1.5), and are therefore (-2, 0, -2) away from B in world units, and (0, 0, -2) away fom A in world units.
/// Therefore, C should end up at (2, 0, -2) in world space, effectively taking B's original world-space position.
/// C is also rotated counter-clockwise around the Y axis by 50 degrees. Before applying A's rotation, D ends up at approx. (-3.1, 0, -0.55) away from A in world units.
/// Once A's 90deg rotation is applied then, D should end up at approx (-0.55, 0, -5.06) in world space.
///
/// (it may be helpful to draw this on a piece of paper)
TEST(SceneGraphTest, SceneGraphChildTransformsCreateCorrectWorldCoords)
{
    lepus::gfx::SceneGraph sceneGraph = lepus::gfx::SceneGraph();
    lepus::gfx::Transformable rootTransformable = lepus::gfx::Transformable();
    lepus::gfx::Transformable childTransformable1 = lepus::gfx::Transformable();
    lepus::gfx::Transformable childTransformable2 = lepus::gfx::Transformable();
    lepus::gfx::Transformable childTransformable3 = lepus::gfx::Transformable();

    rootTransformable.GetTransform()->Origin(lepus::types::Vector3(0.f, 0.f, -2.f));
    childTransformable1.GetTransform()->Origin(lepus::types::Vector3(2.f, 0.f, 0.f));
    childTransformable2.GetTransform()->Origin(lepus::types::Vector3(-3.f, 0.f, -3.f));
    childTransformable3.GetTransform()->Origin(lepus::types::Vector3(0.f, 0.f, 6.f));

    auto rootNode = sceneGraph.AddChild(&rootTransformable);
    auto childNode1 = rootNode->AddChild(&childTransformable1);
    auto childNode2 = childNode1->AddChild(&childTransformable2);
    auto childNode3 = childNode2->AddChild(&childTransformable3);

    const float nodeCTheta = -50.f;

    rootTransformable.GetTransform()->Rotate(lepus::types::Quaternion(0.f, 1.f, 0.f, (float)PI * -0.5f));
    childTransformable1.GetTransform()->SetScale(1.f / 1.5f);
    childTransformable2.GetTransform()->Rotate(lepus::types::Quaternion(0.f, 1.f, 0.f, (float)PI * (nodeCTheta / 180.f)));

    auto childNode3WorldMat = childTransformable3.GetWorldMatrix(childNode3);
    lepus::types::Vector3 childNode3World(childNode3WorldMat.get(0, 3), childNode3WorldMat.get(1, 3), childNode3WorldMat.get(2, 3));

    auto childNode2WorldMat = childTransformable2.GetWorldMatrix(childNode2);
    lepus::types::Vector3 childNode2World(childNode2WorldMat.get(0, 3), childNode2WorldMat.get(1, 3), childNode2WorldMat.get(2, 3));

    auto childNode1WorldMat = childTransformable1.GetWorldMatrix(childNode1);
    lepus::types::Vector3 childNode1World(childNode1WorldMat.get(0, 3), childNode1WorldMat.get(1, 3), childNode1WorldMat.get(2, 3));

    auto finalRootPos = rootTransformable.GetTransform()->Origin();
    ASSERT_EQ(finalRootPos.x(), 0.f);
    ASSERT_EQ(finalRootPos.y(), 0.f);
    ASSERT_EQ(finalRootPos.z(), -2.f);

    float cn1X = childNode1World.x(), cn1Y = childNode1World.y(), cn1Z = childNode1World.z();
    ASSERT_NEAR(cn1X, 0.f, 0.0001f);
    ASSERT_NEAR(cn1Y, 0.f, 0.0001f);
    ASSERT_NEAR(cn1Z, 0.f, 0.0001f);

    float cn2X = childNode2World.x(), cn2Y = childNode2World.y(), cn2Z = childNode2World.z();
    ASSERT_NEAR(cn2X, 2.f, 0.0001f);
    ASSERT_NEAR(cn2Y, 0.f, 0.0001f);
    ASSERT_NEAR(cn2Z, -2.f, 0.0001f);

    float cn3X = childNode3World.x(), cn3Y = childNode3World.y(), cn3Z = childNode3World.z();
    float cn3PreRotX = 0.f + 4.f * sinf((float)PI * (nodeCTheta / 180.f));
    float cn3PreRotZ = -4.f + 4.f * cosf((float)PI * (nodeCTheta / 180.f));
    ASSERT_NEAR(cn3X, 0.f - (cn3PreRotZ - -2.f), 0.03f);
    ASSERT_NEAR(cn3Y, 0.f, 0.0001f);
    ASSERT_NEAR(cn3Z, -2.f + (cn3PreRotX - 0.f), 0.01f);
}