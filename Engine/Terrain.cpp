#include "Terrain.h"
#include "external_files/stb/stb_image.h"
#include "Colours.h"
#include "LoggingManager.h"
#include "AssetManager.h"

namespace piolot {

	glm::vec3 Terrain::ComputeGridNormal(const int _x, const int _z)
	{

		if (_x > nodeCountX || _z > nodeCountZ || _x < 0 || _z < 0)
		{
			LOGGER.AddToLog("Out of Bounds error when generating normals for Terrain", PE_LOG_ERROR);
		}

		// If X and Z are the co ordinates. Get the Four surrounding Vertices it would be connected to.

		/*		B
		*		|
		*C --- O ---- A
		*		|
		*		D
		*
		*		O is the current Vertex. A - D are the surrounding vertices. We compute the normals by computing the cross product for,
		*		OA * OB
		*		OB * OC
		*		OC * OD
		*		OD * OA
		*
		*		And averaging it.
		*/

		glm::vec3 o_position = vertices[_x * nodeCountZ + _z].position;

		glm::vec3 a_position = vertices[((_x + 1 < nodeCountX) ? _x + 1 : _x) * nodeCountZ + _z].position;
		auto tester_temp = _x * nodeCountZ + (_z - 1 > nodeCountZ ? _z : _z - 1);
		glm::vec3 b_position = vertices[_x * nodeCountZ + ((_z - 1 < 0) ? _z : _z - 1)].position;
		glm::vec3 c_position = vertices[((_x - 1 < 0) ? _x : _x - 1) * nodeCountZ + _z].position;
		glm::vec3 d_position = vertices[_x * nodeCountZ + ((_z + 1 < nodeCountZ) ? _z + 1 : _z)].position;

		glm::vec3 oa = a_position - o_position;
		glm::vec3 ob = b_position - o_position;
		glm::vec3 oc = c_position - o_position;
		glm::vec3 od = d_position - o_position;

		glm::vec3 normal = glm::normalize(
			glm::normalize(glm::cross(oa, ob)) +
			glm::normalize(glm::cross(ob, oc)) +
			glm::normalize(glm::cross(oc, od)) +
			glm::normalize(glm::cross(od, oa))
		);

		return normal;

	}

	Terrain::Terrain(int _mapLength, int _mapBreadth, float _gridLength, float _gridBreadth, std::string _heightMapFile)
		: length(_mapLength), breadth(_mapBreadth), gridLength(_gridLength), gridBreadth(_gridBreadth), heightMapFile(_heightMapFile)
	{

		nodeCountX = (length / gridLength) + 1;
		nodeCountZ = (breadth / gridBreadth) + 1;

		tiles = new MapTile*[nodeCountX];

		for (auto i = 0; i < nodeCountX; i++) {
			tiles[i] = new MapTile[nodeCountZ];
		}

		// Load the Image.
		stbi_set_flip_vertically_on_load(true);
		int image_width, image_height, nr_channels;

		auto data = stbi_load(_heightMapFile.c_str(), &image_width, &image_height, &nr_channels, 0);

		for (auto i = 0; i < nodeCountX; i++)
		{
			for (auto j = 0; j < nodeCountZ; j++)
			{
				auto image_x = int(i * image_width / nodeCountX);
				auto image_z = int(j * image_height / nodeCountZ);

				float total = 0;

				auto channels_without_alpha = (nr_channels == 2 || nr_channels == 4) ? (nr_channels - 1) : nr_channels;

				// Do not count the Alpha
				for (auto it = 0; it < channels_without_alpha; it++)
				{
					total += int(data[((image_x + (image_z * image_width)) * nr_channels)] + it);
				}

				// To make it so that, the blacks are higher and the whites are lower, we subtract the values from pure white.
				total = ((255.0f * channels_without_alpha) - total) / (channels_without_alpha * 255.0f);

				tiles[i][j].tileIndexX = i;
				tiles[i][j].tileIndexZ = j;

				tiles[i][j].tilePosX = i * gridLength + gridLength / 2;
				tiles[i][j].tilePosZ = j * gridBreadth + gridBreadth / 2;

				tiles[i][j].tilePosY = total;
			}
		}

		this->vertices.resize(nodeCountX * nodeCountZ);

		for (auto i = 0; i < nodeCountX; i++) {
			for (auto j = 0; j < nodeCountZ; j++) {
				vertices[i * nodeCountZ + j] = TerrainVertexData();
				vertices[i * nodeCountZ + j].position = glm::vec3(tiles[i][j].tilePosX, tiles[i][j].tilePosY, tiles[i][j].tilePosZ);
				vertices[i * nodeCountZ + j].normal = glm::vec3();
				vertices[i * nodeCountZ + j].colour = red;
				vertices[i * nodeCountZ + j].texCoord = glm::vec3(i * 0.4, j * 0.4, 0);
			}
		}

		for (auto i = 0; i < nodeCountX - 1; i++)
		{
			for (auto j = 0; j < nodeCountZ - 1; j++)
			{
				indices.push_back(i * (nodeCountZ)+j);
				indices.push_back(i * (nodeCountZ)+j + 1);
				indices.push_back((i + 1) * (nodeCountZ)+j);

				indices.push_back((i + 1)* (nodeCountZ)+j);
				indices.push_back(i * (nodeCountZ)+j + 1);
				indices.push_back((i + 1) * (nodeCountZ)+j + 1);
			}
		}

		for (auto i = 0; i < nodeCountX; i++)
		{
			for (auto j = 0; j < nodeCountZ; j++) {

				vertices[i * nodeCountZ + j].normal = ComputeGridNormal(i, j);

			}
		}

		auto mesh = std::make_shared<Mesh>(&vertices[0], sizeof(TerrainVertexData), vertices.size(), indices);
		mesh->SetTextureNames(std::vector<std::string>{"grass"});

		objectPtr = std::make_shared<Object>("terrain", std::vector<std::shared_ptr<Mesh>>{mesh});

		if (!ASMGR.AddToObjects("terrain", objectPtr)) {
			LOGGER.AddToLog("Cannot load terrain into AssetManager", PE_LOG_ERROR);
		}

		this->objectName = "terrain";
		this->shaderName = "terrain";

		// Maybe push this mesh and texture data on to the Asset Manager, which we would use to draw stuff in render?

	}

	void Terrain::Render()
	{
		Entity::Render();
	}

	//void Terrain::Render()
	//{
	//	Entity::Render();
	//	//ASMGR.shaders.at("terrain")->use();
	//	//ASMGR.shaders.at("terrain")->setMat4("u_ModelMatrix", glm::mat4(1.0));
	//	//ASMGR.shaders.at("terrain")->setInt("u_texture0", 0);

	//	//glActiveTexture(GL_TEXTURE0);
	//	//???
	//	//glBindTexture(0, ASMGR.textures.at("grass")->GetTextureId());
	//	//???
	//	//// Draw the Mesh.
	//	//this->mesh->Render("terrain");

	//}

	Terrain::~Terrain()
	{
		for (auto i = 0; i < nodeCountX; i++) {
			delete tiles[i];
		}
		
		delete tiles;
	}


}
