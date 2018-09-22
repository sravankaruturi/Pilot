#pragma once

#include "Scene.h"
#include "Grid.h"
#include "Terrain.h"

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

		// GUI Variables when you create a new Entity.

		std::string objName;
		std::string shaderName;


	public:

		explicit TestScene(std::shared_ptr<Window> _window);
		~TestScene() = default;

		void InitEntities() override;

		virtual void OnUpdate(float _deltaTime, float _totalTime) override;

		virtual void OnRender() override;

		virtual void OnImguiRender(ImGuiControlVariables& _vars);

		/**
		 * \brief Save the current Scene to a binary file.
		 * \param _fileName The Filename to save. No Extensions required.
		 */
		void SaveScene(const char * _fileName);

		void LoadScene(const char * _fileName);

	};

}
