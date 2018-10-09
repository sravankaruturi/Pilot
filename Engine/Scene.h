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

		/**
		 * \brief Raw Pointers to the Selected Entities.
		 * 
		 * These selected entities can be destroyed even though they are selected. So, we just copy the addresses.
		 */
		std::vector<Entity *> selectedEntities;

		/**
		 * \brief Raw Pointers to the Temporary Entities.
		 * 
		 * These entities are cleared every frame, after they are drawn.
		 */
		std::vector<std::unique_ptr<Entity>> tempEntities;

		float totalTime;
		float deltaTime;

	public:

		explicit Scene(std::shared_ptr<Window> _window);
		~Scene() = default;

		virtual void InitEntities() {};

		virtual void OnUpdate(float _deltaTime, float _totalTime) {
			deltaTime = _deltaTime;
			totalTime = _totalTime;

			tempEntities.clear();
		}

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
