#include "Window.h"
#include <gtest/gtest.h>

class WindowTests : public ::testing::Test
{

protected:
	Window window = Window(800, 600, "Test");
};

TEST_F(WindowTests, CheckGLFWInitialisation)
{
	EXPECT_TRUE(window.IsGlfwInit());
	EXPECT_TRUE(window.IsGladInit());
	EXPECT_FALSE(window.GetWindow() == nullptr);
}

TEST_F(WindowTests, CheckIfKeysAreInitToInActive)
{
	for (auto& key : window.keys)
	{
		EXPECT_TRUE( key_bool::key_inactive == key);
	}

	for (auto& key : window.mouseButtons)
	{
		EXPECT_TRUE(key_bool::key_inactive == key);
	}
}