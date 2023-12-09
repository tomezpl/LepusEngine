#include <gtest/gtest.h>
#include <array>
#include <string>
#include "GraphicsApiTests.h"

class GraphicsApiTest : public testing::Test
{
};

TEST(GraphicsApiTest, ShutdownGetsCalled)
{
	GraphicsApiStubOptions options = GraphicsApiStubOptions();
	bool shutdownWasCalled = false;

	lepus::gfx::GraphicsApi* api = new GraphicsApiStub(&options, &shutdownWasCalled);

	api->Shutdown();

	ASSERT_TRUE(shutdownWasCalled);
}