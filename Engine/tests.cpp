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

	piolot::Mesh mesh((void *)&vertices[0], sizeof(VertexDataTestGood), 3);

	std::vector<piolot::Texture *> textures;
	textures.push_back(asmngr.textures.at("container"));
	
	while(!glfwWindowShouldClose(window.GetWindow()))
	{
		
		window.HandleInput();
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		mesh.Render(asmngr.shaders.at("good_test"), textures);

		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}


#endif

}
