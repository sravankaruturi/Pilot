#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <gtest/gtest.h>
#include "Window.h"
#include "AssetManager.h"
#include "Mesh.h"
#include <glm/gtc/matrix_transform.inl>

#include "TestScene.h"

#include "GUIHelpers.h"
#include "external_files/ImGUI/imgui_impl_opengl3.h"
#include "external_files/ImGUI/imgui_impl_glfw.h"

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

	{

		const auto aspect_ratio = 16.0f / 9.0f;
		auto width = 1000;
		auto height = width / aspect_ratio;

		std::shared_ptr<Window> window = std::make_shared<Window>(width, height, "Vermin");

		/* ImGui setup */
		ImGui::CreateContext();

		ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(), false);
		ImGui_ImplOpenGL3_Init();

		// Setup style
		ImGui::StyleColorsDark();

		piolot::ImGuiLog imgui_logger;
		LOGGER.SetImGuiLogger(&imgui_logger);

		piolot::TestScene test_scene(window);

		float time = glfwGetTime();

		bool display_multiple_views = false;

		while (!glfwWindowShouldClose(window->GetWindow()))
		{
			const auto projection_matrix = glm::perspective(45.0f, float(window->GetWidth()) / window->GetHeight(), 0.1f, 100.0f);

			const float delta_time = glfwGetTime() - time;
			time = glfwGetTime();

			const ImGuiIO& io = ImGui::GetIO();

			window->HandleInput();

			{
				// Try Letting Imgui Handle the Inputs for Now.. We need to change this..
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

				if (window->IsKeyPressedAndReleased(GLFW_KEY_SPACE))
				{
					display_multiple_views = !display_multiple_views;
				}
				
			}

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			if ( display_multiple_views)
			{
				auto w2 = window->GetWidth() / 2;
				auto h2 = window->GetHeight() / 2;

				PE_GL(glViewportIndexedf(0, 0, 0, w2, h2));
				PE_GL(glViewportIndexedf(1, w2, 0, w2, h2));
				PE_GL(glViewportIndexedf(2, 0, h2, w2, h2));
				PE_GL(glViewportIndexedf(3, w2, h2, w2, h2));
			}else
			{
				PE_GL(glViewportIndexedf(0, 0, 0, window->GetWidth(), window->GetHeight()));
				PE_GL(glViewportIndexedf(1, 0, 0, 0, 0));
				PE_GL(glViewportIndexedf(2, 0, 0, 0, 0));
				PE_GL(glViewportIndexedf(3, 0, 0, 0, 0));
			}

			// Update Function
			test_scene.OnUpdate(delta_time, time);

			// Scene Render.
			test_scene.OnRender();

			// GUI Render
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();

			piolot::ImGuiControlVariables test_scene_vars = {
				display_multiple_views
			};

			test_scene.OnImguiRender(test_scene_vars);

			/*ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);*/

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


			window->Update(delta_time);

		}

		// Do delete all the memory allocated by now.
		ASMGR.ClearAllData();

		// So all the shaders are being deleted and stuff.
		//PE_ASSERT(test.expired());
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

	}

	return 0;
#endif	

}


