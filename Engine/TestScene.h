#pragma once

#include "Scene.h"
#include "Grid.h"
#include "Terrain.h"

namespace piolot {

	class TestScene : public Scene
	{

		Grid test;
		std::shared_ptr<Terrain> terrain_test;

	public:

		explicit TestScene(std::shared_ptr<Window> _window);
		~TestScene() = default;

		virtual void OnUpdate(float _deltaTime) override;


		virtual void OnRender() override;


		virtual void OnImguiRender() override;

	};

}
