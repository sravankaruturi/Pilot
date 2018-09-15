#pragma once
#include <string>
//#include <memory>

#include "Mesh.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/detail/type_vec2.hpp>

/*This is the list of all the meshes and the materials in it.*/
namespace piolot
{

	struct VertexData
	{
		/**
		* \brief A Vector 3, Position.
		*/
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);

		/**
		* \brief Normal, A Vector3.
		*/
		glm::vec3 normal = glm::vec3(0.f, 0.f, 0.f);

		/**
		* \brief UV Co-ordinates, A Vector2
		*/
		// The Z Value would always be zero. Have to make this a vec3 for loading into the buffer.
		glm::vec3 texCoord = glm::vec3(0.f, 0.f, 0.0f);

		/**
		* \brief Default Constructor
		*/
		VertexData() = default;

		/**
		* \brief A Constructor with all the values.
		* \param _x Position.x
		* \param _y Position.y
		* \param _z Position.z
		* \param _r Normal.x
		* \param _g Normal.y
		* \param _b Normal.z
		* \param _tx TexCoords.u
		* \param _ty TexCoords.v
		*/
		//VertexData(float _x, float _y, float _z, float _r, float _g, float _b, float _tx, float _ty)
		//{
		//	position = glm::vec3(_x, _y, _z);
		//	normal = glm::vec3(_r, _g, _b);
		//	texCoord = glm::vec2(_tx, _ty);
		//}
	};

	class Object
	{

	protected:
		std::string directory;
		std::string objectName;

		std::vector<std::shared_ptr<Mesh>> meshes;

		// TODO: Save Animations here.

	public:
		const std::string& GetObjectName() const
		{
			return objectName;
		}

		Object(const std::string& _name, std::vector<std::shared_ptr<Mesh>> _meshes);
		explicit Object(const std::string& _objectPath);
		~Object();

		void Render(std::string shaderName);

		std::vector<std::shared_ptr<piolot::Mesh>> GetMeshes() const { return meshes; }
		void SetMeshes(std::vector<std::shared_ptr<piolot::Mesh>> val) { meshes = val; }

		// TODO: Call PlayAnimation(_deltaTime) here.

	private:
		void ProcessNode(aiNode *_node, const aiScene *_scene, std::vector<std::shared_ptr<Mesh>>& _meshes);
		void ProcessAndAddMesh(aiMesh * _mesh, const aiScene * _scene);
		std::vector<std::string> LoadMaterialTextures(aiMaterial* _mat, aiTextureType _type);
	};
}

