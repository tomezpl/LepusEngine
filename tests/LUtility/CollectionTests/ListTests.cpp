#include "lepus/utility/types/List.h"

#include <gtest/gtest.h>

class ListTests : public testing::Test
{
};

TEST(ListTests, IntegersAreAddedToListCorrectly)
{
    lepus::utility::List<uint32_t> integers;
    for (uint32_t n = 1; n <= 10; n++)
    {
	integers.Push(n);
    }

    ASSERT_EQ(integers.Count(), 10);
    ASSERT_GE(integers.Capacity(), 10);

    for (size_t i = 0; i < integers.Count(); i++)
    {
	ASSERT_EQ(integers.Get(i), i + 1);
    }
}

TEST(ListTests, IntegersAreRemovedFromListCorrectly)
{
    lepus::utility::List<uint32_t> integers;
    for (uint32_t n = 1; n <= 10; n++)
    {
	integers.Push(n);
    }

    ASSERT_EQ(integers.Count(), 10);
    ASSERT_EQ(integers.Get(4), 5);
    integers.Remove(4);
    ASSERT_EQ(integers.Count(), 9);
    for (size_t i = 0; i < 4; i++)
    {
	ASSERT_EQ(integers.Get(i), i + 1);
    }
    ASSERT_EQ(integers.Get(4), 6);
    for (size_t i = 5; i < integers.Count(); i++)
    {
	ASSERT_EQ(integers.Get(i), i + 2);
    }
}

TEST(ListTests, IntegersCanBePushedAfterRemoving)
{
    lepus::utility::List<uint32_t> integers;
    for (uint32_t n = 1; n <= 10; n++)
    {
	integers.Push(n);
    }

    ASSERT_EQ(integers.Count(), 10);
    ASSERT_EQ(integers.Get(4), 5);
    integers.Remove(4);
    ASSERT_EQ(integers.Count(), 9);
    for (size_t i = 0; i < 4; i++)
    {
	ASSERT_EQ(integers.Get(i), i + 1);
    }
    ASSERT_EQ(integers.Get(4), 6);
    for (size_t i = 5; i < integers.Count(); i++)
    {
	ASSERT_EQ(integers.Get(i), i + 2);
    }

    integers.Push(42);
    ASSERT_EQ(integers.Count(), 10);
    ASSERT_EQ(integers.Get(8), 10);
    ASSERT_EQ(integers.Get(9), 42);
}