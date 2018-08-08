#include "Window.h"
#include <gtest/gtest.h>

class WindowTests : public ::testing::Test
{

protected:
	Window window = Window(800, 600, "Test");
};

TEST_F(WindowTests, CheckInitialization)
{
	EXPECT_EQ(window.GetWidth(), 800);
	EXPECT_EQ(window.GetHeight(), 600);
}
