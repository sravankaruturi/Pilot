#include "AllTests.h"

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

TEST_F(AllTests, AssetManagerLoadingShadersProperly)
{

	//std::string test_vert_file_1 = SHADER_FOLDER + std::string("failing_test.vert");
	//std::string test_frag_file_1 = SHADER_FOLDER + std::string("failing_test.frag");
	//
	//GLShader wrong_shader = GLShader(test_vert_file_1.c_str(), test_frag_file_1.c_str());

	//EXPECT_FALSE(wrong_shader.compileStatus);

	ASMGR.LoadShaders();

	for ( auto it : ASMGR.shaders)
	{
		EXPECT_TRUE(it.second->compileStatus);
	}

}

TEST_F(AllTests, TextureFileBeingRead)
{
	EXPECT_TRUE(awesomeFaceTexture.IsLoaded());
}

TEST_F(AllTests, MeshInitializer)
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

}

TEST_F(AllTests, MeshInitializerIndices)
{

	/* https://www.youtube.com/watch?v=FBbPWSOQ0-w&index=10&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2 OpenGL GL GetError Might return an arbitrary error if you do not make sure it is set to NO_ERRORS first?? WTF? */

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
	free(testing_buffer_data);

	// Make sure that the Data copied to the buffer matches the Data in the Indices
	testing_buffer_data = malloc(sizeof(indices));
	memset(testing_buffer_data, 0, sizeof(indices));
	glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), testing_buffer_data);

	// Now we check if the first float value is equal to the one we put in.
	buffer_value = *((unsigned int *)testing_buffer_data);
	EXPECT_EQ(0, buffer_value);
	free(testing_buffer_data);

}

TEST_F(AllTests, EntityConstructorCheck)
{
	
	Entity entity("cube/cube.obj", "good_test");
	EXPECT_STREQ("cube", entity.GetObjectName().c_str());

}