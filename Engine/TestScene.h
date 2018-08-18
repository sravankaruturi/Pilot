#pragma once

#include "Scene.h"
#include "Grid.h"
#include "Terrain.h"

namespace piolot {

	class TestScene : public Scene
	{

		Grid test;
		std::shared_ptr<Terrain> terrain_test;

		//glm::vec3 startPosition;
		//glm::vec3 endPosition = glm::vec3(1.0f, 1.0f, 1.0f);

		glm::vec2 startxz;
		glm::vec2 endxz;

		std::vector<MapTile*> path;

	public:

		explicit TestScene(std::shared_ptr<Window> _window);
		~TestScene() = default;

		virtual void OnUpdate(float _deltaTime, float _totalTime) override;


		virtual void OnRender() override;


		virtual void OnImguiRender() override;

	};

}
