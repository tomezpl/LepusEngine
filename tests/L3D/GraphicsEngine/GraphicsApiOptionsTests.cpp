#include <gtest/gtest.h>
#include "GraphicsApiOptionsTests.h"
#include "GraphicsApiTests.h"
#include <array>
#include <climits>

class GraphicsApiOptionsTest : public testing::TestWithParam<GraphicsApiStubOptions>
{
};

std::array<GraphicsApiStubOptions, 3> getTestOptions()
{
	std::array<GraphicsApiStubOptions, 3> testOptions;

	testOptions[0] = {};
	testOptions[0].testValue = 123;

	testOptions[1] = {};
	testOptions[1].testValue = -7458;

	testOptions[2] = {};
	testOptions[2].testValue = LLONG_MAX;

	return testOptions;
}

TEST(GraphicsApiOptionsTest, TypeMatchesApi)
{
	LepusEngine::Lepus3D::GraphicsApiOptions* options = new GraphicsApiStubOptions();

	ASSERT_EQ(options->GetType(), LepusEngine::Lepus3D::GraphicsApiType::GraphicsApiTest);

	delete options;
}

// Checks that the GraphicsApi.GetOptions<T> method returns a valid reference,
// such that modifying its member values results in them persisting when we call GetOptions<T> again.
TEST_P(GraphicsApiOptionsTest, TestValueIsUpdated)
{
	GraphicsApiStubOptions options = GetParam();
	GraphicsApiStubOptions initOptions = {};
	GraphicsApiStub stubApi(&initOptions);

	ASSERT_EQ(0, stubApi.getOptionsTestValue());

	stubApi.setOptionsTestValue(options.testValue);
	ASSERT_EQ(options.testValue, stubApi.getOptionsTestValue());

	stubApi.Shutdown();
}

INSTANTIATE_TEST_SUITE_P
(
	BasicApiOptionsValues,
	GraphicsApiOptionsTest,
	testing::ValuesIn(getTestOptions()), [](const testing::TestParamInfo<GraphicsApiStubOptions>& paramInfo)
	{
		std::string name(std::to_string(paramInfo.index) + "_testValue" + std::to_string(paramInfo.param.testValue));

		// Replace minus signs with 'minus'.
		// Is this horrible code? Yes.
		// Does it work? I think so.
		// Will I ever refactor it? Doubt.
		size_t minusSign = 0;
		while ((minusSign = name.find('-')) != std::string::npos)
		{
			name[minusSign] = 'm';
			name.insert(minusSign + 1, "inus");
		}
		return name;
	}
);
