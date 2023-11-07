#include <gtest/gtest.h>
#include <LUtility/Math.h>

class MathTransformTests : public testing::Test {};

TEST(MathTransformTests, TransformIsInitialisedCorrectly)
{
    lepus::math::Transform transform = lepus::math::Transform();

    ASSERT_FLOAT_EQ(transform.Origin().x(), 0.f);
    ASSERT_FLOAT_EQ(transform.Origin().y(), 0.f);
    ASSERT_FLOAT_EQ(transform.Origin().z(), 0.f);

    ASSERT_FLOAT_EQ(transform.Forward().x(), 0.f);
    ASSERT_FLOAT_EQ(transform.Forward().y(), 0.f);
    ASSERT_FLOAT_EQ(transform.Forward().z(), 1.f);

    ASSERT_FLOAT_EQ(transform.Up().x(), 0.f);
    ASSERT_FLOAT_EQ(transform.Up().y(), 1.f);
    ASSERT_FLOAT_EQ(transform.Up().z(), 0.f);

    ASSERT_FLOAT_EQ(transform.Right().x(), 1.f);
    ASSERT_FLOAT_EQ(transform.Right().y(), 0.f);
    ASSERT_FLOAT_EQ(transform.Right().z(), 0.f);
}