#pragma once
#include "Entity.h"
#include <memory>
#include "Object.h"

#include <fstream>

namespace piolot {
	class Ray;

	struct TerrainVertexData{

		long header = 11110000;

		glm::vec4 position;
		glm::vec4 normal;
		glm::vec4 colour;
		glm::vec4 texCoord;
	};

	class MapTile {

	public:

		int tileIndexX;
		int tileIndexZ;

		float tilePosX;
		float tilePosZ;
		float tilePosY;


		/* Calculated Navigation Stuff */
		float navCost = 0.0f;

		/**
		 * \brief Is the Tile walkable or does it have any static elements attached to it.
		 */
		bool navWalkable = true;

		/**
		 * \brief Is there a dynamic obstacle in the path.
		 */
		bool navObstacle = false;
		MapTile * navNeighbours[8];
		int navNeighbourCount = 0;
		// Open Set?
		bool navOpen;
		// Closed Set?
		bool navClosed;
		MapTile * navParent = nullptr;
		float navFCost = 0;
		float navGCost = 0;
		int navTileSet;

	public:
		MapTile() = default;

		glm::vec3 GetPosition() const {
			return glm::vec3(tilePosX, tilePosY, tilePosZ);
		}

	};

	class Terrain : public Entity
	{

	private:
		unsigned int length, breadth;
	
		float gridLength, gridBreadth;

		unsigned int nodeCountX{}, nodeCountZ{};

		MapTile ** tiles{};

		std::string heightMapFile;

		std::vector<TerrainVertexData> vertices;

		bool areVerticesDirty = false;

		std::vector<unsigned int> indices;

		std::shared_ptr<Object> objectPtr;

		/**
		 * \brief This is a float that can be used to modify the amplitude of the Terrain.
		 * 
		 * This is also used as Standard Deviation to Caclulate the Ray Intersection Point with the Terrain.
		 */
		float heightFactor = 1.0f;

		glm::vec3 ComputeGridNormal(int _x,int _z);

		/* Testing stuff */
		glm::vec2 startxz{};
		glm::vec2 endxz{};

	public:

		/**
		 * \brief This is set to True, at Run time if you are pointing the Mouse Cursor over the Terrain origin.
		 */
		bool pointingAtOrigin = false;

		/**
		 * \brief This would be the Current Indices that are being pointed to by, the Mouse.
		 * 
		 * By Default, they are set to store INT_MAX. Unless they are being Updated.
		 */
		glm::ivec2 pointedNodeIndices{INT_MAX, INT_MAX};

		/**
		 * \brief The Accuracy by which we say, that a Ray has a Point on it. Lower is more Accurate.
		 */
		float accuracyFactor = 0.2f;

	public:

		unsigned GetLength() const
		{
			return length;
		}

		unsigned GetBreadth() const
		{
			return breadth;
		}

		float GetGridLength() const
		{
			return gridLength;
		}

		float GetGridBreadth() const
		{
			return gridBreadth;
		}

		unsigned GetNodeCountX() const
		{
			return nodeCountX;
		}

		unsigned GetNodeCountZ() const
		{
			return nodeCountZ;
		}

		MapTile** GetTiles() const
		{
			return tiles;
		}

		const std::string& GetHeightMapFile() const
		{
			return heightMapFile;
		}

		const std::vector<TerrainVertexData>& GetVertices() const
		{
			return vertices;
		}

		const std::vector<unsigned>& GetIndices() const
		{
			return indices;
		}

		const std::shared_ptr<Object>& GetObjectPtr() const
		{
			return objectPtr;
		}

		Terrain(int _mapLength, int _mapBreadth, float _gridLength, float _gridBreadth, std::string _heightMapFile);

		void Init();

		void Render();

		void Update(float _delatTime, float _totalTime);

		float GetHeightAtPos(const float& _x, const float& _z);
		float GetHeightForNode(const int& _x, const int& _z);

		glm::ivec2 GetNodeIndicesFromPos(const float& _x, const float& _z) const;

		void HighlightNode(const unsigned int _x, const unsigned int _z);

		void ClearColours();

		std::vector<MapTile *> GetPathFromTiles(MapTile * _startTile, MapTile * _endTile);
		std::vector<MapTile *> GetPathFromPositions(glm::vec3, glm::vec3);

		MapTile * GetTileFromIndices(int _x, int _y);

		void InitPathFinding();

		void FillNeighbours(MapTile& _tile);

		void OnImguiRender();

		int GetNodeSetFromPos(float _x, float _z);

		std::vector<int> GetAllTileSets();

		void SaveToFile(std::ofstream& _out);

		void LoadFromFile(std::ifstream& _in);

		void DeleteTiles() const;

		~Terrain();

		/**
		 * \brief Resets the Obstacle during Path.
		 */
		void ResetObstacles();

		/* Terrain Debug */
		bool terrainDebug = false;

		/**
		 * \brief Get the Intersection Point for the Ray and the Terrain.
		 * \param _ray The Mouse Ray, based on the Current Mouse Position and the View Matrix.
		 * \param _granularity The Granularity with which to search for the Intersection.
		 * 
		 * This is used in pathfinding.
		 */
		void GetMouseRayPoint(Ray _ray, float _granularity = 0.5f);

		void SetTerrainNodeNotWalkable(glm::ivec2 _nodeIndices);

	};


}
