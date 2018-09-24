#pragma once

#include <memory>
#include <map>

#include "Camera.h"
#include "Entity.h"
#include "AnimatedEntity.h"

class Window;

namespace piolot {

	class Scene
	{

	protected:

		std::shared_ptr<Window> window;

		std::shared_ptr<Camera> activeCamera;

		std::map<std::string, std::shared_ptr<Camera>> cameras;

		std::vector<std::shared_ptr<Entity>> entities;
		std::vector<std::unique_ptr<AnimatedEntity>> animatedEntities;

		//std::vector<std::weak_ptr<AnimatedEntity>> selectedEntities;
		Entity * selectedEntity;

	public:

		explicit Scene(std::shared_ptr<Window> _window);
		~Scene() = default;

		virtual void InitEntities() {};

		virtual void OnUpdate(float _deltaTime, float _totalTime) {}

		virtual void OnRender() {}

		virtual void OnImguiRender() {}

		std::shared_ptr<piolot::Camera> ActiveCamera() const { return activeCamera; }
		void ActiveCamera(std::shared_ptr<piolot::Camera> val) { activeCamera = val; }
	};

	inline Scene::Scene(std::shared_ptr<Window> _window)
		: window(_window)
	{

	}
}
