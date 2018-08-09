#include <gtest/gtest.h>

#include "Window.h"
#include "GLShader.h"
#include "FolderLocations.h"
#include "AssetManager.h"

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

	float vertices[] = {
		// positions         // colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,// bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 1.0, // top 
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texcoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(2);

	asmngr.shaders.at("good_test")->use();
	asmngr.shaders.at("good_test")->setInt("texture0", 0);

	
	while(!glfwWindowShouldClose(window.GetWindow()))
	{
		
		window.HandleInput();
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, asmngr.textures.at("container")->GetTextureId());

		// render the triangle
		asmngr.shaders.at("good_test")->use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}


#endif

}
