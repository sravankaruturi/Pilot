#include <iostream>
#include <vector>

#include "assimp/postprocess.h"
#include "Object.h"
#include "AssetManager.h"


namespace piolot
{
	Object::Object(const std::string& _name, std::vector<std::shared_ptr<Mesh>> _meshes)
		: objectName(_name), meshes(_meshes)
	{

	}

	Object::Object(const std::string& _objectPath)
	{
		directory = _objectPath.substr(0, _objectPath.find_last_of('/'));
		objectName = _objectPath.substr(_objectPath.find_last_of('/') + 1, _objectPath.find_last_of('.') - _objectPath.find_last_of('/') - 1);

		if (ASMGR.IsObjectLoaded(objectName))
		{
			LOGGER.AddToLog("File, " + objectName + " skipped loading");
			return;
		}

		Assimp::Importer importer;

		const aiScene * scene = importer.ReadFile(_objectPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}

		// process ASSIMP's root node recursively
		ProcessNode(scene->mRootNode, scene, GetMeshes());

	}

	Object::~Object()
	= default;

	void Object::Render(const std::string _shaderName)
	{
		// Set any object wide uniforms here. Like Highlight colour os something.
		PE_EXPECT(this->meshes.size() > 0);
		for ( const std::shared_ptr<Mesh>& mesh : GetMeshes())
		{
			mesh->Render(_shaderName);
		}
	}

	void Object::ProcessNode(aiNode* _node, const aiScene* _scene, std::vector<std::shared_ptr<Mesh>>& meshes)
	{
		/* Process each mesh at the current Node */
		for (auto i = 0; i < _node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = _scene->mMeshes[_node->mMeshes[i]];
			ProcessAndAddMesh(mesh, _scene);
		}

		/* We then go for the children */
		for (auto i = 0; i < _node->mNumChildren; i++)
		{
			ProcessNode(_node->mChildren[i], _scene, meshes);
		}
	}

	void Object::ProcessAndAddMesh(aiMesh* _mesh, const aiScene* _scene)
	{
		std::vector<VertexData> vertices;
		std::vector<unsigned int> indices;
		std::vector<std::string> texture_strings;

		for (auto i = 0; i < _mesh->mNumVertices; i++)
		{
			VertexData vertex;

			glm::vec3 vector; /* Placeholder */

			vector.x = _mesh->mVertices[i].x;
			vector.y = _mesh->mVertices[i].y;
			vector.z = _mesh->mVertices[i].z;
			vertex.position = vector;

			///* Normals */
			//vector.x = _mesh->mNormals[i].x;
			//vector.y = _mesh->mNormals[i].y;
			//vector.z = _mesh->mNormals[i].z;
			//vertex.normal = vector;

			/* UV TexCoords */
			if (_mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec3 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = _mesh->mTextureCoords[0][i].x;
				vec.y = _mesh->mTextureCoords[0][i].y;
				vec.z = 0.0f;
				vertex.texCoord = vec;
			}

			vertices.push_back(vertex);
		}

		for (auto i = 0; i < _mesh->mNumFaces; i++)
		{
			aiFace face = _mesh->mFaces[i];

			/* Retrieve Indices */
			for (auto j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		aiMaterial * material = _scene->mMaterials[_mesh->mMaterialIndex];

		/* This is the textures string that we're going to add to the Mesh/Renderable */
		std::vector<std::string> textures;

		/* Now we load all the textures */
		// 1. diffuse maps
		std::vector<std::string> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<std::string> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<std::string> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<std::string> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT);
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		std::shared_ptr<Mesh> return_renderable = std::make_shared<Mesh>(&vertices[0], sizeof(VertexData), vertices.size(), indices);
		return_renderable->SetTextureNames(textures);

		this->meshes.push_back(return_renderable);

	}

	std::vector<std::string> Object::LoadMaterialTextures(aiMaterial* _mat, aiTextureType _type)
	{
		std::vector<std::string> textures;

		for (unsigned int i = 0; i < _mat->GetTextureCount(_type); i++)
		{
			aiString str;
			_mat->GetTexture(_type, i, &str);

			/* Casting it to normal string so that I can perform string operations */
			std::string filename = std::string(str.C_Str());

			/* Check if the texture has been loaded already. */

			/* TO do that, we need to get the filename without any extension */
			/* We cut to the last '.' */
			const std::string key = filename.substr(0, filename.find_first_of('.'));

			if (ASMGR.IsTextureLoaded(key))
			{
				/* Do not load the texture, but do specify that this mesh uses this texture in the list of the textures */
				/* TODO: Maybe this is redundant ? */
				textures.push_back(key);
			}
			else
			{
				/* We've go to load it the old fashioned way. */
				filename = directory + '/' + filename;

				/* Create a new Texture Object and push it on to the Asset Manager. */
				std::shared_ptr<Texture> t = std::make_shared<Texture>(filename);
				if (ASMGR.AddToTextures(key, t))
				{
					textures.push_back(key);
				}
				else
				{
					std::cout << "Loading " << filename << " failed, at" << __FILE__ << ':' << __LINE__ << std::endl;
				}

			}
		}
		return textures;
	}
}
