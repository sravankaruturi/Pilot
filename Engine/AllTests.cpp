#include "AssetManager.h"
#include "Window.h"

#include <gtest/gtest.h>


class AllTests : public ::testing::Test
{
	
protected:

	// The window is needed to initiate OpenGL Calls used in the Shaders.
	Window window = Window(800, 600, "Test");

	const std::string awesomefacetexturepath = TEXTURE_FOLDER + std::string("awesomeface.png");
	piolot::Texture texture = piolot::Texture(awesomefacetexturepath);

	piolot::AssetManager asmngr;

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

	std::string test_vert_file_1 = SHADER_FOLDER + std::string("failing_test.vert");
	std::string test_frag_file_1 = SHADER_FOLDER + std::string("failing_test.frag");

	std::string test_vert_file_2 = SHADER_FOLDER + std::string("good_test.vert");
	std::string test_frag_file_2 = SHADER_FOLDER + std::string("good_test.frag");

	GLShader wrong_shader = GLShader(test_vert_file_1.c_str(), test_frag_file_1.c_str());
	GLShader right_shader = GLShader(test_vert_file_2.c_str(), test_frag_file_2.c_str());

	EXPECT_FALSE(wrong_shader.compileStatus);
	EXPECT_TRUE(right_shader.compileStatus);
}

TEST_F(AllTests, TextureFileBeingRead)
{
	EXPECT_TRUE(texture.IsLoaded());
}

TEST_F(AllTests, AssetManagerShaderLoading)
{
	asmngr.LoadShaders();

	EXPECT_TRUE(asmngr.shaders.find("good_test") != asmngr.shaders.end());
	EXPECT_TRUE(asmngr.shaders.find("failing_test") != asmngr.shaders.end());
}

TEST_F(AllTests, AssetManagerTextureLoading)
{

	EXPECT_FALSE(asmngr.IsTextureLoaded("awesomeface"));

	asmngr.LoadTextures();

	EXPECT_TRUE(asmngr.textures.find("awesomeface") != asmngr.textures.end());
	EXPECT_TRUE(asmngr.IsTextureLoaded("awesomeface"));

	// Intentionally spelt wrong.
	EXPECT_FALSE(asmngr.IsTextureLoaded("awesomefaec"));

	// Try adding texture to the same thing. It should return false.
	EXPECT_FALSE(asmngr.AddToTextures("awesomeface", &texture));
}