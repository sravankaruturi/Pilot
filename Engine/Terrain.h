#pragma once
#include "Entity.h"
#include <memory>
#include "Object.h"

#include <fstream>

namespace piolot {
	class Ray;

	/**
	 * \brief The Struct to represent the Vertex Data of the Terrain
	 */
	struct TerrainVertexData{

		long header = 11110000;

		glm::vec4 position{};
		glm::vec4 normal{};
		glm::vec4 colour{};
		glm::vec4 texCoord{};
	};

	/**
	 * \brief This represents a Tile in the Terrain.
	 */
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

		/**
		 * \brief Pointers to the Neighbours of this Tile.
		 */
		MapTile * navNeighbours[8];

		/**
		 * \brief Number of Neighbours. Should be in between 3 and 8.
		 */
		int navNeighbourCount = 0;

		/**
		 * \brief Does this tile belong to the OpenSet.
		 */
		bool navOpen;

		/**
		 * \brief Does this tile belong in the Closed Set.
		 */
		bool navClosed;

		/**
		 * \brief Which Tile did you arrive at this tile, from?
		 */
		MapTile * navParent = nullptr;

		float navFCost = 0;
		float navGCost = 0;

		/**
		 * \brief Signify which kind of entity occupies this Tile.
		 * 
		 * We use this for prinitng mini maps, checking if we can move through a tile ( if it contains enemies. )
		 * 0 -- None
		 * 1 -- Team 1, Unit
		 * 2 -- Team 2, Unit
		 * 11 -- Team 1, Building
		 * 22 -- Team 2, Building
		 */
		int occupiedBy = 0;

		/**
		 * \brief The TileSet that this Tile belongs to.
		 * 
		 * If two tiles belong to two different Tile Sets, there exists no path between them.
		 */
		int navTileSet;

		MapTile() = default;

		/**
		 * \brief Converts the Tile Position to a Vec3 and returns it.
		 * \return Position as a Vec3
		 */
		glm::vec3 GetPosition() const;
	};

	/**
	 * \brief Container of all Terrain functions. Is Derived from Entity.
	 */
	class Terrain : public Entity
	{

		unsigned int length, breadth;
	
		float gridLength, gridBreadth;

		unsigned int nodeCountX{}, nodeCountZ{};

		MapTile ** tiles{};

		/**
		 * \brief The Filename, from which this terrain is based on.
		 */
		std::string heightMapFile;

		/**
		 * \brief The vertices we use to create the Mesh.
		 */
		std::vector<TerrainVertexData> vertices;

		/**
		 * \brief Flag to Check if we need to update the Buffer.
		 */
		bool areVerticesDirty = false;

		/**
		 * \brief The Indices
		 */
		std::vector<unsigned int> indices;

		/**
		 * \brief Pointer of the Created Object/Mesh.
		 */
		std::shared_ptr<Object> objectPtr;

		/**
		 * \brief This is a float that can be used to modify the amplitude of the Terrain.
		 * 
		 * This is also used as Standard Deviation to Calculate the Ray Intersection Point with the Terrain.
		 */
		float heightFactor = 1.0f;

		/**
		 * \brief Computes the Normal based on its surrounding Nodes.
		 * \param _x Node Index X
		 * \param _z Node Index Z
		 * \return Normal at Index X, Z
		 */
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

		/**
		 * \brief Create a Terrain based on the Height Map Image
		 * \param _mapLength The Length of the Terrain in the World Coordinates
		 * \param _mapBreadth The Breadth of the Terrain in the world Co-ordinates
		 * \param _gridLength Length of each Grid in the Terrain
		 * \param _gridBreadth Breadth of each Tile in the Terrain
		 * \param _heightMapFile The File to base the Height Map on
		 */
		Terrain(int _mapLength, int _mapBreadth, float _gridLength, float _gridBreadth, std::string _heightMapFile);

		/**
		 * \brief Initialize the Terrain
		 * 
		 * 1. Load the Image and parse it.
		 * 2. Create Vertices based in the Lengths and breadths passed.
		 * 3. Calculate Normals for each vertex.
		 * 4. Create the Object and push it to the Asset Manager to Render.
		 */
		void Init();

		/**
		 * \brief Render the Terrain.
		 */
		void Render();

		/**
		 * \brief Update the Terrain
		 * \param _delatTime Delta Time
		 * \param _totalTime Total Time
		 * 
		 * Push to the Buffer, if the Vertices are dirty.
		 */
		void Update(float _delatTime, float _totalTime);

		/**
		 * \brief Gets the Height at a position in this terrain. All values are in World Space.
		 * \param _x X Position
		 * \param _z Z Position
		 * \return Y Position
		 */
		float GetHeightAtPos(const float& _x, const float& _z);

		/**
		 * \brief Gets the Height of a Node in this Terrain. All the values are in World Space.
		 * \param _x Node Index X
		 * \param _z Node Index Z
		 * \return Height at Node X, Z
		 */
		float GetHeightForNode(const int& _x, const int& _z);


		/**
		 * \brief Get the Indices of the Node Corresponding to this Position
		 * \param _position Position Vector3
		 * \return Node Indices
		 */
		glm::ivec2 GetNodeIndicesFromPos(const glm::vec3 _position);

		/**
		 * \brief Get the Indices of the Node corresponding to this Position
		 * \param _x Position X
		 * \param _z Position Z
		 * \return The Node Indices, as an iVec2
		 */
		glm::ivec2 GetNodeIndicesFromPos(const float& _x, const float& _z) const;

		/**
		 * \brief Highlight the Node (X, Z) with Yellow Colour
		 * \param _x Node Index X
		 * \param _z Node Index Z
		 */
		void HighlightNode(unsigned int _x, unsigned int _z);

		bool CanPlaceHere(unsigned int _x, unsigned int _z);

		/**
		 * \brief Clear any highlights we made in the previous frame.
		 */
		void ClearColours();

		/**
		 * \brief Get the Path from _startTile to _endTile
		 * \param _startTile The Map Tile where you start
		 * \param _endTile The Map Tile where you end
		 * \return A Vector of Tile, the path to take.
		 */
		std::vector<MapTile *> GetPathFromTiles(MapTile * _startTile, MapTile * _endTile);

		/**
		 * \brief Get the Path from start position to the end position
		 * \return A Vector of Tiles, the path to take.
		 */
		std::vector<MapTile *> GetPathFromPositions(glm::vec3, glm::vec3);

		/**
		 * \brief Get the Node at the Node Indices
		 * \param _nodeIndices Node Indices Vec2
		 * \return Pointer to the Tile at the _nodeIndices
		 */
		MapTile * GetTileFromIndices(glm::ivec2 _nodeIndices);

		/**
		 * \brief Get the Node at (x, z)
		 * \param _x Node Index X
		 * \param _y Node Index Z
		 * \return Pointer to the Tile at (x, z)
		 */
		MapTile * GetTileFromIndices(int _x, int _y);

		/**
		 * \brief Initialize the necessary variables for Pathfinding.
		 * 
		 * 1. Calculate the Cost of Each Tile based on the Normals and the Height.
		 * 2. Calculate the Tilesets.
		 * 3. Calculate if you can actually reach that node, ever, or not.
		 */
		void InitPathFinding();

		/**
		 * \brief Populate the neighbours list of the Tile.
		 * \param _tile The Tile to perform this operation on.
		 */
		void FillNeighbours(MapTile& _tile);

		void OnImguiRender();

		int GetNodeSetFromPos(float _x, float _z);

		/**
		 * \brief Get a List of all the Tilesets.
		 * \return List of all the TileSets in the Terrain when called.
		 */
		std::vector<int> GetAllTileSets();

		/**
		 * \brief Save this to the Output Stream.
		 * \param _out Output Stream Reference
		 */
		void SaveToFile(std::ofstream& _out);

		/**
		 * \brief Load the Terrain from the Input Stream
		 * \param _in Input Stream Reference
		 */
		void LoadFromFile(std::ifstream& _in);

		/**
		 * \brief Delete the Tiles ( Tiles are allocated on the Heap. Destroy Them )
		 */
		void DeleteTiles() const;

		~Terrain();

		/**
		 * \brief Resets the Obstacle during Path.
		 */
		void ResetObstacles();

		/**
		 * \brief Reset all the tiles to, Occupied by none
		 */
		void ResetOccupiedBy();

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


		void SetTerrainNodeObstacle(glm::ivec2 _nodeIndices);


	};


}
