#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <gtest/gtest.h>
#include "Window.h"
#include "AssetManager.h"
#include "Mesh.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.inl>

#define TESTING_ONLY	0

int main(int argc, char ** argv)
{

	/*_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);*/

	testing::InitGoogleTest(&argc, argv);
	int test_value = RUN_ALL_TESTS();

	std::cout << "Press Return to Continue" << std::endl;
	const int c = getchar();
	if (c == 32)
	{
		return 0;
	}


#if !TESTING_ONLY

	_CrtMemState memoryState = { 0 };
	_CrtMemCheckpoint(&memoryState);

	{

		Window window = Window(800, 600, "Vermin");

		piolot::Camera camera = piolot::Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));

		piolot::AssetManager asmngr;

		asmngr.LoadShaders();
		asmngr.LoadTextures();

		struct VertexDataTestGood
		{
			glm::vec3 position;
			glm::vec3 texCoord;	// Note all the things have to be a vec3.

			VertexDataTestGood(glm::vec3 _position, glm::vec3 _texCoord)
				: position(_position), texCoord(_texCoord)
			{

			}

		};

		std::vector<VertexDataTestGood> vertices = {
			VertexDataTestGood(glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 0.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(0.5f, -0.5f, -0.5f),	glm::vec3(1.0f, 0.0f,0.0f)),
			VertexDataTestGood(glm::vec3(0.5f,  0.5f, -0.5f),	glm::vec3(1.0f, 1.0f,0.0f)),
			VertexDataTestGood(glm::vec3(0.5f,  0.5f, -0.5f),	glm::vec3(1.0f, 1.0f,0.0f)),
			VertexDataTestGood(glm::vec3(-0.5f,  0.5f, -0.5f),	glm::vec3(0.0f, 1.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 0.0f, 0.0f)),

			VertexDataTestGood(glm::vec3(-0.5f, -0.5f,  0.5f),	glm::vec3(0.0f, 0.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(0.5f, -0.5f,  0.5f),	glm::vec3(1.0f, 0.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(0.5f,  0.5f,  0.5f),	glm::vec3(1.0f, 1.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(0.5f,  0.5f,  0.5f),	glm::vec3(1.0f, 1.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(-0.5f,  0.5f,  0.5f),	glm::vec3(0.0f, 1.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(-0.5f, -0.5f,  0.5f),	glm::vec3(0.0f, 0.0f, 0.0f)),

			VertexDataTestGood(glm::vec3(-0.5f,  0.5f,  0.5f),	glm::vec3(1.0f, 0.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(-0.5f,  0.5f, -0.5f),	glm::vec3(1.0f, 1.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 1.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 1.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(-0.5f, -0.5f,  0.5f),	glm::vec3(0.0f, 0.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(-0.5f,  0.5f,  0.5f),	glm::vec3(1.0f, 0.0f, 0.0f)),

			VertexDataTestGood(glm::vec3(0.5f,  0.5f,  0.5f),	glm::vec3(1.0f, 0.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(0.5f,  0.5f, -0.5f),	glm::vec3(1.0f, 1.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 1.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 1.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(0.5f, -0.5f,  0.5f),	glm::vec3(0.0f, 0.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(0.5f,  0.5f,  0.5f),	glm::vec3(1.0f, 0.0f, 0.0f)),

			VertexDataTestGood(glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 1.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(0.5f, -0.5f, -0.5f),	glm::vec3(1.0f, 1.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(0.5f, -0.5f,  0.5f),	glm::vec3(1.0f, 0.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(0.5f, -0.5f,  0.5f),	glm::vec3(1.0f, 0.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(-0.5f, -0.5f,  0.5f),	glm::vec3(0.0f, 0.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 1.0f, 0.0f)),

			VertexDataTestGood(glm::vec3(-0.5f,  0.5f, -0.5f),	glm::vec3(0.0f, 1.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(0.5f,  0.5f, -0.5f),	glm::vec3(1.0f, 1.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(0.5f,  0.5f,  0.5f),	glm::vec3(1.0f, 0.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(0.5f,  0.5f,  0.5f),	glm::vec3(1.0f, 0.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(-0.5f,  0.5f,  0.5f),	glm::vec3(0.0f, 0.0f, 0.0f)),
			VertexDataTestGood(glm::vec3(-0.5f,  0.5f, -0.5f),	glm::vec3(0.0f, 1.0f, 0.0f)),
		};

		const std::string awesomefacetexturepath = TEXTURE_FOLDER + std::string("container.jpg");
		piolot::Texture awesomefacetexture = piolot::Texture(awesomefacetexturepath);

		std::vector<unsigned int> indices = { 0, 1, 2 };
		//piolot::Mesh mesh((void *)&vertices[0], sizeof(VertexDataTestGood), 3, indices);
		piolot::Mesh mesh((void *)&vertices[0], sizeof(VertexDataTestGood), 36);

		std::string test_vert_file_2 = SHADER_FOLDER + std::string("good_test.vert");
		std::string test_frag_file_2 = SHADER_FOLDER + std::string("good_test.frag");

		piolot::GLShader right_shader = piolot::GLShader(test_vert_file_2.c_str(), test_frag_file_2.c_str());

		std::vector<piolot::Texture *> textures;
		textures.push_back(&awesomefacetexture);

		glm::mat4 model_matrix = glm::mat4(1.0);
		// Any thing beyond -1 in the model matrix is messed up because of the Near and Far clip Planes.
		model_matrix = glm::translate(model_matrix, glm::vec3(0, 0, -1));

		glm::mat4 projection_matrix = glm::perspective(45.0f, float(window.GetWidth()) / window.GetHeight(), 0.1f, 100.0f);

		right_shader.setMat4("model", model_matrix);
		right_shader.setMat4("projection", projection_matrix);

		float time = glfwGetTime();
		float deltaTime = 0;

		while (!glfwWindowShouldClose(window.GetWindow()))
		{

			deltaTime = glfwGetTime() - time;
			time = glfwGetTime();

			window.HandleInput();

			{
				if (window.IsKeyPressedOrHeld(GLFW_KEY_W))
				{
					camera.ProcessKeyboard(piolot::Camera::forward, deltaTime);
				}

				if (window.IsKeyPressedOrHeld(GLFW_KEY_S))
				{
					camera.ProcessKeyboard(piolot::Camera::back, deltaTime);
				}

				if (window.IsKeyPressedOrHeld(GLFW_KEY_A))
				{
					camera.ProcessKeyboard(piolot::Camera::leftside, deltaTime);
				}

				if (window.IsKeyPressedOrHeld(GLFW_KEY_D))
				{
					camera.ProcessKeyboard(piolot::Camera::rightside, deltaTime);
				}

				camera.ProcessMouseMovement(window.mouseOffsetX, window.mouseOffsetY);
			}

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			right_shader.setMat4("model", model_matrix);
			right_shader.setMat4("view", camera.GetViewMatrix());
			right_shader.setMat4("projection", projection_matrix);
			mesh.Render(&right_shader, textures);

			window.Update(deltaTime);
		}

	}

	// Do delete all the memory allocated by now.

	_CrtMemDumpAllObjectsSince(&memoryState);
	return 0;
#endif	

}


