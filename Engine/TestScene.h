#pragma once

#include "Scene.h"
#include "Grid.h"
#include "Terrain.h"

#include "ScreenQuad.h"

namespace piolot {
	
	struct ImGuiControlVariables
	{
		bool& show_multiple_viewports;
	};

	// TODO: Camera/Viewport Stuff
	//////////////////////////////////////////////////////////////////////////
	// Add A Viewport Struct here to hold a pointer to the Current Camera, and the Projection Matrix Type.
	// We should be able to update the Current Camera for a specific viewport on the Fly.
	// Use ImGUi Camera Control panel and Change the Activate Camera to Set Camera to a Viewport.
	// Create a Modal with all the Viewports as an option. If a viewport other than 0 is selected, switch to the All View or maybe just the view of that camera.
	//////////////////////////////////////////////////////////////////////////

	struct ViewportDetails {

		// The Position in the array can be used as the Index
		//int index;
		std::shared_ptr<Camera> camera;
		bool isOrthogonal = false;

	};

	class TestScene : public Scene
	{

		// Stuff that should be saved
		Grid testGrid;
		std::shared_ptr<Terrain> testTerrain;

		/**
		 * \brief This would be a temporary entity that we draw when we are in the Placing Mode.
		 */
		std::unique_ptr<Entity> buildingPlacer;

		glm::vec2 startxz;
		glm::vec2 endxz;

		float totalTimeCounterForPathing = 0;

		std::vector<MapTile*> path;

		/* GUI Variables */
		bool pathingDebugWindow = false;
		bool displayAssetManagerWindow = false;
		bool displayLogWindow = false;
		bool displayCameraControls = false;
		bool displayRaypickingControls = false;
		bool displayDemoWindow = false;
		bool displayViewportControls = false;

		ViewportDetails viewportsDetails[4];

		// Save these as a nice QOL Feature.
		std::string filenameToSaveScene = "File Name";
		std::string filenameToLoadScene = "File Name";

		// Stuff that should not be saved.
		bool openSaveSceneAsWindow = false;
		bool openLoadSceneWindow = false;
		bool displayHierarchy = false;
		bool displayAddEntity = false;
		bool displayStats = true;

		bool isPlacingMode = false;

		// GUI Variables when you create a new Entity.

		std::string objName;
		std::string shaderName;

		// Debug the Ray.
		Ray cameraRay{glm::vec3(), glm::vec3()};

		/**
		 * \brief The Projection Matrix.
		 */
		glm::mat4 projectionMatrix;

	public:

		explicit TestScene(std::shared_ptr<Window> _window);
		~TestScene() = default;

		/**
		 * \brief Initialize all the Entities that shall be present in the Scene when it loads.
		 */
		void InitEntities() override;

		/**
		 * \brief Update the program by one frame.
		 * \param _deltaTime The Time from between the last frame and this frame
		 * \param _totalTime The Total Time since the Program started
		 */
		void OnUpdate(float _deltaTime, float _totalTime) override;

		void OnRender() override;

		/**
		 * \brief Render the ImGUI's current Frame.
		 * \param _vars The Variables that ImGui modifies and might need, to perform its functions
		 */
		virtual void OnImguiRender(ImGuiControlVariables& _vars);

		/**
		 * \brief Save the current Scene to a binary file.
		 * \param _fileName The Filename to save. No Extensions required.
		 */
		void SaveScene(const char * _fileName);

		/**
		 * \brief Load the Scene from the binary file.
		 * \param _fileName The Filename to load. As specific as you can get, with the extension.
		 */
		void LoadScene(const char * _fileName);

		void RayPicking();
		

	};

}
