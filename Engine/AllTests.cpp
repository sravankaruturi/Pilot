#include "FolderLocations.h"
#include "Window.h"
#include "GLShader.h"
#include <gtest/gtest.h>
#include "Texture.h"

class AllTests : public ::testing::Test
{
	
protected:

	// The window is needed to initiate OpenGL Calls used in the Shaders.
	Window window = Window(800, 600, "Test");

	std::string test_vert_file_1 = SHADER_FOLDER + std::string("failing_test.vert");
	std::string test_frag_file_1 = SHADER_FOLDER + std::string("failing_test.frag");

	std::string test_vert_file_2 = SHADER_FOLDER + std::string("good_test.vert");
	std::string test_frag_file_2 = SHADER_FOLDER + std::string("good_test.frag");

	GLShader wrong_shader = GLShader(test_vert_file_1.c_str(), test_frag_file_1.c_str());
	GLShader right_shader = GLShader(test_vert_file_2.c_str(), test_frag_file_2.c_str());

	const std::string awesomefacetexturepath = TEXTURE_FOLDER + std::string("awesomeface.png");
	piolot::Texture texture = piolot::Texture(awesomefacetexturepath);

};

TEST_F(AllTests, CheckGLFWInitialisation)
{
	EXPECT_TRUE(window.IsGlfwInit());
	EXPECT_TRUE(window.IsGladInit());
	EXPECT_FALSE(window.GetWindow() == nullptr);
}

TEST_F(AllTests, CheckIfKeysAreInitToInActive)
{
	for (auto& key : window.keys)
	{
		EXPECT_TRUE(key_bool::key_inactive == key);
	}

	for (auto& key : window.mouseButtons)
	{
		EXPECT_TRUE(key_bool::key_inactive == key);
	}
}

TEST_F(AllTests, CompileErrorsShowing)
{
	EXPECT_FALSE(wrong_shader.compileStatus);
	EXPECT_TRUE(right_shader.compileStatus);
}

TEST_F(AllTests, TextureFileBeingRead)
{
	EXPECT_TRUE(texture.IsLoaded());
}
