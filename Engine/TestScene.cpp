#include "TestScene.h"
#include "AssetManager.h"
#include "Window.h"
#include <glm/gtc/matrix_transform.inl>
#include "external_files/ImGUI/imgui.h"

namespace piolot {

	std::string Vec3ToString(glm::vec3 _in)
	{
		return "(" + std::to_string(_in.x) + ", " + std::to_string(_in.y) + ", " + std::to_string(_in.z) + ")";
	}

	TestScene::TestScene(std::shared_ptr<Window> _window)
		: Scene(_window)
	{
		ASMGR.ClearAllData();

		ASMGR.LoadShaders();
		ASMGR.LoadTextures();

		cameras.push_back(std::make_shared<Camera>(glm::vec3(0, 0, 10), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0)));

		entities.push_back(std::make_shared<Entity>("lowpolytree/lowpolytree.obj", "good_test"));

		ActiveCamera(cameras[0]);

		// We need to wait for the Shaders to be loaded to call this function.
		test.Init();
		
		//terrain_test = std::make_shared<Terrain>(10, 10, 0.5, 0.5, "Assets/Textures/heightmap.jpg");
		terrain_test = std::make_shared<Terrain>(10, 10, 0.5, 0.5, "Assets/Textures/heightmap.jpg");

	}

	void TestScene::OnUpdate(float _deltaTime, float _totalTime)
	{
		const auto projection_matrix = glm::perspective(45.0f, float(window->GetWidth()) / window->GetHeight(), 0.1f, 100.0f);

		for (const auto& it : entities) {
			it->Update(_deltaTime);
		}

		glm::vec3 mouse_pointer_ray = activeCamera->GetMouseRayDirection(window->mouseX, window->mouseY, window->GetWidth(), window->GetHeight(), projection_matrix);

		/* Ray Picking */
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

		/* Find Random Paths */
		{
			const float interval = 5;

			terrain_test->ClearColours();

			glm::vec3 startPosition = glm::vec3(startxz.x, 0, startxz.y);
			glm::vec3 endPosition = glm::vec3(endxz.x, 0, endxz.y);

			// Get the node pos.
			const glm::vec2 test_get_node = terrain_test->GetNodeIndicesFromPos(startPosition.x, startPosition.z);

			//LOGGER.AddToLog(
			//	"Random Point Selected: " + std::to_string(startPosition.x) + " , " + std::to_string(startPosition.z) + " "
			//	"The node returned is: " + std::to_string(test_get_node.x) + ", " + std::to_string(test_get_node.y)
			//);

			terrain_test->HighlightNode(test_get_node.x, test_get_node.y);

			startPosition.y = terrain_test->GetHeightAtPos(startPosition.x, startPosition.z);
			endPosition.y = terrain_test->GetHeightAtPos(endPosition.x, endPosition.z);

			glm::vec2 test_end_node = terrain_test->GetNodeIndicesFromPos(endPosition.x, endPosition.z);

			terrain_test->HighlightNode(test_end_node.x, test_end_node.y);

			path = terrain_test->GetPathFromPositions(startPosition, endPosition);

			std::string log_temp = "The path b/w the tiles, ";

			log_temp += Vec3ToString(startPosition) + " and " + Vec3ToString(endPosition) + " has " + std::to_string(path.size()) + " nodes";

			for ( auto it : path)
			{
				terrain_test->HighlightNode(it->tileIndexX, it->tileIndexZ);
			}

		}

		terrain_test->Update(_deltaTime, _totalTime);

		test.Update(activeCamera->GetViewMatrix(), projection_matrix);
	}

	void TestScene::OnRender()
	{
		for (const auto& it : entities) {
			it->Render();
		}

		terrain_test->Render();

		test.Render();
	}

	void TestScene::OnImguiRender()
	{

		ImGui::NewFrame();

		if (ImGui::BeginMainMenuBar())
		{
			if ( ImGui::BeginMenu("Windows"))
			{
				if ( ImGui::MenuItem("Pathing Debug Window"))
				{
					pathingDebugWindow = true;
				}
				if( ImGui::MenuItem("Asset Manager Debug Window"))
				{
					displayAssetManagerWindow = true;
				}
				if ( ImGui::MenuItem("Log Window"))
				{
					displayLogWindow = true;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		//throw std::logic_error("The method or operation is not implemented.");
		ImGui::Text("Hello World");

		if ( pathingDebugWindow )
		{
			ImGui::Begin("Terrain Pathing", &pathingDebugWindow);

			ImGui::SliderFloat2("Start Position", glm::value_ptr(startxz), 0.0f, 10.0f);
			ImGui::SliderFloat2("End Position", glm::value_ptr(endxz), 0.0f, 10.0f);

			std::string temp_log = "The path b/w these points has size + " + std::to_string(path.size());

			ImGui::Text(temp_log.c_str());

			if (path.empty())
			{
				// Print the tilesets.
				std::string temp_log = "The node sets for the nodes are " + std::to_string(terrain_test->GetNodeSetFromPos(startxz.x, startxz.y)) + ", " + std::to_string(terrain_test->GetNodeSetFromPos(endxz.x, endxz.y));
				ImGui::Text(temp_log.c_str());
			}

			ImGui::End();
		}

		if ( displayAssetManagerWindow )
		{
			ASMGR.GuiRender(&displayAssetManagerWindow);
		}
		if ( displayLogWindow)
		{
			LOGGER.Render(&displayLogWindow);
		}
		// ImGui::End();
	}

}
