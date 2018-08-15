#pragma once

#include "Scene.h"

namespace piolot {

	class TestScene : public Scene
	{
	public:

		explicit TestScene(std::shared_ptr<Window> _window);
		~TestScene() = default;

		virtual void OnUpdate(float _deltaTime) override;


		virtual void OnRender() override;


		virtual void OnImguiRender() override;

	};

}