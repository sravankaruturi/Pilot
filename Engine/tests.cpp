#include <gtest/gtest.h>

#include "Window.h"
#include "GLShader.h"
#include "FolderLocations.h"

#define TESTING_ONLY	1

int main(int argc, char ** argv)
{
	
	testing::InitGoogleTest(&argc, argv);
	int test_value = RUN_ALL_TESTS();


#if !TESTING_ONLY

	Window window = Window(800, 600, "Vermin");

	GLShader shader = GLShader((SHADER_FOLDER + std::string("good_test.vert")).c_str(), (SHADER_FOLDER + std::string("good_test.frag")).c_str());

	float vertices[] = {
		// positions         // colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	while(!glfwWindowShouldClose(window.GetWindow()))
	{
		
		window.HandleInput();
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// render the triangle
		shader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}


#endif

}
