#pragma once

#include "Scene.h"
#include "Grid.h"
#include "Terrain.h"

namespace piolot {

	struct ImGuiControlVariables
	{
		bool& show_multiple_viewports;
	};

	class TestScene : public Scene
	{

		Grid testGrid;
		std::shared_ptr<Terrain> testTerrain;

		//glm::vec3 startPosition;
		//glm::vec3 endPosition = glm::vec3(1.0f, 1.0f, 1.0f);

		glm::vec2 startxz;
		glm::vec2 endxz;

		std::vector<MapTile*> path;

		/* GUI Variables */
		bool pathingDebugWindow = false;
		bool displayAssetManagerWindow = false;
		bool displayLogWindow = false;
		bool displayCameraControls = false;
		bool displayRaypickingControls = true;
		bool displayDemoWindow = false;

	public:

		explicit TestScene(std::shared_ptr<Window> _window);
		~TestScene() = default;

		virtual void OnUpdate(float _deltaTime, float _totalTime) override;

		virtual void OnRender() override;

		virtual void OnImguiRender(ImGuiControlVariables& _vars);

	};

}
