#include <gtest/gtest.h>

#include "Window.h"
#include "AssetManager.h"
#include "Mesh.h"

#define TESTING_ONLY	0

int main(int argc, char ** argv)
{
	
	testing::InitGoogleTest(&argc, argv);
	int test_value = RUN_ALL_TESTS();

	std::cout << "Press Return to Continue" << std::endl;
	getchar();


#if !TESTING_ONLY

	Window window = Window(800, 600, "Vermin");

	piolot::AssetManager asmngr;

	asmngr.LoadShaders();
	asmngr.LoadTextures();

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

	const std::string awesomefacetexturepath = TEXTURE_FOLDER + std::string("awesomeface.png");
	piolot::Texture awesomefacetexture = piolot::Texture(awesomefacetexturepath);

	std::vector<unsigned int> indices = { 0, 1, 2 };
	//piolot::Mesh mesh((void *)&vertices[0], sizeof(VertexDataTestGood), 3, indices);
	piolot::Mesh mesh((void *)&vertices[0], sizeof(VertexDataTestGood), 3);
	
	std::string test_vert_file_2 = SHADER_FOLDER + std::string("good_test.vert");
	std::string test_frag_file_2 = SHADER_FOLDER + std::string("good_test.frag");

	piolot::GLShader right_shader = piolot::GLShader(test_vert_file_2.c_str(), test_frag_file_2.c_str());

	std::vector<piolot::Texture *> textures;
	textures.push_back(&awesomefacetexture);
	
	while(!glfwWindowShouldClose(window.GetWindow()))
	{
		
		window.HandleInput();
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		mesh.Render(&right_shader, textures);

		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}


#endif

}
