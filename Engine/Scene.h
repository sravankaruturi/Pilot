#pragma once

#include <memory>
#include <unordered_map>

#include "Camera.h"
#include "Entity.h"
#include "AnimatedEntity.h"

class Window;

namespace pilot {

	class Scene
	{

	protected:

		/**
		 * \brief The Shared Pointer for the Window.
		 */
		std::shared_ptr<Window> window;

		/**
		 * \brief The Camera that is currently Active.
		 */
		std::shared_ptr<Camera> activeCamera;

		/**
		 * \brief The Map of all the Cameras that are currently in the Scene.
		 * 
		 * We use Unordered Map because when we disable the UI, we have a slight performance benefit for the UnOrdered Map. This is definitely worse when you have the UI Enabled.
		 */
		std::unordered_map<std::string, std::shared_ptr<Camera>> cameras;

		/**
		 * \brief All the Entities.
		 * 
		 * The Scene owns them. When you delete the Scene, the entities are deleted.
		 */
		std::vector<std::shared_ptr<Entity>> entities;

		/**
		 * \brief All the Entities with Animations.
		 * 
		 * The Scene owns them. When you delete the Scene, the entities are deleted.
		 */
		std::vector<std::shared_ptr<AnimatedEntity>> animatedEntities;

		/**
		 * \brief Number of seconds to wait before erasing the To be Deleted Entities.
		 */
		float tbdTimer = 5.0f;

		/**
		 * \brief Entities Marked For Deletion, and the time, they were loaded.
		 */
		std::vector<std::pair<std::shared_ptr<AnimatedEntity>, float>> tbdAnimatedEntities;

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

		/**
		 * \brief Total time since the Window Opened.
		 * 
		 * Populated by using glfwgettime() function.
		 * @see https://www.glfw.org/docs/3.0/group__time.html
		 */
		float totalTime;

		/**
		 * \brief The time elapsed since the last frame.
		 */
		float deltaTime;

		/**
		 * \brief Should the Scene Display Multiple Views.
		 */
		bool displayMultipleViews = false;

		/**
		 * \brief Should we shut the Scene down?
		 */
		bool shutDown = false;

	public:

		explicit Scene(std::shared_ptr<Window> _window);
		virtual ~Scene() = default;

		/**
		 *  \defgroup Virtual functions that are supposed to be overwritten.
		 *  @{
		 **/

		/**
		 * \brief Function to be overwritten to Initialize the Entities
		 */
		virtual void InitEntities() = 0;

		virtual void OnUpdate(float _deltaTime, float _totalTime) {
			deltaTime = _deltaTime;
			totalTime = _totalTime;

			tempEntities.clear();
		}

		virtual void OnRender() = 0;

		/**
		 * \brief Render the ImGUI Stuff
		 * 
		 * This can be completely overwritten and ignored and doesn't have to be implemented.
		 */
		virtual void OnImguiRender(){}

		/**
		 *@}
		**/

		/**
		* \brief Activate the passed Camera.
		* \param val The Camera to Activate
		**/
		void ActivateCamera(std::shared_ptr<Camera> _val) { activeCamera = _val; }

		/**
		 * \brief Get the ActiveCamera
		 * \return Returns a pointer to the Current Active Camera
		 */
		std::shared_ptr<Camera> GetActiveCamera() const { return activeCamera; }

		/**
		 * \brief This must be implemented.
		 */
		virtual void RunScene() = 0;
		
	};

	inline Scene::Scene(std::shared_ptr<Window> _window)
		: window(_window)
	{

	}
}
