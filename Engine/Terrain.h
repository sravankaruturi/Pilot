#pragma once
#include "Entity.h"
#include <memory>
#include "Object.h"

namespace piolot {

	struct TerrainVertexData{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 colour;
		glm::vec3 texCoord;
	};

	class MapTile {

	public:

		int tileIndexX;
		int tileIndexZ;

		float tilePosX;
		float tilePosZ;
		float tilePosY;

	public:
		MapTile() = default;

	};

	class Terrain : public Entity
	{

		unsigned int length, breadth;
		
		float gridLength, gridBreadth;

		unsigned int nodeCountX, nodeCountZ;

		MapTile ** tiles;

		std::string heightMapFile;

		std::vector<TerrainVertexData> vertices;

		std::vector<unsigned int> indices;

		std::shared_ptr<Object> objectPtr;

		glm::vec3 ComputeGridNormal(int _x,int _z);

	public:

		Terrain(int _mapLength, int _mapBreadth, float _gridLength, float _gridBreadth, std::string _heightMapFile);

		void Render();

		~Terrain();

	};


}
