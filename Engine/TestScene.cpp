#include "TestScene.h"
#include "AssetManager.h"
#include "Window.h"
#include <glm/gtc/matrix_transform.inl>

namespace piolot {

	TestScene::TestScene(std::shared_ptr<Window> _window)
		: Scene(_window)
	{
		ASMGR.ClearAllData();

		ASMGR.LoadShaders();
		ASMGR.LoadTextures();

		cameras.push_back(std::make_shared<Camera>(glm::vec3(0, 0, 10), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0)));

		entities.push_back(std::make_shared<Entity>("nanosuit/nanosuit.obj", "good_test"));

		ActiveCamera(cameras[0]);

		// We need to wait for the Shaders to be loaded to call this function.
		test.Init();

	}

	void TestScene::OnUpdate(float _deltaTime)
	{
		const auto projection_matrix = glm::perspective(45.0f, float(window->GetWidth()) / window->GetHeight(), 0.1f, 100.0f);

		for (const auto it : entities) {
			it->Update(_deltaTime);
		}

		glm::vec3 mouse_pointer_ray = activeCamera->GetMouseRayDirection(window->mouseX, window->mouseY, window->GetWidth(), window->GetHeight(), projection_matrix);

		{
			float int_distance = 0;
			float min_int_distance = 10000.0f;
			// Do Ray Picking Here.
			// For each Bounding Box, we check for the collision, and do what we want, as part of the Scene Update.

			// We reset this every frame.
			std::shared_ptr<Entity> selected_entity;
			// Loop through all Entities that can be selected.
			for (auto it : entities)
			{
				if (it->CheckIfMouseOvered(this->activeCamera->GetPosition(), mouse_pointer_ray, min_int_distance))
				{
					if (int_distance < min_int_distance)
					{
						min_int_distance = int_distance;
						selected_entity = it;
					}
				}
				it->SetSelectedInScene(false);
			}

			if (nullptr != selected_entity)
			{
				selected_entity->SetSelectedInScene(true);
			}

		}

		test.Update(activeCamera->GetViewMatrix(), projection_matrix);
	}

	void TestScene::OnRender()
	{
		for (const auto& it : entities) {
			it->Render();
		}

		test.Render();
	}

	void TestScene::OnImguiRender()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

}
