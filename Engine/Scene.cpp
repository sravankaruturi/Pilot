#pragma once

#include "Scene.h"
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

namespace pilot
{
	
	Scene::Scene(std::shared_ptr<Window> _window)
		: window(_window)
	{
		ASMGR.ClearAllData();
		ASMGR.LoadShaders();
		ASMGR.LoadTextures();
	}

	void Scene::HandleInputs()
	{

		window->HandleInput();
		shutDown = glfwWindowShouldClose(window->GetWindow());

	}

}