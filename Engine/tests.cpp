#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <gtest/gtest.h>
#include "Window.h"
#include "AssetManager.h"
#include "Mesh.h"
#include <glm/gtc/matrix_transform.inl>
#include "Object.h"

#include "TestScene.h"

#define TESTING_ONLY			0
#define DISABLE_UNIT_TESTS		1

int main(int argc, char ** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#if !DISABLE_UNIT_TESTS

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

	/*_CrtMemState memoryState = { 0 };
	_CrtMemCheckpoint(&memoryState);*/

	{

		std::shared_ptr<Window> window = std::make_shared<Window>(800, 600, "Vermin");

		piolot::TestScene test_scene(window);

		float time = glfwGetTime();

		while (!glfwWindowShouldClose(window->GetWindow()))
		{
			const auto projection_matrix = glm::perspective(45.0f, float(window->GetWidth()) / window->GetHeight(), 0.1f, 100.0f);
			const float delta_time = glfwGetTime() - time;
			time = glfwGetTime();

			window->HandleInput();

			{
				if (window->IsKeyPressedOrHeld(GLFW_KEY_W))
				{
					test_scene.ActiveCamera()->ProcessKeyboard(piolot::Camera::forward, delta_time);
				}

				if (window->IsKeyPressedOrHeld(GLFW_KEY_S))
				{
					test_scene.ActiveCamera()->ProcessKeyboard(piolot::Camera::back, delta_time);
				}

				if (window->IsKeyPressedOrHeld(GLFW_KEY_A))
				{
					test_scene.ActiveCamera()->ProcessKeyboard(piolot::Camera::leftside, delta_time);
				}

				if (window->IsKeyPressedOrHeld(GLFW_KEY_D))
				{
					test_scene.ActiveCamera()->ProcessKeyboard(piolot::Camera::rightside, delta_time);
				}

				if (window->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
				{
					test_scene.ActiveCamera()->ProcessMouseMovement(window->mouseOffsetX, window->mouseOffsetY);
				}
				
			}

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			test_scene.OnUpdate(delta_time);

			{
				// We set the View and Projection Matrices for all the Shaders that has them ( They all should have them ideally ).
				for ( auto it : ASMGR.shaders)
				{
					it.second->use();
					it.second->setMat4("view", test_scene.ActiveCamera()->GetViewMatrix());
					it.second->setMat4("projection", projection_matrix);
				}
			}

			test_scene.OnRender();

			window->Update(delta_time);

		}

		// Do delete all the memory allocated by now.
		ASMGR.ClearAllData();

		// So all the shaders are being deleted and stuff.
		//PE_ASSERT(test.expired());

	}

	//_CrtMemDumpAllObjectsSince(&memoryState);
	return 0;
#endif	

}


