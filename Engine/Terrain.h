#pragma once
#include "Entity.h"
#include <memory>
#include "Object.h"

#include <fstream>

namespace piolot {

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
		bool navWalkable = true;
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

		glm::vec3 GetPosition() {
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

		glm::vec3 ComputeGridNormal(int _x,int _z);

		/* Testing stuff */
		glm::vec2 startxz{};
		glm::vec2 endxz{};

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

		glm::vec2 GetNodeIndicesFromPos(const float& _x, const float& _z) const;

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

		/* Terrain Debug */
		bool terrainDebug = false;

	};


}
