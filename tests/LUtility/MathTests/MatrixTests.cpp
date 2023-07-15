#include <gtest/gtest.h>
#include <LUtility/Math.h>
#include <iostream>
#include <cmath>

class MathMatrixTests : public testing::Test {};

TEST(MathMatrixTests, MatrixMultipliedByMatrixCorrectly)
{
    lepus::math::Matrix4x4 matA, matB = matA = lepus::math::Matrix4x4();

    /* Matrix A looks like this:

        1   2   3   4
        5   6   7   8
        9   10   11  12
        13  14  15  16

    */
    for (uint8_t i = 0; i < 16; i++) { matA.set(floor((float)i / (float)4), i % 4, float(i + 1)); }

    /* Matrix B looks like this:

        2   3   4   5
        6   7   8   9
        10  11  12  13
        14  15  16  17
    */
    for (uint8_t i = 0; i < 16; i++) { matB.set(floor((float)i / (float)4), i % 4, float(i + 2)); }

    /* Expected results:
        100 110 120 130
        228 254 280 306
        356 398 440 482
        484 542 600 658
    */
    lepus::math::Matrix4x4 expected = lepus::math::Matrix4x4();
    expected.set<0, 0>(100.f); expected.set<0, 1>(110.f); expected.set<0, 2>(120.f); expected.set<0, 3>(130.f);
    expected.set<1, 0>(228.f); expected.set<1, 1>(254.f); expected.set<1, 2>(280.f); expected.set<1, 3>(306.f);
    expected.set<2, 0>(356.f); expected.set<2, 1>(398.f); expected.set<2, 2>(440.f); expected.set<2, 3>(482.f);
    expected.set<3, 0>(484.f); expected.set<3, 1>(542.f); expected.set<3, 2>(600.f); expected.set<3, 3>(658.f);


    lepus::math::Matrix4x4 actual = matA.Multiply(matB);

    // Make sure the original matrices are still correct - they should not be affected by the multiplication, as it's not in-place.
    for (uint8_t i = 0; i < 16; i++) { ASSERT_FLOAT_EQ(matA.get(floor((float)i / (float)4), i % 4), (float)(i + 1)); }
    for (uint8_t i = 0; i < 16; i++) { ASSERT_FLOAT_EQ(matB.get(floor((float)i / (float)4), i % 4), (float)(i + 2)); }

    // Check that the multiplied result is correct.
    for (uint8_t r = 0; r < 4; r++)
    {
        for (uint8_t c = 0; c < 4; c++)
        {
            std::cout << "actual[" << std::to_string(r) << ", " << std::to_string(c) << "]: " << actual.get(r, c) << ", expected[" << std::to_string(r) << ", " << std::to_string(c) << "]: " << expected.get(r, c) << std::endl;
            ASSERT_FLOAT_EQ(actual.get(r, c), expected.get(r, c));
        }
    }
}