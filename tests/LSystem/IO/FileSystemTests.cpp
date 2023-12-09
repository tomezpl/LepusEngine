#include <gtest/gtest.h>
#include <array>
#include <string>
#include "FileSystemTests.h"
#include <cstdlib>

class FileSystemTests : public testing::Test
{
};

TEST(FileSystemTests, TextFileIsReadCorrectly)
{
	std::string contents = lepus::system::FileSystem::Read("./tests/Content/SampleFile.txt");

	ASSERT_STREQ(contents.c_str(), "hello world");
}