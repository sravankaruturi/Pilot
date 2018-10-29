#pragma once
#include "TestScene.h"
#include "AssetManager.h"
#include "Window.h"
#include <glm/gtc/matrix_transform.inl>
#include "Colours.h"
#include "Configurations.h"

#include <fstream>

#if ENABLE_GUI
#include "../EngineDeps/external_files/ImGUI/imgui.h"
#endif

#include "SaveSceneHelpers.h"

#define		NAME_LENGTH_TO_FILE		20

namespace piolot {

	static void test_scene_resize(GLFWwindow * _window, int)
	{

	}

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

		// We need to wait for the Shaders to be loaded to call this function.
		testGrid.Init();

		std::string heightmap_path = TEXTURE_FOLDER + std::string("heightmap.jpg");
		testTerrain = std::make_shared<Terrain>(25, 25, 0.5, 0.5, heightmap_path);

		TestScene::InitEntities();

		viewportsDetails[0].camera = activeCamera;
		viewportsDetails[1].camera = activeCamera;
		viewportsDetails[2].camera = activeCamera;
		viewportsDetails[3].camera = activeCamera;

		viewportsDetails[0].isOrthogonal = false;

	}

	void TestScene::InitEntities()
	{

		LOGGER.AddToLog("Initializing Entities...");

		/* Initialize Cameras */
		cameras.insert_or_assign("First", std::make_shared<Camera>("First", glm::vec3(0, 5, 10), glm::vec3(0.5, -0.5, -0.5), glm::vec3(0, 1, 0)));
		cameras.insert_or_assign("Second", std::make_shared<Camera>("Second", glm::vec3(10, 5, 10), glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0, 1, 0)));

		ActivateCamera(cameras.at("First"));

		LOGGER.AddToLog("Cameras Initialized...");

		//entities.push_back(std::make_shared<Entity>("tree", "lowpolytree/lowpolytree.obj", "good_test"));
		entities.push_back(std::make_unique<Entity>("building", "Medieval_House/Medieval_House.obj", "good_test"));
		LOGGER.AddToLog("Loaded the Medieval House");

		const float building_scaling_factor = 256.0f;
		entities[0]->SetScale(glm::vec3(1.0f / building_scaling_factor, 1.0f / building_scaling_factor, 1.0f / building_scaling_factor));

		animatedEntities.push_back(std::make_unique<AnimatedEntity>("bob", "boblamp/boblampclean.md5mesh", "bob_lamp", glm::vec3(-10, -10, 0), glm::vec3(10, 10, -60)));
		LOGGER.AddToLog("Loaded Bob");

		AnimatedEntity * animatedEntity = animatedEntities[0].get();
		animatedEntity->SetInitialPosition(glm::vec3(4.0, 0.0, 2.0), testTerrain.get());
		animatedEntity->SetScale(glm::vec3(0.0125f, 0.0125f, 0.0125f));
		animatedEntity->SetRotation(glm::vec3(90.f, 0.0f, 0.00f));

		std::shared_ptr<Texture> archer_diffuse = std::make_shared<Texture>(MODEL_FOLDER + std::string("archer/akai_diffuse.png"), false);
		ASMGR.AddToTextures("akai_diffuse", archer_diffuse);

		std::shared_ptr<Texture> building_diffuse = std::make_shared<Texture>(MODEL_FOLDER + std::string("Medieval_House/Medieval_House_Diff.png"), false);
		ASMGR.AddToTextures("building_diffuse", building_diffuse);

		for (int i = 0; i < 5; i++)
		{

			animatedEntities.push_back(std::make_unique<AnimatedEntity>("archer", "archer/KB_Punches.fbx", "bob_lamp", glm::vec3(-30, 0, -30), glm::vec3(30, 180, 30)));
			LOGGER.AddToLog("Pushed an Archer on to the Animated Entities");

			animatedEntity = animatedEntities[i + 1].get();
			animatedEntity->SetInitialPosition(glm::vec3(2.0, 0.0, (i + 1)), testTerrain.get());
			const float scale_factor = 256.f;
			animatedEntity->SetScale(glm::vec3(1 / scale_factor));
			animatedEntity->SetRotation(glm::vec3(0, 0.0f, 0.00f));
			animatedEntity->SetAnimationTotalTime(0.75f);
		}

		animatedEntity->gPlay.health = 20;

		// We have the pointer to the last Entitity.
		animatedEntity->gPlay.team = 1;

		//ASMGR.objects.at("archer_walking")->GetMeshes()[0]->textureNames[0] = "akai_diffuse";
		ASMGR.objects.at("KB_Punches")->GetMeshes()[0]->textureNames.push_back("akai_diffuse");
		ASMGR.objects.at("Medieval_House")->GetMeshes()[0]->textureNames.push_back("building_diffuse");

		buildingPlacer = std::make_unique<Entity>("building", "Medieval_House/Medieval_House.obj", "buildingPlacer");
		//buildingPlacer->SetPosition(glm::vec3(0.0f));
		buildingPlacer->SetScale(glm::vec3(1.0f / building_scaling_factor, 1.0f / building_scaling_factor, 1.0f / building_scaling_factor) * 1.01f);
		ASMGR.shaders.at("buildingPlacer")->use();
		ASMGR.shaders.at("buildingPlacer")->setVec4("u_Colour0", 0, 1, 0, 1);

		LOGGER.AddToLog("Finished Initializing Entities..");

	}

	void TestScene::OnUpdate(float _deltaTime, float _totalTime)
	{

		testTerrain->ResetOccupiedBy();

		Scene::OnUpdate(_deltaTime, _totalTime);
		

		if (window->IsKeyPressedOrHeld(GLFW_KEY_C))
		{
			ActivateCamera(cameras.at("Second"));
			viewportsDetails[0].camera = activeCamera;
		}

		projectionMatrix = glm::perspective(45.0f, float(window->GetWidth()) / window->GetHeight(), 0.1f, 100.0f);

		for (auto& it : cameras) {
			it.second->UpdateVectors();
		}

		// I can update all the Positions here.
		glm::vec3 temp_position{};
		for (const auto& it : entities) {

			temp_position = it->GetPosition();
			temp_position.y = testTerrain->GetHeightAtPos(temp_position.x, temp_position.z);
			it->SetPosition(temp_position);

			it->Update(_deltaTime);
			
			testTerrain->GetTileFromIndices(
				testTerrain->GetNodeIndicesFromPos(it->GetPosition())
			)->occupiedBy = it.get();
		}

		std::vector<unsigned int> to_be_deleted_entities_indices;
		unsigned int i = 0;
		for (const auto& it : animatedEntities)
		{
			temp_position = it->GetPosition();
			temp_position.y = testTerrain->GetHeightAtPos(temp_position.x, temp_position.z);
			it->SetPosition(temp_position);

			// If You are actually attacking someone and they move, you are supposed to update the position.

			it->Update(_deltaTime);
			it->PlayAnimation(_deltaTime, _totalTime);

			testTerrain->GetTileFromIndices(testTerrain->GetNodeIndicesFromPos(it->GetPosition()))->occupiedBy = it.get();

			// Now if you are supposed to attack, then attack every frame gradually. We should eventually change it to something like attack every 1 move or something like that.
			// We have to make sure that no other person is attacking this target.
			// We have to wait if someone else is attacking the same target.
			// So we can take a hit from every attacker we have which is atmost one. We do not deal with the attack targets so much as the attacker itself.
			if (it->gPlay.attacker != nullptr) {
				it->gPlay.health -= _deltaTime;
			}

			if (it->gPlay.health <= 0) {
				// Die.
				// Add the Index to the to die list.
				// Get the Index
				to_be_deleted_entities_indices.push_back(i);
			}

			i++;
		}

		for ( unsigned int i = 0 ; i < to_be_deleted_entities_indices.size() ; i++)
		{
			const unsigned int j = to_be_deleted_entities_indices[i];

			// Check if the selected Entities consist that, and remove that??
			for (unsigned int k = 0; k < selectedEntities.size(); k++)
			{
				if (selectedEntities[k] == animatedEntities[j - i].get())
				{
					selectedEntities.erase(selectedEntities.begin() + k);
				}
			}

			// Update the attackers, target.
			animatedEntities[j - i]->gPlay.attacker->gPlay.attackTarget = nullptr;
			animatedEntities[j - i]->gPlay.attacker->gPlay.attackingMode = false;
			animatedEntities.erase(animatedEntities.begin() + j - i);

			// We have to subtract with i because every delete shortens the vector itself.
		}

		buildingPlacer->Update(_deltaTime);

		testTerrain->ClearColours();

		this->RayPicking();

		/* Find Paths for each entity */
		for (auto& it : animatedEntities)
		{

			// Make sure that the Target Node is up to date  if the Target moves around.
			if( it->gPlay.attackingMode && it->gPlay.attackTarget != nullptr)
			{
				it->setTargetNode(
					testTerrain->GetNodeIndicesFromPos(
						it->gPlay.attackTarget->GetPosition()
					)
				);
			}

			glm::vec3 startPosition = it->GetPosition();
			glm::ivec2 end_node = it->GetTargetPosition();

			glm::vec3 endPosition = testTerrain->GetTileFromIndices(end_node.x, end_node.y)->GetPosition();

			testTerrain->HighlightNode(end_node.x, end_node.y);

			path = testTerrain->GetPathFromPositions(startPosition, endPosition);

			std::string log_temp = "The path b/w the tiles, ";

			log_temp += Vec3ToString(startPosition) + " and " + Vec3ToString(endPosition) + " has " + std::to_string(path.size()) + " nodes";

			if (it->gPlay.attackingMode && !path.empty()) {
				// If you are attacking, you stop one tile before the actual target.
				path.pop_back();
			}

			if ( it->gPlay.attackingMode ){

				// Close enough
				if (path.size() < 2 && it->gPlay.attackTarget->gPlay.attacker == nullptr)
				{
					it->gPlay.attackTarget->gPlay.attacker = it.get();
					// That object immediately starts attacking the current player.
					it->gPlay.attackTarget->gPlay.attackingMode = true;
					it->gPlay.attackTarget->gPlay.attackTarget = it.get();
				}else if (path.size() > 2 && it->gPlay.attackTarget->gPlay.attacker == it.get())
				{
					it->gPlay.attackTarget->gPlay.attacker = nullptr;
				}
			}

			// To remove attackers.. Each entity can check if the attacker still exists and is still attacking?

			totalTimeCounterForPathing += _deltaTime;

			if (totalTimeCounterForPathing < 1.0f && !path.empty()) {

				// Get the Current Node.
				glm::vec2 start_indices = testTerrain->GetNodeIndicesFromPos(it->GetPosition().x, it->GetPosition().z);
				auto start_tile = testTerrain->GetTileFromIndices(start_indices.x, start_indices.y);

				// Look for the Next Node.
				auto next_tile = path.back();

				

				// Traverse the Distance b/w them * deltaTime. --> You complete the distance two nodes in 1 second.
				glm::vec3 current_position = it->GetPosition();
				glm::vec3 final_position = current_position + ((next_tile->GetPosition() - current_position) * _deltaTime * it->gPlay.movementSpeed);

				// Get the Rotation, about, y ,axis, with both the nodes.
				// Get the Vector, Target Node - Current Node.
				// The Inverse of Dot Product b/w that and X Axis, should be the angle?
				glm::vec3 target_direction = glm::normalize(final_position - current_position);
				glm::vec3 x_axis = glm::vec3(0, 0, 1);

				glm::vec3 rotation = it->GetRotation();
				rotation.y = (target_direction.x > 0) ?  glm::degrees(glm::acos(glm::dot(target_direction, x_axis))) : (360 - glm::degrees(glm::acos(glm::dot(target_direction, x_axis))));

				current_position.x = final_position.x;
				current_position.z = final_position.z;

				it->SetPosition(current_position);
				it->SetRotation(rotation);

			}
			else if (totalTimeCounterForPathing > 1.0f) {
				totalTimeCounterForPathing = 0.0f;
			}

		}

		testTerrain->Update(_deltaTime, _totalTime);

		testGrid.Update(activeCamera->GetViewMatrix(), projectionMatrix);
	}

	void TestScene::OnRender()
	{
		const auto persp_projection_matrix = glm::perspective(45.0f, float(window->GetWidth()) / window->GetHeight(), 0.1f, 100.0f);
		//const auto ortho_projection_matrix = glm::ortho(-1 * window->GetWidth()/2, window->GetWidth()/2, - 1 * window->GetHeight()/2, window->GetHeight()/2);
		const auto ortho_projection_matrix = glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, 0.1f, 100.f);

		glm::mat4 projection_matrices[4] = { persp_projection_matrix, ortho_projection_matrix, ortho_projection_matrix, ortho_projection_matrix };
		glm::mat4 view_matrices[4] = { this->activeCamera->GetViewMatrix() };

		view_matrices[0] = viewportsDetails[0].camera->GetViewMatrix();
		view_matrices[1] = viewportsDetails[1].camera->GetViewMatrix();
		view_matrices[2] = viewportsDetails[2].camera->GetViewMatrix();
		view_matrices[3] = viewportsDetails[3].camera->GetViewMatrix();

		projection_matrices[0] = viewportsDetails[0].isOrthogonal ? ortho_projection_matrix : persp_projection_matrix;
		projection_matrices[1] = viewportsDetails[1].isOrthogonal ? ortho_projection_matrix : persp_projection_matrix;
		projection_matrices[2] = viewportsDetails[2].isOrthogonal ? ortho_projection_matrix : persp_projection_matrix;
		projection_matrices[3] = viewportsDetails[3].isOrthogonal ? ortho_projection_matrix : persp_projection_matrix;

		// We set the View and Projection Matrices for all the Shaders that has them ( They all should have them ideally ).
		for (auto it : ASMGR.shaders)
		{
			it.second->use();

			if (it.first != "terrain" && it.first != "axes" && it.first != "bob_lamp")
			{
				it.second->setMat4("u_ViewMatrix", view_matrices[0]);
				it.second->setMat4("u_ProjectionMatrix", projection_matrices[0]);
			}

		}

		{
			std::vector<std::string> multiple_viewport_shaders = {
				"axes",
				"bob_lamp",
				"terrain"
			};

			for (auto& it : multiple_viewport_shaders) {

				auto shader_pointer = ASMGR.shaders.at(it);
				shader_pointer->use();
				auto loc = shader_pointer->GetUniformLocation("u_ViewMatrix");
				PE_GL(glUniformMatrix4fv(loc, 4, GL_FALSE, &view_matrices[0][0][0]));

				loc = shader_pointer->GetUniformLocation("u_ProjectionMatrix");
				PE_GL(glUniformMatrix4fv(loc, 4, GL_FALSE, glm::value_ptr(projection_matrices[0])));

			}
		}


		for (const auto& it : entities) {
			it->Render();
		}

		for (const auto& it : animatedEntities) {
			it->Render();
		}

		if ( isPlacingMode)
		{
			buildingPlacer->Render();
		}

		cameraRay.Render(ASMGR.shaders.at("axes"), red);

		testTerrain->Render();

	}

	
	void TestScene::SaveScene(const char * _fileName)
	{

		std::string test_string = std::string(SCENES_FOLDER) + std::string(_fileName);
		const char * actual_file_name = (test_string).c_str();

		std::ofstream out(actual_file_name, std::ios::binary);

		if (out.good()) {

			// Save all the Stuff.

			// Start by saving all the GUI bools and see if it is working.
			out.write((char *)&pathingDebugWindow, sizeof(bool));
			out.write((char *)&displayAssetManagerWindow, sizeof(bool));
			out.write((char *)&displayLogWindow, sizeof(bool));
			out.write((char *)&displayCameraControls, sizeof(bool));
			out.write((char *)&displayRaypickingControls, sizeof(bool));
			out.write((char *)&displayDemoWindow, sizeof(bool));
			out.write((char *)&displayViewportControls, sizeof(bool));

			// We Save all the Cameras.
			int number_of_cameras = cameras.size();
			out.write((char *)&number_of_cameras, sizeof(int));

			for (auto it : cameras) {
				pe_helpers::store_strings(it.first, out);
				it.second->SaveToStream(out);
			}

			// Store the Viewport Details.
			for (auto i = 0; i < 4; i++) {
				out.write((char*)(&(viewportsDetails[i].isOrthogonal)), sizeof(bool));
				pe_helpers::store_strings(viewportsDetails[i].camera->GetCameraName(), out);
			}

			// Store the Terrain
			testTerrain->SaveToFile(out);

			pe_helpers::store_strings("Entities", out);
			// Save all the Entities.
			int number_of_entities = entities.size();
			out.write((char*)&number_of_entities, sizeof(int));
			for (auto& it : entities) {
				it->SaveToFile(out);
			}


			// Save all the Animated Entities
			pe_helpers::store_strings("Animated Entities", out);
			number_of_entities = animatedEntities.size();
			out.write((char*)&number_of_entities, sizeof(int));
			for (auto& it : animatedEntities) {
				it->SaveToFile(out);
			}

		}

		out.close();
	}

	void TestScene::LoadScene(const char * _fileName)
	{

		std::string test_string = std::string(SCENES_FOLDER) + std::string(_fileName);
		const char * actual_file_name = (test_string).c_str();

		std::ifstream in(actual_file_name, std::ios::binary);

		if (in.good() && !in.eof()) {
			in.read((char *)&pathingDebugWindow, sizeof(bool));
			in.read((char *)&displayAssetManagerWindow, sizeof(bool));
			in.read((char *)&displayLogWindow, sizeof(bool));
			in.read((char *)&displayCameraControls, sizeof(bool));
			in.read((char *)&displayRaypickingControls, sizeof(bool));
			in.read((char *)&displayDemoWindow, sizeof(bool));
			in.read((char *)&displayViewportControls, sizeof(bool));

			int number_of_cameras = 0;
			in.read((char *)&number_of_cameras, sizeof(int));

			cameras.clear();
			activeCamera.reset();

			for (auto i = 0; i < number_of_cameras; i++) {

				std::string camera_key;

				std::shared_ptr<Camera> cam = std::make_shared<Camera>();

				pe_helpers::read_strings(camera_key, in);

				cam->LoadFromStream(in);

				cameras.insert_or_assign(camera_key, cam);

				if (nullptr == activeCamera) {
					activeCamera = cam;
					viewportsDetails[0].camera = activeCamera;
				}

			}

			// Load the Viewport Details
			for (auto i = 0; i < 4; i++) {
				in.read((char*)(&(viewportsDetails[i].isOrthogonal)), sizeof(bool));
				std::string camera_name;
				pe_helpers::read_strings(camera_name, in);
				viewportsDetails[i].camera = cameras.at(camera_name);
			}

			// Load the Terrain
			ASMGR.objects.erase("terrain");
			testTerrain->LoadFromFile(in);

			int number_of_entities = 0;
			std::string entity_header_string;
			pe_helpers::read_strings(entity_header_string, in);
			in.read((char*)&number_of_entities, sizeof(int));
			entities.clear();
			entities.resize(number_of_entities);
			for (auto i = 0; i < number_of_entities; i++) {

				entities[i] = (std::make_unique<Entity>());
				entities[i]->LoadFromFile(in);

			}

			// Load all the animated Entities.
			pe_helpers::read_strings(entity_header_string, in);
			in.read((char*)&number_of_entities, sizeof(int));
			animatedEntities.clear();
			animatedEntities.resize(number_of_entities);

			for (auto i = 0; i < number_of_entities; i++) {

				animatedEntities[i] = (std::make_unique<AnimatedEntity>());
				animatedEntities[i]->LoadFromFile(in);

			}
		}

		in.close();

	}

	void TestScene::RayPicking()
	{

		glm::vec3 mouse_pointer_ray;

		/* Ray Picking */
		/* We would assume that Ray Picking works only in two instances of Viewport Style. 4 Viewports and 1 Viewport */
		{
			float int_distance = 0;
			float min_int_distance = 10000.0f;
			// Do Ray Picking Here.
			// For each Bounding Box, we check for the collision, and do what we want, as part of the Scene Update.

			// We get the Camera Position and then move it according to the Viewports.
			// We get the Size of the Viewport 0. We comapre that to the Size of the framebuffer.

			int viewport_size[4];
			PE_GL(glGetIntegeri_v(GL_VIEWPORT, 0, viewport_size));

			// If this is 0, 0, w, h, then we only have one viewport. Else. We have 4.
			//std::cout << viewport_size[0] << ", " << viewport_size[1] << ", " << viewport_size[2] << ", " << viewport_size[3] << std::endl;

			glm::vec3 ray_start;
			if (viewport_size[2] == window->GetWidth() && viewport_size[3] == window->GetHeight()) {
				// One Viewport
				ray_start = this->activeCamera->GetPosition();
				mouse_pointer_ray = activeCamera->GetMouseRayDirection(window->mouseX, window->mouseY, window->GetWidth(), window->GetHeight(), projectionMatrix);
			}
			else {

				ray_start = this->activeCamera->GetPosition();

				// Multiple Viewport
				float updated_x, updated_y;

				updated_x = (window->mouseX > window->GetWidth() / 2.0f) ? window->mouseX - window->GetWidth() / 2.0f : window->mouseX;
				updated_y = (window->mouseY > window->GetHeight() / 2.0f) ? window->mouseY - window->GetHeight() / 2.0f : window->mouseY;

				updated_x *= 2;
				updated_y *= 2;

				//mouse_pointer_ray = activeCamera->GetMouseRayDirection(updated_x, updated_y, viewport_size[2], viewport_size[3], projectionMatrix);
				mouse_pointer_ray = activeCamera->GetMouseRayDirection(updated_x, updated_y, window->GetWidth(), window->GetHeight(), projectionMatrix);

			}

			// Draw the house wherever the mouse points, on the Terrain.
			const glm::ivec2 target_node = testTerrain->pointedNodeIndices;

			// Check if you can place the building there.
			bool test_can_place = testTerrain->CanPlaceHere(target_node.x, target_node.y);

			const MapTile * current_tile = testTerrain->GetTileFromIndices(target_node.x, target_node.y);
			buildingPlacer->SetPosition(current_tile->GetPosition());

			ASMGR.shaders.at("buildingPlacer")->use();
			ASMGR.shaders.at("buildingPlacer")->setVec4("u_Colour0", glm::vec4((test_can_place) ? green : red, 1.0f));


			// #TODO: Create a separate fucntion called Add Building or something.
			if (window->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1) && isPlacingMode && test_can_place) {

				glm::ivec2 target_node = testTerrain->pointedNodeIndices;

				// Make sure that there are enough nodes around the pointed node.
				if (!((target_node.x > testTerrain->GetNodeCountX() - 1 && target_node.x < 1) || (target_node.y > testTerrain->GetNodeCountZ() - 1 && target_node.y < 1))) {

					entities.push_back(std::make_unique<Entity>("building_new", "Medieval_House/Medieval_House.obj", "good_test"));

					Entity * last_entity = entities.back().get();
					const float scaling_factor = 256.0f;
					last_entity->SetScale(glm::vec3(1.0f / scaling_factor, 1.0f / scaling_factor, 1.0f / scaling_factor));
					last_entity->SetPosition(testTerrain->GetTileFromIndices(target_node.x, target_node.y)->GetPosition());

					// We then update the corresponding terrain nodes to not walkable.

					// We get the Nine Nodes, of the Building.
					testTerrain->SetTerrainNodeObstacle(target_node);
					testTerrain->SetTerrainNodeObstacle(glm::ivec2(target_node.x, target_node.y - 1));
					testTerrain->SetTerrainNodeObstacle(glm::ivec2(target_node.x, target_node.y + 1));

					testTerrain->SetTerrainNodeObstacle(glm::ivec2(target_node.x + 1, target_node.y));
					testTerrain->SetTerrainNodeObstacle(glm::ivec2(target_node.x + 1, target_node.y - 1));
					testTerrain->SetTerrainNodeObstacle(glm::ivec2(target_node.x + 1, target_node.y + 1));

					testTerrain->SetTerrainNodeObstacle(glm::ivec2(target_node.x - 1, target_node.y));
					testTerrain->SetTerrainNodeObstacle(glm::ivec2(target_node.x - 1, target_node.y - 1));
					testTerrain->SetTerrainNodeObstacle(glm::ivec2(target_node.x - 1, target_node.y + 1));

					isPlacingMode = false;

				}

			}

			// Loop through all Entities that can be selected.
			for (auto& it : entities)
			{
				if ((window->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1)) && (it->CheckIfMouseOvered(ray_start, mouse_pointer_ray, min_int_distance)))
				{
					if (int_distance < min_int_distance)
					{
						min_int_distance = int_distance;
						if (!window->IsKeyPressedOrHeld(GLFW_KEY_LEFT_SHIFT)) {
							selectedEntities.clear();
						}
						if (std::find(selectedEntities.begin(), selectedEntities.end(), it.get()) == selectedEntities.end()) {
							selectedEntities.push_back(it.get());
						}
					}
				}
				it->SetSelectedInScene(false);
			}

			for (int i = 0; i < animatedEntities.size(); i++)
			{
				AnimatedEntity * it = animatedEntities[i].get();
				if ((window->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1)) && it->CheckIfMouseOvered(ray_start, mouse_pointer_ray, min_int_distance))
				{
					if (int_distance < min_int_distance)
					{
						min_int_distance = int_distance;
						if (!window->IsKeyHeld(GLFW_KEY_LEFT_SHIFT)) {
							selectedEntities.clear();
						}
						if (std::find(selectedEntities.begin(), selectedEntities.end(), it) == selectedEntities.end()) {
							selectedEntities.push_back(it);
						}
					}
				}
				it->SetSelectedInScene(false);
			}


			for (auto it : selectedEntities)
			{
				it->SetSelectedInScene(true);
			}

		}

		cameraRay = Ray(activeCamera->GetPosition(), mouse_pointer_ray);

		if (nullptr != activeCamera)
		{
			Ray mouse_pointer_ray_ray{ activeCamera->GetPosition(), mouse_pointer_ray };
			this->testTerrain->GetMouseRayPoint(mouse_pointer_ray_ray);
		}

		if (window->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
		{
			// Update the Target Node

			// Based on the Number of Entities selected, Make sure that you set the formations here.
			// We use the Square Formation.. Always..

			const int number_of_selected_entities = selectedEntities.size();
			const int number_of_rows = glm::ceil(glm::sqrt(number_of_selected_entities));
			const int number_of_columns = glm::ceil((float)number_of_selected_entities / number_of_rows);
			int entity_counter = 0;

			glm::ivec2 target_node = testTerrain->pointedNodeIndices;

			// Check if the Target node already has an Entity. If so, we need to attack.
			if ( testTerrain->GetTileFromIndices(target_node)->occupiedBy != nullptr/* && testTerrain->GetTileFromIndices(target_node)->occupiedBy->team != selectedEntities.back()->team*/)
			{
				// Move to that tile, and attack.
				// To Attack, we set them to be in attacking mode.
				for ( auto it: selectedEntities)
				{
					it->gPlay.attackingMode = true;
					// You go there, and attack.

					it->gPlay.attackTarget = testTerrain->GetTileFromIndices(target_node)->occupiedBy;

				}
			}
			else {
				for (auto it : selectedEntities)
				{
					it->gPlay.attackingMode = false;
					it->gPlay.attackTarget = nullptr;
				}
			}

			for (int row_counter = 0; row_counter < number_of_rows; row_counter++) {
				for (int column_counter = 0; column_counter < number_of_columns; column_counter++) {

					if (entity_counter >= number_of_selected_entities) {
						break;
					}

					glm::ivec2 temp_target_node = target_node;
					temp_target_node.x += (row_counter - number_of_rows / 2);
					temp_target_node.y += (column_counter - number_of_rows / 2);
					selectedEntities[entity_counter++]->setTargetNode(temp_target_node);

				}
			}

		}

	}
}
