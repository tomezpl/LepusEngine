#include <gtest/gtest.h>
#include <lepus/utility/Math.h>
#include <cmath>

class MathVectorTests : public testing::Test
{
};

TEST(MathVectorTests, Vector2InitialisedCorrectly)
{
    lepus::types::Vector2 a;

    ASSERT_FLOAT_EQ(a.x(), 0.f);
    ASSERT_FLOAT_EQ(a.y(), 0.f);
}

TEST(MathVectorTests, Vector3InitialisedCorrectly)
{
    lepus::types::Vector3 a = lepus::types::Vector3();

    ASSERT_FLOAT_EQ(a.x(), 0.f);
    ASSERT_FLOAT_EQ(a.y(), 0.f);
    ASSERT_FLOAT_EQ(a.z(), 0.f);
}

TEST(MathVectorTests, Vector4InitialisedCorrectly)
{
    lepus::types::Vector4 a;

    ASSERT_FLOAT_EQ(a.x(), 0.f);
    ASSERT_FLOAT_EQ(a.y(), 0.f);
    ASSERT_FLOAT_EQ(a.z(), 0.f);
    ASSERT_FLOAT_EQ(a.w(), 0.f);
}

TEST(MathVectorTests, Vector4InitialisedFromVector3Correctly)
{
    lepus::types::Vector4 a = lepus::types::Vector4(lepus::types::Vector3(2.f, 3.f, 4.f));
    ASSERT_FLOAT_EQ(a.x(), 2.f);
    ASSERT_FLOAT_EQ(a.y(), 3.f);
    ASSERT_FLOAT_EQ(a.z(), 4.f);
    ASSERT_FLOAT_EQ(a.w(), 1.f);
}

// TODO: Constrain template to Vector classes?
template <class TVector>
struct VectorOperationTestData
{
    struct { TVector a, b, expected; } add, subtract, multiply;
};

template <class TVector>
class MathVectorOperationsTests : public testing::Test
{
    public:
    VectorOperationTestData<TVector> _getTestData();
};

typedef lepus::types::Vector2 Vector2;
typedef lepus::types::Vector3 Vector3;
typedef lepus::types::Vector4 Vector4;

template<>
VectorOperationTestData<Vector2> MathVectorOperationsTests<Vector2>::_getTestData()
{
    return {
        { Vector2(7.f, 8.f), Vector2(13.f, 22.f), Vector2(20.f, 30.f)}, // add
        { Vector2(1.f, 8.f), Vector2(3.f, 5.f), Vector2(-2.f, 3.f)}, // subtract
        { Vector2(7.f, 8.f), Vector2(2.f, 2.f), Vector2(14.f, 16.f)} // multiply 
    };
}

template<>
VectorOperationTestData<Vector3> MathVectorOperationsTests<Vector3>::_getTestData()
{
    return {
        { Vector3(4.f, 50.f, 9.f), Vector3(11.f, 25.f, 1.f), Vector3(15.f, 75.f, 10.f)}, // add
        { Vector3(2.f, 4.f, 0.f), Vector3(0.f, 1.f, -1.f), Vector3(2.f, 3.f, 1.f)}, // subtract
        { Vector3(62.f, 5.f, 10.f), Vector3(0.25f, 3.f, 0.f), Vector3(15.5f, 15.f, 0.f)} // multiply 
    };
}

template<>
VectorOperationTestData<Vector4> MathVectorOperationsTests<Vector4>::_getTestData()
{
    return {
        { Vector4(17.f, 200.f, 1.f, 1000.f), Vector4(0.1f, 2550.f, -1.f, 10000.f), Vector4(17.1f, 2750.f, 0.f, 11000.f)}, // add
        { Vector4(23.f, 34.f, 0.f, 150.f), Vector4(3.f, 14.f, 20.f, 300.f), Vector4(20.f, 20.f, -20.f, -150.f)}, // subtract
        { Vector4(37.f, 51.f, 0.f, 1.f), Vector4(5.f, 0.5f, 100000.f, 3.14f), Vector4(185.f, 25.5f, 0.f, 3.14f)} // multiply 
    };
}

TYPED_TEST_SUITE_P(MathVectorOperationsTests);

TYPED_TEST_P(MathVectorOperationsTests, VectorsAreAddedCorrectly)
{
    VectorOperationTestData<TypeParam> testData = this->_getTestData();
    TypeParam out = testData.add.a + testData.add.b;
    ASSERT_EQ(out, testData.add.expected);
}

TYPED_TEST_P(MathVectorOperationsTests, VectorsAreSubtractedCorrectly)
{
    VectorOperationTestData<TypeParam> testData = this->_getTestData();
    TypeParam out = testData.subtract.a - testData.subtract.b;
    ASSERT_EQ(out, testData.subtract.expected);
}

TYPED_TEST_P(MathVectorOperationsTests, VectorsAreMultipliedCorrectly)
{
    VectorOperationTestData<TypeParam> testData = this->_getTestData();
    TypeParam out = testData.multiply.a * testData.multiply.b;
    ASSERT_EQ(out, testData.multiply.expected);
}

TEST(MathVectorOperationsTests, Vector3CrossIsCorrect)
{
    Vector3 forward = Vector3(0.0f, 0.0f, -1.f), right = Vector3(1.f, 0.0f, 0.0f), up = Vector3(0.0f, 1.0f, 0.0f);

    Vector3 cross = Vector3::Cross(right, forward);
    std::cout << cross.ToString().c_str() << "\n";
    ASSERT_EQ(cross, up);
    cross = Vector3::Cross(forward, up);
    std::cout << cross.ToString().c_str() << "\n";
    ASSERT_EQ(cross, right);
    cross = Vector3::Cross(up, right);
    std::cout << cross.ToString().c_str() << "\n";
    ASSERT_EQ(cross, forward);
}

REGISTER_TYPED_TEST_SUITE_P(MathVectorOperationsTests, VectorsAreAddedCorrectly, VectorsAreSubtractedCorrectly, VectorsAreMultipliedCorrectly);

using VectorTypes = testing::Types<Vector2, Vector3, Vector4>;
INSTANTIATE_TYPED_TEST_SUITE_P(VectorTypeTests, MathVectorOperationsTests, VectorTypes);