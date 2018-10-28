#pragma once
#include "TestScene.h"
#include "FolderLocations.h"
#include "../EngineDeps/external_files/ImGUI/imgui.h"
#include <glm/gtc/matrix_transform.inl>
#include "Window.h"
#include "AssetManager.h"


void piolot::TestScene::OnImguiRender(ImGuiControlVariables& _vars)
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
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Pathing Debug Window"))
			{
				pathingDebugWindow = true;
			}
			if (ImGui::MenuItem("Asset Manager Debug Window"))
			{
				displayAssetManagerWindow = true;
			}
			if (ImGui::MenuItem("Log Window"))
			{
				displayLogWindow = true;
			}
			if (ImGui::MenuItem("Ray Picking Debug Window")) {
				displayRaypickingControls = true;
			}

			if (ImGui::MenuItem("Demo Window")) {
				displayDemoWindow = true;
			}

			if (ImGui::MenuItem("Hierarchy"))
			{
				displayHierarchy = true;
			}

			if (ImGui::MenuItem("Stats")) {
				displayStats = true;
			}

			ImGui::EndMenu();
		}


		if (ImGui::BeginMenu("View")) {

			if (ImGui::MenuItem("Cameras")) {
				displayCameraControls = true;
			}

			if (ImGui::MenuItem(_vars.show_multiple_viewports ? "Hide Other Views" : "Show All Views"))
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

		if (ImGui::BeginMenu("Action")) {
			if (ImGui::MenuItem("Place a New Building")) {
				isPlacingMode = true;
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

			if (ImGui::Button(file_name_temp.c_str())) {

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

	if (pathingDebugWindow)
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

		ImGui::Separator();

		if (!selectedEntities.empty()) {

			glm::vec3 ent_pos = selectedEntities[0]->GetPosition();
			glm::ivec2 ent_node = testTerrain->GetNodeIndicesFromPos(ent_pos.x, ent_pos.z);

			ImGui::InputFloat3("Position: ", glm::value_ptr(ent_pos));

			ImGui::DragInt2("Corresponding Node", glm::value_ptr(ent_node));

		}
		else {
			ImGui::Text("No Entity selected to debug pathing.");
		}

		ImGui::Separator();

		ImGui::Text("Fighting Variables");

		if (!selectedEntities.empty()) {

			ImGui::Checkbox("Attacking Mode: ", &(selectedEntities[0]->attackingMode));

		}
		else {
			ImGui::Text("No Entity selected to debug Fighting.");
		}

		ImGui::End();

	}

	if (displayHierarchy)
	{

		ImGui::Begin("Hierarchy", &displayHierarchy);

		//TODO: This should be highlighted in the Viewport as well.

		// Set the selected flag for the entity.
		ImGui::BeginChild("Entities##List", ImVec2(200, 0), true);

		ImGui::Separator();
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Entities");

		// Loop through all the Entities
		for (auto& it : entities)
		{
			ImGui::PushID(&it);
			if (ImGui::Selectable(it->GetEntityName().c_str(), (std::find(selectedEntities.begin(), selectedEntities.end(), it.get()) != selectedEntities.end())))
			{
				// It is already selected. If they click here, make sure that this is the only selected thing.
				selectedEntities.clear();
				selectedEntities.push_back(it.get());
			}
			ImGui::PopID();

		}

		ImGui::Separator();
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Animated Entities");

		for (auto& it : animatedEntities) {
			ImGui::PushID(&it);
			if (ImGui::Selectable(it->GetEntityName().c_str(), (std::find(selectedEntities.begin(), selectedEntities.end(), it.get()) != selectedEntities.end())))
			{
				// It is already selected. If they click here, make sure that this is the only selected thing.
				selectedEntities.clear();
				selectedEntities.push_back(it.get());
			}
			ImGui::PopID();
		}

		ImGui::Separator();
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Temporary Entities");
		for (auto& it : tempEntities)
		{
			ImGui::PushID(&it);
			if (ImGui::Selectable(it->GetEntityName().c_str(), (std::find(selectedEntities.begin(), selectedEntities.end(), it.get()) != selectedEntities.end())))
			{
				// It is already selected. If they click here, make sure that this is the only selected thing.
				selectedEntities.clear();
				selectedEntities.push_back(it.get());
			}
			ImGui::PopID();
		}

		ImGui::EndChild();

		ImGui::SameLine();
		// Details of the selected Entity.
		ImGui::BeginGroup();

		{
			if (!selectedEntities.empty() && selectedEntities[0] != nullptr) {
				ImGui::BeginChild("Details", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

				ImGui::Text(selectedEntities[0]->GetEntityName().c_str());
				ImGui::Separator();

				selectedEntities[0]->DisplayDetailsImgui();

				ImGui::EndChild();
			}
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

		projectionMatrix = glm::perspective(45.0f, float(window->GetWidth()) / window->GetHeight(), 0.1f, 100.0f);
		glm::vec3 mouse_pointer_ray;

		if (viewport_size[2] == window->GetWidth() && viewport_size[3] == window->GetHeight()) {
			updated_x = window->mouseX;
			updated_y = window->mouseY;

			mouse_pointer_ray = activeCamera->GetMouseRayDirection(window->mouseX, window->mouseY, window->GetWidth(), window->GetHeight(), projectionMatrix);

		}
		else {
			// Set Which viewport this is hovering over here.
			updated_x = (window->mouseX > window->GetWidth() / 2.0f) ? window->mouseX - window->GetWidth() / 2.0f : window->mouseX;
			updated_y = (window->mouseY > window->GetHeight() / 2.0f) ? window->mouseY - window->GetHeight() / 2.0f : window->mouseY;

			updated_x *= 2;
			updated_y *= 2;

			mouse_pointer_ray = activeCamera->GetMouseRayDirection(updated_x, updated_y, window->GetWidth(), window->GetHeight(), projectionMatrix);
		}

		ImGui::InputFloat3("Mouse Pointer Ray", glm::value_ptr(mouse_pointer_ray));

		ImGui::Text("Updated Mouse position: %.3f , %.3f", updated_x, updated_y);

		ImGui::Separator();

		ImGui::Checkbox("Pointing at Origin: ", &this->testTerrain->pointingAtOrigin);

		ImGui::SliderFloat("The Accuracy Parameter: ", &this->testTerrain->accuracyFactor, 0.1f, 1.0f);

		ImGui::DragInt2("The selected Node Indices", glm::value_ptr(testTerrain->pointedNodeIndices), 0, 20);

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

		if (ImGui::Button("Create"))
		{
			// Make sure that you have selected and stored the strings.
			PE_ASSERT(!objName.empty());
			PE_ASSERT(!shaderName.empty());

			entities.push_back(std::make_unique<Entity>());

			Entity * new_ent = entities.back().get();

			new_ent->SetObjectName(objName);
			new_ent->SetShaderName(shaderName);

		}

		ImGui::End();

	}

	if (displayStats) {

		ImGui::Begin("Add an Entity", &displayAddEntity);

		float fps = 1.0f / deltaTime;

		std::string text = "Framerate " + std::to_string(fps);

		ImGui::Text(text.c_str());

		ImGui::End();

	}

	if (displayAssetManagerWindow)
	{
		ASMGR.GuiRender(&displayAssetManagerWindow);
	}

	if (displayLogWindow)
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

