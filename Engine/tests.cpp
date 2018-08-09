#include <gtest/gtest.h>

#include "Window.h"
#include "AssetManager.h"
#include "Mesh.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.inl>

#define TESTING_ONLY	0

int main(int argc, char ** argv)
{
	
	testing::InitGoogleTest(&argc, argv);
	int test_value = RUN_ALL_TESTS();

	std::cout << "Press Return to Continue" << std::endl;
	const int c = getchar();
	if ( c == 32)
	{
		return 0;
	}


#if !TESTING_ONLY

	Window window = Window(800, 600, "Vermin");

	piolot::Camera camera = piolot::Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));

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

	glm::mat4 model_matrix = glm::mat4(1.0);
	// Any thing beyond -1 in the model matrix is messed up because of the Near and Far clip Planes.
	model_matrix = glm::translate(model_matrix, glm::vec3(0, 0, -1));

	right_shader.setMat4("model", model_matrix);

	float time = glfwGetTime();
	float deltaTime = 0;
	
	while(!glfwWindowShouldClose(window.GetWindow()))
	{
		
		deltaTime = glfwGetTime() - time;
		time = glfwGetTime();

		window.HandleInput();

		camera.ProcessKeyboard(piolot::Camera::rightside, deltaTime);
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		right_shader.setMat4("model", model_matrix);
		right_shader.setMat4("view", camera.GetViewMatrix());
		mesh.Render(&right_shader, textures);

		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}


#endif

}
