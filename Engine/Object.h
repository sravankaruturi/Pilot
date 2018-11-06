#pragma once
#include <string>
//#include <memory>

#include "Mesh.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <glm/vec4.hpp>
#include <glm/detail/type_vec2.hpp>

#include "PE_GL.h"
#include <map>
#include <glm/mat4x4.hpp>

#define MAX_BONE_WEIGHTS_PER_VERTEX			8

/*This is the list of all the meshes and the materials in it.*/
namespace pilot
{

	/**
	* \brief The Struct to hold the data for each Vertex, and how the surrounding bones impact this vertex.
	*/
	struct VertexBoneData
	{
		unsigned int ids[MAX_BONE_WEIGHTS_PER_VERTEX] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		float weights[MAX_BONE_WEIGHTS_PER_VERTEX] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		VertexBoneData()
		{
			Reset();
		}
		void Reset()
		{
			memset(ids, 0, sizeof(ids));
			memset(weights, 0, sizeof(weights));
		}
		void AddBoneData(unsigned int _boneId, float _weight)
		{
			for (auto i = 0; i < sizeof(ids) / sizeof(unsigned int); i++) {
				if (weights[i] == 0.0) {
					ids[i] = _boneId;
					weights[i] = _weight;
					return;
				}
			}
			// should never get here - more bones than we have space for
			PE_ASSERT(0);
		}
	};

	struct AnimatedVertexData
	{
		/**
		* \brief This contains the details regarding the following variables.
		*
		* It can be a 8 - length integer value, signifying that the number of locations for now, is limited to 8. We can add increase them later on if needed.
		* Each digit signifies the Data Type we are passing to the GPU.
		* 0 --> No Value.
		* 1 --> Float ( 32 bit?? ).
		* 2 --> Int ( 32 bit?? ).
		*
		* 9 --> Undefined
		* All the values are vec4's. The digits signify what the datatype of each of these vector elements is.
		*
		*
		*
		*/
		long header = 11122110;
		/**
		* \brief A Vector 3, Position. And Appended with a Zero
		*/
		glm::vec4 position = glm::vec4(0.f, 0.f, 0.f, 0.f);

		/**
		* \brief Normal, A Vector3., Appended with a Zero
		*/
		glm::vec4 normal = glm::vec4(0.f, 0.f, 0.f, 0.f);

		/**
		* \brief UV Co-ordinates, A Vector2
		*/
		// The Z Value would always be zero. Have to make this a vec3 for loading into the buffer.
		glm::vec4 texCoord = glm::vec4(0.f, 0.f, 0.0f, 0.0f);
		VertexBoneData vbd;

		/**
		* \brief Default Constructor
		*/
		AnimatedVertexData() = default;
	};

	struct VertexData
	{

		long header = 11100000;

		/**
		* \brief A Vector 3, Position. And Appended with a Zero
		*/
		glm::vec4 position = glm::vec4(0.f, 0.f, 0.f, 0.f);

		/**
		* \brief Normal, A Vector3., Appended with a Zero
		*/
		glm::vec4 normal = glm::vec4(0.f, 0.f, 0.f, 0.f);

		/**
		* \brief UV Co-ordinates, A Vector2
		*/
		// The Z Value would always be zero. Have to make this a vec3 for loading into the buffer.
		glm::vec4 texCoord = glm::vec4(0.f, 0.f, 0.0f, 0.0f);

		

		/**
		* \brief Default Constructor
		*/
		VertexData() = default;

	};

	class Object
	{

	protected:
		std::string directory;
		std::string objectName;

		std::vector<std::shared_ptr<Mesh>> meshes;

		/**
		 * \brief The Assimp Importer.
		 *
		 * We need the imported Object because, as soon as this runs out of scope, the @assimpScene variable would be freed.
		 */
		Assimp::Importer assimpImporter;

		/**
		 * \brief This is the Assimp Scene Variable to keep track of the Animations.
		 *
		 * This is the easiest way to keep track of Animations instead of writing an entirely new class and functions based on that.
		 */
		const aiScene * assimpScene;

		/**
		* \brief A map to keep track of the Bones in the Object.
		*
		* String --> Key --> Bone Name.
		* unsigned int --> Value --> Index of this particular bone in the Bone Data.
		*/
		std::map<std::string, unsigned int> boneMapping;


		/**
		* \brief Number of Bones Loaded by our Engine.
		*
		* If this is different from the Actual Number of Bones in Assimp, we have a problem.
		*/
		unsigned int numberOfBonesLoaded = 0;

		/**
		 * \brief The Inverse of the Global Transform Matrix. Used to calculate the Final Transformation Matrices of the Bones.
		 */
		aiMatrix4x4 globalInverseTransform;

		glm::vec3 leastVertex{};
		glm::vec3 highestVertex{};

	public:

		const aiScene* AssimpScene() const
		{
			return assimpScene;
		}

		const glm::vec3 GetLeastVertex() const{
			return leastVertex;
		}

		const glm::vec3 GetHighestVertex() const{
			return highestVertex;
		}

		const std::string& GetObjectName() const
		{
			return objectName;
		}

		Object(const std::string& _name, std::vector<std::shared_ptr<Mesh>> _meshes);
		explicit Object(const std::string& _objectPath);
		~Object();

		void Render(std::string shaderName);

		std::vector<std::shared_ptr<pilot::Mesh>> GetMeshes() const { 
			return meshes; 
		}
		void SetMeshes(std::vector<std::shared_ptr<pilot::Mesh>> val) { meshes = val; }

		void MeshDetailsImGUI();

		/**
		 * \brief This function would load the final Transformation Matrices ( transposed and Ready for OpenGL ) for a particular animation at a given time, from the start of the animation.
		 * \param _totalTime Time elapsed since the start of the Animation
		 * \param _matrices Matrices to load the Bone Matrices in
		 */
		void BoneTransform(float _totalTime, std::vector<glm::mat4>& _matrices);

		/*
		* \brief Copy the Bone Matrices.
		* \param _matrices A reference to the Vector of Matrices that we want to assign the Bone Matrices to.
		*/
		void CopyBoneMatrices(std::vector<glm::mat4>& _matrices);

	private:

		void ProcessNode(aiNode *_node, const aiScene *_scene, std::vector<std::shared_ptr<Mesh>>& _meshes);
		void ProcessAndAddMesh(aiMesh * _mesh, const aiScene * _scene);
		std::vector<std::string> LoadMaterialTextures(aiMaterial* _mat, aiTextureType _type);

		/**
		 * \brief Find the Node Animation Details Pointer using the Model Animation and the Node Name.
		 * \param _animation The Animation of the Mesh we are currently looking for
		 * \param _nodeName The Node Name that we currently want the animation details for.
		 * \return The Node Animation Details.
		 */
		const aiNodeAnim * FindNodeAnim(const aiAnimation * _animation, const std::string& _nodeName);

	};
}

