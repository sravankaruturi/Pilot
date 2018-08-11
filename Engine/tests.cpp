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
#include "Entity.h"

#define TESTING_ONLY			0
#define DISABLE_UNIT_TESTS		0

int main(int argc, char ** argv)
{

#if !DISABLE_UNIT_TESTS
	/*_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);*/

	testing::InitGoogleTest(&argc, argv);
	int test_value = RUN_ALL_TESTS();

	std::cout << "Press Return to Continue" << std::endl;
	const int c = getchar();
	if (c == 32)
	{
		return 0;
	}

	ASMGR.ClearAllData();

#endif


#if !TESTING_ONLY

	_CrtMemState memoryState = { 0 };
	_CrtMemCheckpoint(&memoryState);

	{

		Window window = Window(800, 600, "Vermin");

		piolot::Camera camera = piolot::Camera(glm::vec3(0, 0, 10), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));

		ASMGR.LoadShaders();
		ASMGR.LoadTextures();

		piolot::Entity nanosuit("cube/cube.obj", "good_test");
		piolot::Entity nanosuit2("cube/cube.obj", "good_test");

		nanosuit2.SetPosition(glm::vec3(3, 0, 0));
		nanosuit2.SetRotation(glm::vec3(0, 0, 45.0f));
		

		glm::mat4 projection_matrix = glm::perspective(45.0f, float(window.GetWidth()) / window.GetHeight(), 0.1f, 100.0f);

		float time = glfwGetTime();
		float deltaTime = 0;

		glm::vec3 mouse_pointer_ray;

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

				if (window.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
				{
					camera.ProcessMouseMovement(window.mouseOffsetX, window.mouseOffsetY);
				}

				mouse_pointer_ray = camera.GetMouseRayDirection(window.mouseX, window.mouseY, window.GetWidth(), window.GetHeight(), projection_matrix);
				
			}

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			nanosuit.Update(deltaTime);
			nanosuit2.Update(deltaTime);


			{
				// We set the View and Projection Matrices for all the Shaders that has them ( They all should have them ideally ).
				for ( auto it : ASMGR.shaders)
				{
					it.second->use();
					it.second->setMat4("view", camera.GetViewMatrix());
					it.second->setMat4("projection", projection_matrix);
				}
			}

			{
				// Do Ray Picking Here.
				// For each Bounding Box, we check for the collision, and do what we want, as part of the Scene Update.
				if ( nanosuit.GetBoundingBox().CheckForCollisionWithRay(camera.GetPosition(), mouse_pointer_ray))
				{
					nanosuit.SetSelectedInScene(true);
				}

				if (nanosuit2.GetBoundingBox().CheckForCollisionWithRay(camera.GetPosition(), mouse_pointer_ray))
				{
					nanosuit2.SetSelectedInScene(true);
				}

			}

			nanosuit.Render();		
			nanosuit2.Render();

			window.Update(deltaTime);
		}

	}

	// Do delete all the memory allocated by now.

	_CrtMemDumpAllObjectsSince(&memoryState);
	return 0;
#endif	

}


