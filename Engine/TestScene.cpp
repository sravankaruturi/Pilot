#pragma once
#include "TestScene.h"
#include "AssetManager.h"
#include "Window.h"
#include <glm/gtc/matrix_transform.inl>

#include "Configurations.h"

#include <fstream>

#if ENABLE_GUI
#include "../EngineDeps/external_files/ImGUI/imgui.h"
#endif

#include "SaveSceneHelpers.h"

#define		NAME_LENGTH_TO_FILE		20

namespace piolot {

	static void test_scene_resize(GLFWwindow * _window, int )
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

		cameras.insert_or_assign("First", std::make_shared<Camera>("First", glm::vec3(0, 0, 10), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0)));
		cameras.insert_or_assign("Second", std::make_shared<Camera>("Second", glm::vec3(10, 0, 10), glm::vec3(-1, 0, -1), glm::vec3(0, 1, 0)));

		entities.push_back(std::make_shared<Entity>("lowpolytree/lowpolytree.obj", "good_test"));

		ActiveCamera(cameras.at("First"));

		// We need to wait for the Shaders to be loaded to call this function.
		testGrid.Init();
		
		//terrain_test = std::make_shared<Terrain>(10, 10, 0.5, 0.5, "Assets/Textures/heightmap.jpg");
		std::string heightmap_path = TEXTURE_FOLDER + std::string("heightmap.jpg");
		testTerrain = std::make_shared<Terrain>(10, 10, 0.5, 0.5, heightmap_path);

		viewportsDetails[0].camera = activeCamera;
		viewportsDetails[1].camera = activeCamera;
		viewportsDetails[2].camera = activeCamera;
		viewportsDetails[3].camera = activeCamera;

		viewportsDetails[0].isOrthogonal = false;

	}

	void TestScene::OnUpdate(float _deltaTime, float _totalTime)
	{

		const auto projection_matrix = glm::perspective(45.0f, float(window->GetWidth()) / window->GetHeight(), 0.1f, 100.0f);

		for (auto& it : cameras) {
			it.second->UpdateVectors();
		}

		for (const auto& it : entities) {
			it->Update(_deltaTime);
		}

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

				mouse_pointer_ray = activeCamera->GetMouseRayDirection(window->mouseX, window->mouseY, window->GetWidth(), window->GetHeight(), projection_matrix);
			}
			else {

				ray_start = this->activeCamera->GetPosition();

				// Multiple Viewport
				// TODO: Fix This.
				float updated_x, updated_y;

				updated_x = (window->mouseX > window->GetWidth() / 2.0f) ? window->mouseX - window->GetWidth() / 2.0f : window->mouseX;
				updated_y = (window->mouseY > window->GetHeight() / 2.0f) ? window->mouseY - window->GetHeight() / 2.0f : window->mouseY;

				updated_x *= 2;
				updated_y *= 2;

				//mouse_pointer_ray = activeCamera->GetMouseRayDirection(updated_x, updated_y, viewport_size[2], viewport_size[3], projection_matrix);
				mouse_pointer_ray = activeCamera->GetMouseRayDirection(updated_x, updated_y, window->GetWidth(), window->GetHeight(), projection_matrix);

			}


			// We reset this every frame.
			std::shared_ptr<Entity> selected_entity;
			// Loop through all Entities that can be selected.
			for (auto it : entities)
			{
				if (it->CheckIfMouseOvered(ray_start, mouse_pointer_ray, min_int_distance))
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

			testTerrain->ClearColours();

			glm::vec3 startPosition = glm::vec3(startxz.x, 0, startxz.y);
			glm::vec3 endPosition = glm::vec3(endxz.x, 0, endxz.y);

			// Get the node pos.
			const glm::vec2 test_get_node = testTerrain->GetNodeIndicesFromPos(startPosition.x, startPosition.z);

			//LOGGER.AddToLog(
			//	"Random Point Selected: " + std::to_string(startPosition.x) + " , " + std::to_string(startPosition.z) + " "
			//	"The node returned is: " + std::to_string(test_get_node.x) + ", " + std::to_string(test_get_node.y)
			//);

			testTerrain->HighlightNode(test_get_node.x, test_get_node.y);

			startPosition.y = testTerrain->GetHeightAtPos(startPosition.x, startPosition.z);
			endPosition.y = testTerrain->GetHeightAtPos(endPosition.x, endPosition.z);

			glm::vec2 test_end_node = testTerrain->GetNodeIndicesFromPos(endPosition.x, endPosition.z);

			testTerrain->HighlightNode(test_end_node.x, test_end_node.y);

			path = testTerrain->GetPathFromPositions(startPosition, endPosition);

			std::string log_temp = "The path b/w the tiles, ";

			log_temp += Vec3ToString(startPosition) + " and " + Vec3ToString(endPosition) + " has " + std::to_string(path.size()) + " nodes";

			for ( auto it : path)
			{
				testTerrain->HighlightNode(it->tileIndexX, it->tileIndexZ);
			}

		}

		testTerrain->Update(_deltaTime, _totalTime);

		testGrid.Update(activeCamera->GetViewMatrix(), projection_matrix);
	}

	void TestScene::OnRender()
	{
		const auto persp_projection_matrix = glm::perspective(45.0f, float(window->GetWidth()) / window->GetHeight(), 0.1f, 100.0f);
		//const auto ortho_projection_matrix = glm::ortho(-1 * window->GetWidth()/2, window->GetWidth()/2, - 1 * window->GetHeight()/2, window->GetHeight()/2);
		const auto ortho_projection_matrix = glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, 0.1f, 100.f);

		glm::mat4 projection_matrices[4] = {persp_projection_matrix, ortho_projection_matrix, ortho_projection_matrix, ortho_projection_matrix};
		glm::mat4 view_matrices[4] = {this->activeCamera->GetViewMatrix()};

		/*view_matrices[1] = glm::lookAt(glm::vec3(8, 0, 0), glm::vec3(), glm::vec3(0, 1, 0));
		view_matrices[2] = glm::lookAt(glm::vec3(0, 8, 0), glm::vec3(), glm::vec3(0, 0, 1));;
		view_matrices[3] = glm::lookAt(glm::vec3(0, 0, 8), glm::vec3(), glm::vec3(0, 1, 0));;*/

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

			if ( it.first != "terrain" && it.first != "axes")
			{
				it.second->setMat4("u_ViewMatrix", view_matrices[0]);
				it.second->setMat4("u_ProjectionMatrix", projection_matrices[0]);
			}

		}

		{
			// Set the terrain Shader View and projection matrices for multiple viewports.
			auto terrain_shader = ASMGR.shaders.at("terrain");
			terrain_shader->use();
			//PE_GL(glUniformMatrix4fv(it.second->GetUniformLocation("u_ViewMatrix"), GL_FALSE, 4, &view_matrices[0][0][0]));
			auto loc = terrain_shader->GetUniformLocation("u_ViewMatrix");
			PE_GL(glUniformMatrix4fv(loc, 4, GL_FALSE, &view_matrices[0][0][0]));

			loc = terrain_shader->GetUniformLocation("u_ProjectionMatrix");
			PE_GL(glUniformMatrix4fv(loc, 4, GL_FALSE, glm::value_ptr(projection_matrices[0])));


			auto axes_shader = ASMGR.shaders.at("axes");
			axes_shader->use();
			//PE_GL(glUniformMatrix4fv(it.second->GetUniformLocation("u_ViewMatrix"), GL_FALSE, 4, &view_matrices[0][0][0]));
			loc = axes_shader->GetUniformLocation("u_ViewMatrix");
			PE_GL(glUniformMatrix4fv(loc, 4, GL_FALSE, &view_matrices[0][0][0]));

			loc = axes_shader->GetUniformLocation("u_ProjectionMatrix");
			PE_GL(glUniformMatrix4fv(loc, 4, GL_FALSE, glm::value_ptr(projection_matrices[0])));
		}
		

		for (const auto& it : entities) {
			it->Render();
		}

		testTerrain->Render();

		testGrid.Render();

	}

	void TestScene::OnImguiRender(ImGuiControlVariables& _vars)
	{

		ImGui::NewFrame();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Scene")) {
				if (ImGui::MenuItem("Save Scene")) {
					this->SaveScene(filenameToSaveScene.c_str());
				}
				if (ImGui::MenuItem("Save Scene As...")) {
					openSaveSceneAsWindow = true;
				}
				if (ImGui::MenuItem("Load Scene")) {
					openLoadSceneWindow = true;
				}
				ImGui::EndMenu();
			}
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
				if (ImGui::MenuItem("Ray Picking Debug Window")) {
					displayRaypickingControls = true;
				}

				if (ImGui::MenuItem("Demo Window")) {
					displayDemoWindow = true;
				}

				if( ImGui::MenuItem("Hierarchy"))
				{
					displayHierarchy = true;
				}

				ImGui::EndMenu();
			}


			if (ImGui::BeginMenu("View")) {

				if (ImGui::MenuItem("Cameras")) {
					displayCameraControls = true;
				}

				if ( ImGui::MenuItem( _vars.show_multiple_viewports ? "Hide Other Views" : "Show All Views" ))
				{
					_vars.show_multiple_viewports = !_vars.show_multiple_viewports;
				}

				if (ImGui::MenuItem("View Ports")) {
					displayViewportControls = true;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Entity"))
			{

				if (ImGui::MenuItem("Add"))
				{
					displayAddEntity = true;
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (openSaveSceneAsWindow) {
			ImGui::Begin("Save Scene", &openSaveSceneAsWindow);

			if (ImGui::InputText("File Name: ##SaveSceneFileName", &filenameToSaveScene[0], 20));

			if (ImGui::Button("Save Scene")) {
				this->SaveScene(filenameToSaveScene.c_str());
			}

			// TODO: Should we do this?. No do not do this, and provide a save button.
			if (ImGui::Button("Cancel")) {
				//filenameToSaveScene = "File Name";
				openSaveSceneAsWindow = false;
			}

			ImGui::End();
		}
		
		// TODO: Input Text is really wonky. Can we provide a better way, a drop down or selection box for people to choose how to save the files and how to load the files.
		if (openLoadSceneWindow) {
			ImGui::Begin("Load Scene", &openLoadSceneWindow);

			// Iterate through the Scenes directory and Show the Scenes.
			for (auto& p : std::experimental::filesystem::directory_iterator(SCENES_FOLDER)) {

				std::string file_name_temp = p.path().filename().generic_string();

				filenameToLoadScene = file_name_temp;

				if ( ImGui::Button(file_name_temp.c_str())) {

					//try {
						this->LoadScene(filenameToLoadScene.c_str());
						filenameToSaveScene = filenameToLoadScene;
					/*}
					catch (...) {
						LOGGER.AddToLog("Cannot Open Scene " + std::string(filenameToLoadScene), PE_LOG_ERROR);
						ImGui::Text("Cannot find the file. Please Check");
					}*/
				}
			}

			//ImGui::Separator();

			//if (ImGui::InputText("File to Load: ##LoadSceneFileName", &filenameToLoadScene[0], 20));

			//if (ImGui::Button("Load Scene")) {
			//	//try {
			//		this->LoadScene(filenameToLoadScene.c_str());
			//		filenameToSaveScene = filenameToLoadScene;
			//	//}
			//	//catch (...) {
			//	//	LOGGER.AddToLog("Cannot Open Scene " + std::string(filenameToLoadScene), PE_LOG_ERROR);
			//	//	ImGui::Text("Cannot find the file. Please Check");
			//	//}
			//}

			//ImGui::SameLine();

			//// TODO: Should we do this?. No do not do this, and provide a save button.
			//if (ImGui::Button("Cancel")) {
			//	//filenameToSaveScene = "File Name";
			//	openLoadSceneWindow = false;
			//}

			ImGui::End();
		}

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
				std::string temp_log = "The node sets for the nodes are " + std::to_string(testTerrain->GetNodeSetFromPos(startxz.x, startxz.y)) + ", " + std::to_string(testTerrain->GetNodeSetFromPos(endxz.x, endxz.y));
				ImGui::Text(temp_log.c_str());
			}

			ImGui::End();
		}

		if (displayHierarchy)
		{

			ImGui::Begin("Hierarchy", &displayHierarchy);

			//TODO: This should be highlighted in the Viewport as well.
			static std::shared_ptr<Entity> selected_entity;

			if (selected_entity == nullptr)
			{
				selected_entity = entities[0];
			}

			// Set the selected flag for the entity.
			selected_entity->SetSelectedInScene(true);

			ImGui::BeginChild("Entities##List", ImVec2(200, 0), true);

			// Loop through all the Entities
			for ( auto& it : entities)
			{
				ImGui::PushID(&it);
				if ( ImGui::Selectable(it->GetEntityName().c_str(), selected_entity == it))
				{
					selected_entity = it;
				}
				ImGui::PopID();

			}

			ImGui::EndChild();

			ImGui::SameLine();
			// Details of the selected Entity.
			ImGui::BeginGroup();

			{
				ImGui::BeginChild("Details", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

				ImGui::Text(selected_entity->GetEntityName().c_str());
				ImGui::Separator();

				selected_entity->DisplayDetailsImgui();

				ImGui::EndChild();

			}

			ImGui::EndGroup();
				
			

			// Cameras

			// Terrain?

			ImGui::End();

		}

		if (displayCameraControls)
		{
			ImGui::Begin("Available Cameras", &displayCameraControls);

			static std::shared_ptr<Camera> selected_camera;
			if (nullptr == selected_camera) {
				selected_camera = cameras.at("First");
			}

			ImGui::BeginChild("Cameras##List", ImVec2(150, 0), true);
			// Show all the cameras here.
			for (auto& it : cameras) {

				ImGui::PushID(&it);
				if (ImGui::Selectable(it.second->GetCameraName().c_str(), selected_camera == it.second)) {

					selected_camera = it.second;

				}
				ImGui::PopID();
			}
			ImGui::EndChild();

			ImGui::SameLine();

			ImGui::BeginGroup();

				ImGui::BeginChild("Details", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

				ImGui::Text(selected_camera->GetCameraName().c_str());
				ImGui::Separator();

				// Display Camera Details here.
				selected_camera->DisplayCameraDetailsImgui();

				ImGui::EndChild();

				if (ImGui::Button("Activate Camera")) {
					activeCamera = selected_camera;
					// Set the Camera to the Current Viewport.
					// It is 0 for now.
					viewportsDetails[0].camera = activeCamera;
				}

			ImGui::EndGroup();

			ImGui::End();
		}

		if (displayRaypickingControls) {
			ImGui::Begin("Ray Picking Debug", &displayRaypickingControls);

			int viewport_size[4];
			PE_GL(glGetIntegeri_v(GL_VIEWPORT, 0, viewport_size));

			ImGui::Text("Mouse position: %.3f , %.3f", this->window->mouseX, this->window->mouseY);

			float updated_x, updated_y;

			const auto projection_matrix = glm::perspective(45.0f, float(window->GetWidth()) / window->GetHeight(), 0.1f, 100.0f);
			glm::vec3 mouse_pointer_ray;

			if (viewport_size[2] == window->GetWidth() && viewport_size[3] == window->GetHeight()) {
				updated_x = window->mouseX;
				updated_y = window->mouseY;

				mouse_pointer_ray = activeCamera->GetMouseRayDirection(window->mouseX, window->mouseY, window->GetWidth(), window->GetHeight(), projection_matrix);

			}
			else {
				// Set Which viewport this is hovering over here.
				updated_x = (window->mouseX > window->GetWidth()/2.0f) ? window->mouseX - window->GetWidth() / 2.0f : window->mouseX;
				updated_y = (window->mouseY > window->GetHeight()/2.0f) ? window->mouseY - window->GetHeight() / 2.0f : window->mouseY;

				updated_x *= 2;
				updated_y *= 2;

				mouse_pointer_ray = activeCamera->GetMouseRayDirection(updated_x, updated_y, window->GetWidth(), window->GetHeight(), projection_matrix);
			}

			ImGui::InputFloat3("Mouse Pointer Ray", glm::value_ptr(mouse_pointer_ray));

			ImGui::Text("Updated Mouse position: %.3f , %.3f", updated_x, updated_y);

			ImGui::End();

		}

		if (displayAddEntity)
		{
			ImGui::Begin("Add an Entity", &displayAddEntity);

			// Add the Entity. The Gui for it.

			if (ImGui::Button("Select Object"))
			{
				ImGui::OpenPopup("Select Object##For Entities");
			}

			if (ImGui::BeginPopupModal("Select Object##For Entities")) {

				for (const auto& it : ASMGR.objects)
				{
					if (ImGui::Button(it.first.c_str()))
					{
						objName = it.first;
						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::EndPopup();
			}

			if (ImGui::Button("Select Shader"))
			{
				ImGui::OpenPopup("Select Shader##For Entities");
			}

			if (ImGui::BeginPopupModal("Select Shader##For Entities")) {

				for (const auto& it : ASMGR.shaders)
				{
					if (ImGui::Button(it.first.c_str()))
					{
						shaderName = it.first;
						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::EndPopup();
			}

			// You select the Object Name and the Shader Name.

			if ( ImGui::Button("Create"))
			{
				std::shared_ptr<Entity> new_ent = std::make_shared<Entity>();

				// Make sure that you have selected and stored the strings.
				PE_ASSERT(!objName.empty());
				PE_ASSERT(!shaderName.empty());

				new_ent->SetObjectName(objName);
				new_ent->SetShaderName(shaderName);

				entities.push_back(new_ent);
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

		if (displayDemoWindow) {
			ImGui::ShowDemoWindow(&displayDemoWindow);
		}

		if (displayViewportControls) {
			ImGui::Begin("View Port Controls", &displayViewportControls);

			// Expose all of the Viewport Data we currently have.

			static int selected_index = 0;

			ImGui::BeginChild("Viewports##List", ImVec2(150, 0), true);

			for (auto i = 0; i < 4; i++) {
				ImGui::PushID(&viewportsDetails[i]);
				std::string name = "Viewport " + std::to_string(i);
				if (ImGui::Selectable(name.c_str(), i == selected_index)) {
					selected_index = i;
				}
				ImGui::PopID();
			}

			ImGui::EndChild();

			ImGui::SameLine();

			ImGui::BeginGroup();

			ImGui::BeginChild("Details", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

			ImGui::Text("Viewport %d", selected_index);
			ImGui::Separator();

			ImGui::Text("Camera");
			// Display Camera Details here.
			viewportsDetails[selected_index].camera->DisplayCameraDetailsImgui();

			if (ImGui::Button("Select a Different Camera")) {

				ImGui::OpenPopup("Camera_Selector_For_Viewports");

			}

			if (ImGui::BeginPopupModal("Camera_Selector_For_Viewports")) {

				for (auto it : cameras) {
					if (ImGui::Button(it.first.c_str())) {
						viewportsDetails[selected_index].camera = it.second;
						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::EndPopup();
			}

			ImGui::Checkbox("Orthogonal", &viewportsDetails[selected_index].isOrthogonal);

			ImGui::EndChild();

			ImGui::EndGroup();

			ImGui::End();
		}

	}

	void TestScene::SaveScene(const char * _fileName)
	{

		std::string test_string = std::string(SCENES_FOLDER) + std::string(_fileName);
		const char * actual_file_name =  (test_string).c_str();

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

			// Store the Terrain
			// We need to store the Map Tile Data as well, that is on the heap.
			// TODO: Create a Save Terrain function for the Terrain, and a Load Terrain Function.
			testTerrain->SaveToFile(out);

			// Store the Viewport Details.
			for (auto i = 0; i < 4; i++) {
				out.write((char*)(&(viewportsDetails[i].isOrthogonal)), sizeof(bool));
				pe_helpers::store_strings(viewportsDetails[i].camera->GetCameraName(), out);
			}

		}

		out.close();
	}

	void TestScene::LoadScene(const char * _fileName)
	{

		std::string test_string = std::string(SCENES_FOLDER) + std::string(_fileName);
		const char * actual_file_name = (test_string).c_str();

		std::ifstream in(actual_file_name, std::ios::binary );

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

				std::string camera_index;

				std::shared_ptr<Camera> cam = std::make_shared<Camera>();

				pe_helpers::read_strings(camera_index, in);

				cam->LoadFromStream(in);

				cameras.insert_or_assign(camera_index, cam);

				if (nullptr == activeCamera) {
					activeCamera = cam;
					viewportsDetails[0].camera = activeCamera;
				}

			}

			// Load the Terrain
			ASMGR.objects.erase("terrain");
			testTerrain->LoadFromFile(in);

			// Load the Viewport Details
			for (auto i = 0; i < 4; i++) {
				in.read((char*)(&(viewportsDetails[i].isOrthogonal)), sizeof(bool));
				std::string camera_name;
				pe_helpers::read_strings(camera_name, in);
				viewportsDetails[i].camera = cameras.at(camera_name);
			}

		}

		in.close();

	}

}
