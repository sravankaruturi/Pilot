#include "AssetManager.h"
#include "Window.h"
#include <gtest/gtest.h>
#include "Mesh.h"

using namespace piolot;

class AllTests : public ::testing::Test
{
	
protected:

	// The window is needed to initiate OpenGL Calls used in the Shaders.
	Window window = Window(800, 600, "Test");

	const std::string awesomefacetexturepath = TEXTURE_FOLDER + std::string("awesomeface.png");

	Texture awesomeFaceTexture = Texture(awesomefacetexturepath);

	std::string test_vert_file_2 = SHADER_FOLDER + std::string("good_test.vert");
	std::string test_frag_file_2 = SHADER_FOLDER + std::string("good_test.frag");

	GLShader right_shader = GLShader(test_vert_file_2.c_str(), test_frag_file_2.c_str());

	AssetManager asmngr;

public:

	struct VertexDataTestGood
	{
		glm::vec3 position;
		glm::vec3 colour;
		glm::vec3 texCoord;	// Note all the things have to be a vec3.

		VertexDataTestGood(glm::vec3 _position, glm::vec3 _colour, glm::vec3 _texCoord)
			: position(_position), colour(_colour), texCoord(_texCoord)
		{
			
		}

	};

	std::vector<VertexDataTestGood> vertices = {
		VertexDataTestGood(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
		VertexDataTestGood(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
		VertexDataTestGood(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.5f, 1.0f, 0.0f))
	};

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
	
	GLShader wrong_shader = GLShader(test_vert_file_1.c_str(), test_frag_file_1.c_str());

	EXPECT_FALSE(wrong_shader.compileStatus);
	EXPECT_TRUE(right_shader.compileStatus);
}

TEST_F(AllTests, TextureFileBeingRead)
{
	EXPECT_TRUE(awesomeFaceTexture.IsLoaded());
}

TEST_F(AllTests, AssetManagerShaderLoading)
{
	const auto loaded = asmngr.LoadShaders();

	EXPECT_TRUE(loaded);
	EXPECT_TRUE(asmngr.shaders.find("good_test") != asmngr.shaders.end());
	EXPECT_TRUE(asmngr.shaders.find("failing_test") != asmngr.shaders.end());
}

TEST_F(AllTests, AssetManagerTextureLoading)
{

	EXPECT_FALSE(asmngr.IsTextureLoaded("awesomeface"));

	const auto loaded = asmngr.LoadTextures();

	EXPECT_TRUE(loaded);
	EXPECT_TRUE(asmngr.textures.find("awesomeface") != asmngr.textures.end());
	EXPECT_TRUE(asmngr.IsTextureLoaded("awesomeface"));

	// Intentionally spelt wrong.
	EXPECT_FALSE(asmngr.IsTextureLoaded("awesomefaec"));

	// Try adding texture to the same thing. It should return false.
	EXPECT_FALSE(asmngr.AddToTextures("awesomeface", &awesomeFaceTexture));
}

TEST_F(AllTests, MeshInitializerAndRender)
{
	Mesh mesh((void *)&vertices[0], sizeof(VertexDataTestGood), 3);

	EXPECT_FALSE(mesh.IsUsingIndexBuffer());
	EXPECT_EQ(3, mesh.GetVertexAttribCounter());

	// Make sure that the Data copied to the buffer matches the Data in the vertices.
	void * testing_buffer_data;
	testing_buffer_data = malloc(sizeof(vertices));
	memset(testing_buffer_data, 0, sizeof(vertices));
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), testing_buffer_data);

	// Now we check if the first float value is equal to the one we put in.
	float buffer_value = *((float *)testing_buffer_data);

	EXPECT_FLOAT_EQ(0.5f, buffer_value);

	// Make sure no OpenGL Errors took Place.
	EXPECT_EQ(0, mesh.GetOpenglErrorFlag());

	std::vector<Texture *> textures;
	textures.push_back(&awesomeFaceTexture);
	mesh.Render(&right_shader, textures);

	// Make sure no OpenGL Errors took Place.
	EXPECT_EQ(0, mesh.GetOpenglErrorFlag());

}

TEST_F(AllTests, MeshInitializerIndicesAndRender)
{
	std::vector<unsigned int> indices = { 0, 1, 2 };

	Mesh mesh((void *)&vertices[0], sizeof(VertexDataTestGood), 3, indices);

	EXPECT_TRUE(mesh.IsUsingIndexBuffer());
	EXPECT_EQ(3, mesh.GetVertexAttribCounter());

	// Make sure that the Data copied to the buffer matches the Data in the vertices.
	void * testing_buffer_data;
	testing_buffer_data = malloc(sizeof(vertices));
	memset(testing_buffer_data, 0, sizeof(vertices));
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), testing_buffer_data);

	// Now we check if the first float value is equal to the one we put in.
	float buffer_value = *((float *)testing_buffer_data);
	EXPECT_FLOAT_EQ(0.5f, buffer_value);
	delete testing_buffer_data;

	// Make sure that the Data copied to the buffer matches the Data in the Indices
	testing_buffer_data = malloc(sizeof(indices));
	memset(testing_buffer_data, 0, sizeof(indices));
	glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), testing_buffer_data);

	// Now we check if the first float value is equal to the one we put in.
	buffer_value = *((unsigned int *)testing_buffer_data);
	EXPECT_EQ(0, buffer_value);
	delete testing_buffer_data;

	// Make sure no OpenGL Errors took Place.
	EXPECT_EQ(0, mesh.GetOpenglErrorFlag());

}