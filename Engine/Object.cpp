#include <iostream>
#include <vector>

#include "assimp/postprocess.h"
#include "Object.h"
#include "AssetManager.h"


namespace piolot
{

	void convert_aimatrix_to_glm(glm::mat4& _glmMat4, const aiMatrix4x4& _aiMatrix) {

		for (auto i = 0; i < 4; i++) {
			for (auto j = 0; j < 4; j++) {
				_glmMat4[i][j] = _aiMatrix[i][j];
			}
		}

	}

	// This just copies. This function doesn't transpose matrices.
	void convert_aimatrix_to_glm(glm::mat4& _glmMat4, const aiMatrix3x3& _aiMatrix) {

		for (auto i = 0; i < 3; i++) {
			for (auto j = 0; j < 3; j++) {
				_glmMat4[i][j] = _aiMatrix[i][j];
			}
		}

		// The rest would be zero, other than the 4,4.
		_glmMat4[0][3] = 0.0f;
		_glmMat4[1][3] = 0.0f;
		_glmMat4[2][3] = 0.0f;

		_glmMat4[3][0] = 0.0f;
		_glmMat4[3][1] = 0.0f;
		_glmMat4[3][2] = 0.0f;

		_glmMat4[3][3] = 1.0f;

	}

	glm::mat4 get_glm_matrix(const aiMatrix4x4& _aiMatrix) {
		glm::mat4 return_matrix;
		convert_aimatrix_to_glm(return_matrix, _aiMatrix);
		return return_matrix;
	}

	unsigned int find_scaling(float _animationTime, const aiNodeAnim* _nodeAnim)
	{
		PE_ASSERT(_nodeAnim->mNumScalingKeys > 0);
		for (unsigned int i = 0; i < _nodeAnim->mNumScalingKeys - 1; i++) {
			if (_animationTime < (float)_nodeAnim->mScalingKeys[i + 1].mTime) {
				return i;
			}
		}

		// It should never reach here.
		PE_ASSERT(0);
		return 0;
	}

	unsigned int find_rotation(float _animationTime, const aiNodeAnim* _nodeAnim)
	{
		PE_ASSERT(_nodeAnim->mNumRotationKeys > 0);
		for (unsigned int i = 0; i < _nodeAnim->mNumRotationKeys - 1; i++) {
			if (_animationTime < (float)_nodeAnim->mRotationKeys[i + 1].mTime) {
				return i;
			}
		}

		// It should never reach here.
		PE_ASSERT(0);
		return 0;
	}

	unsigned int find_position(float& _animationTime, const aiNodeAnim* _nodeAnim)
	{
		PE_ASSERT(_nodeAnim->mNumPositionKeys > 0);
		for (unsigned int i = 0; i < _nodeAnim->mNumPositionKeys - 1; i++) {
			if (_animationTime < (float)_nodeAnim->mPositionKeys[i + 1].mTime) {
				return i;
			}
		}

		// The Very first time, that we start playing the animation, maybe set it to zero.
		_animationTime = 0;
		return 0;

		// It should never reach here.
		PE_ASSERT(0);
		return 0;
	}

	void calc_interpolated_position(aiVector3D& _out, float& _animationTime, const aiNodeAnim* _nodeAnim)
	{
		if (_nodeAnim->mNumPositionKeys == 1) {
			// There is only one Position.
			_out = _nodeAnim->mPositionKeys[0].mValue;
			return;
		}

		unsigned int position_index = find_position(_animationTime, _nodeAnim);
		unsigned int next_pos_index = position_index + 1;
		PE_ASSERT(next_pos_index < _nodeAnim->mNumPositionKeys);

		// The Difference between two key frames.
		float delta_time = (float)(_nodeAnim->mPositionKeys[next_pos_index].mTime - _nodeAnim->mPositionKeys[position_index].mTime);

		// The Factor by which the current frame has transitioned into the next frame.
		float factor = (_animationTime - (float)_nodeAnim->mPositionKeys[position_index].mTime) / delta_time;

		//if ( factor < 0.0f )
		//{
		//	factor = 0.0f;
		//}

		PE_ASSERT(factor >= 0.0f && factor <= 1.0f);

		const auto start = _nodeAnim->mPositionKeys[position_index].mValue;
		const auto end = _nodeAnim->mPositionKeys[next_pos_index].mValue;

		_out = start + factor * (end - start);

	}

	void CalcInterpolatedRotation(aiQuaternion& _out, float _animationTime, const aiNodeAnim* _nodeAnim)
	{

		if (_nodeAnim->mNumRotationKeys == 1) {
			// There is only one Position.
			_out = _nodeAnim->mRotationKeys[0].mValue;
			return;
		}

		unsigned int rotation_index = find_rotation(_animationTime, _nodeAnim);
		unsigned int next_rot_index = rotation_index + 1;
		PE_ASSERT(next_rot_index < _nodeAnim->mNumRotationKeys);

		// The Difference between two key frames.
		float delta_time = (float)(_nodeAnim->mRotationKeys[next_rot_index].mTime - _nodeAnim->mRotationKeys[rotation_index].mTime);

		// The Factor by which the current frame has transitioned into the next frame.
		float factor = (_animationTime - (float)_nodeAnim->mRotationKeys[rotation_index].mTime) / delta_time;

		//if ( factor < 0.0f)
		//{
		//	factor = 0.0f;
		//}

		PE_ASSERT(factor >= 0.0f && factor <= 1.0f);

		const auto start = _nodeAnim->mRotationKeys[rotation_index].mValue;
		const auto end = _nodeAnim->mRotationKeys[next_rot_index].mValue;

		aiQuaternion::Interpolate(_out, start, end, factor);

		_out = _out.Normalize();

	}

	void CalcInterpolatedScaling(aiVector3D& _out, float _animationTime, const aiNodeAnim* _nodeAnim)
	{

		if (_nodeAnim->mNumScalingKeys == 1) {
			_out = _nodeAnim->mScalingKeys[0].mValue;
			return;
		}

		auto scaling_index = find_scaling(_animationTime, _nodeAnim);
		auto nex_sca_index = scaling_index + 1;

		PE_ASSERT(nex_sca_index < _nodeAnim->mNumScalingKeys);

		auto delta_time = (float)(_nodeAnim->mScalingKeys[nex_sca_index].mTime - _nodeAnim->mScalingKeys[scaling_index].mTime);

		auto factor = (_animationTime - (float)_nodeAnim->mScalingKeys[scaling_index].mTime) / delta_time;

		//if( factor < 0.0f)
		//{
		//	factor = 0.0f;
		//}
		PE_ASSERT(factor >= 0.0f && factor <= 1.0f);

		auto start = _nodeAnim->mScalingKeys[scaling_index].mValue;
		auto end = _nodeAnim->mScalingKeys[nex_sca_index].mValue;

		_out = start + factor * (end - start);

	}




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

		assimpScene = assimpImporter.ReadFile(_objectPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		if (!assimpScene|| assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpScene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << assimpImporter.GetErrorString() << std::endl;
			return;
		}

		globalInverseTransform = assimpScene->mRootNode->mTransformation;

		// process ASSIMP's root node recursively
		ProcessNode(assimpScene->mRootNode, assimpScene, GetMeshes());

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

	void Object::MeshDetailsImGUI()
	{
		std::string name;

		ImGui::Text("Bounding Box Details: ");
		ImGui::Text("Min: %f %f %f", &leastVertex[0], &leastVertex[1], &leastVertex[2]);
		ImGui::Text("Max: %f %f %f", &highestVertex[0], &highestVertex[1], &highestVertex[2]);

		short i = 0;
		for (auto& it : this->meshes) {
			i++;
			ImGui::PushID(&it);
			name = "Mesh " + std::to_string(i);
			if (ImGui::TreeNode(name.c_str())) {
				// Display the texture names.
				ImGui::TextColored(ImVec4(1.0, 0.2, 0.2, 0.8), "Textures");
				ImGui::Separator();

				// Display all the textures.
				//auto texture_names = it->GetTextureNames();
				/*for (auto itit : texture_names) {
					ImGui::Text(itit.c_str());
				}*/

				ImGui::TreePop();
			}
			ImGui::PopID();

		}

	}

	void Object::BoneTransform(float _totalTime, std::vector<glm::mat4>& _matrices)
	{

		_matrices.resize(numberOfBonesLoaded);

		aiMatrix4x4 root_node_matrix;

		int selected_animation_index = 0;

		if (assimpScene->mNumAnimations > 14) {
			selected_animation_index = 14;
		}

		// TODO: Check if valid scene, before accessing Animations here.
		const auto ticks_per_second = assimpScene->mAnimations[selected_animation_index]->mTicksPerSecond != 0 ? assimpScene->mAnimations[0]->mTicksPerSecond : 25.0f;
		auto time_in_ticks = _totalTime * ticks_per_second;
		auto animation_time = fmod(time_in_ticks, assimpScene->mAnimations[selected_animation_index]->mDuration);

		ProcessNodeHierarchyAnimation(animation_time, assimpScene->mRootNode, selected_animation_index, root_node_matrix);

		// For now, set them to Identity.
		for (auto i = 0; i < numberOfBonesLoaded; i++) {
			_matrices[i] = glm::transpose(get_glm_matrix(boneData[i].final_transformation));
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

		if (_mesh->HasBones()) {

			std::vector<AnimatedVertexData> vertices;
			std::vector<unsigned int> indices;
			std::vector<std::string> texture_strings;

			std::vector<VertexBoneData> vertex_bone_data;
			vertex_bone_data.resize(_mesh->mNumVertices);
			for (auto& it : vertex_bone_data)
			{
				it.Reset();
			}


			for (auto i = 0; i < _mesh->mNumVertices; i++)
			{
				AnimatedVertexData vertex;

				glm::vec4 vector; /* Placeholder */

				vector.x = _mesh->mVertices[i].x;
				vector.y = _mesh->mVertices[i].y;
				vector.z = _mesh->mVertices[i].z;
				vector.w = 0.0f;
				vertex.position = vector;

				leastVertex.x = glm::min(vector.x, leastVertex.x);
				leastVertex.y = glm::min(vector.y, leastVertex.y);
				leastVertex.z = glm::min(vector.z, leastVertex.z);

				highestVertex.x = glm::max(vector.x, highestVertex.x);
				highestVertex.y = glm::max(vector.y, highestVertex.y);
				highestVertex.z = glm::max(vector.z, highestVertex.z);

				if (_mesh->HasNormals()) {

					/* Normals */
					vector.x = _mesh->mNormals[i].x;
					vector.y = _mesh->mNormals[i].y;
					vector.z = _mesh->mNormals[i].z;
					vector.w = 0.0f;
					vertex.normal = vector;

				}

				/* UV TexCoords */
				if (_mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
				{
					glm::vec4 vec;
					// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
					// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
					vec.x = _mesh->mTextureCoords[0][i].x;
					vec.y = _mesh->mTextureCoords[0][i].y;
					vec.z = 0.0f;
					vec.w = 0.0f;
					vertex.texCoord = vec;
				}

				vertices.push_back(vertex);
			}

			// This loop adds all the data for all the bones in this mesh. There might be overlap because, an entity might have two meshes with the same bone.
			for (auto j = 0; j < _mesh->mNumBones; j++)
			{
				std::string bone_name = std::string(_mesh->mBones[j]->mName.data);
				int bone_index = -1;
				if (boneMapping.find(bone_name) == boneMapping.end())
				{
					bone_index = numberOfBonesLoaded;
					BoneInfo bi;
					bi.bone_offset = _mesh->mBones[j]->mOffsetMatrix;
					boneData.push_back(bi);
					boneMapping[bone_name] = bone_index;
					numberOfBonesLoaded++;
				}
				else
				{
					bone_index = boneMapping[bone_name];
				}
				for (auto k = 0; k < _mesh->mBones[j]->mNumWeights; k++)
				{

					unsigned int local_vertex_id = _mesh->mBones[j]->mWeights[k].mVertexId;
					float weight = _mesh->mBones[j]->mWeights[k].mWeight;
					vertex_bone_data[local_vertex_id].AddBoneData(bone_index, weight);
					if (bone_index > 128)
					{
						// This shouldn't happen usually.
						PE_ASSERT(0);
					}
				}
			}

			// Now assign the Vertex Weights to the Vertex Data.
			for (auto i = 0; i < _mesh->mNumVertices; i++)
			{
				vertices[i].vbd = vertex_bone_data[i];
			}


			//for (int i = 0; i < _mesh->mNumVertices; i++) {
			//	vertices[i].vbd = VertexBoneData();
			//}

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

			std::shared_ptr<Mesh> return_renderable = std::make_shared<Mesh>(&vertices[0], sizeof(AnimatedVertexData), vertices.size(), indices);
			return_renderable->SetTextureNames(textures);

			this->meshes.push_back(return_renderable);

		}
		else {

			std::vector<VertexData> vertices;
			std::vector<unsigned int> indices;
			std::vector<std::string> texture_strings;

			for (auto i = 0; i < _mesh->mNumVertices; i++)
			{
				VertexData vertex;

				glm::vec4 vector; /* Placeholder */

				vector.x = _mesh->mVertices[i].x;
				vector.y = _mesh->mVertices[i].y;
				vector.z = _mesh->mVertices[i].z;
				vector.w = 0.0f;
				vertex.position = vector;

				leastVertex.x = (vector.x < leastVertex.x) ? vector.x : leastVertex.x ;
				leastVertex.y = glm::min(vector.y, leastVertex.y);
				leastVertex.z = glm::min(vector.z, leastVertex.z);

				highestVertex.x = glm::max(vector.x, highestVertex.x);
				highestVertex.y = glm::max(vector.y, highestVertex.y);
				highestVertex.z = glm::max(vector.z, highestVertex.z);

				if (_mesh->HasNormals()) {

					/* Normals */
					vector.x = _mesh->mNormals[i].x;
					vector.y = _mesh->mNormals[i].y;
					vector.z = _mesh->mNormals[i].z;
					vector.w = 0.0f;
					vertex.normal = vector;

				}

				/* UV TexCoords */
				if (_mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
				{
					glm::vec4 vec;
					// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
					// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
					vec.x = _mesh->mTextureCoords[0][i].x;
					vec.y = _mesh->mTextureCoords[0][i].y;
					vec.z = 0.0f;
					vec.w = 0.0f;
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
				std::shared_ptr<Texture> t = std::make_shared<Texture>(filename, false);
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

	void Object::ProcessNodeHierarchyAnimation(float _animationTime, const aiNode* _node, const int _animationIndex,
		const aiMatrix4x4& _parentTransform)
	{

		auto selectedAnimationIndex = _animationIndex;

		std::string node_name = _node->mName.data;

		const aiAnimation * p_animation = assimpScene->mAnimations[selectedAnimationIndex];

		aiMatrix4x4 node_transformation = _node->mTransformation;

		const aiNodeAnim * node_anim = FindNodeAnim(p_animation, node_name);

		if (node_anim) {

			//glm::mat4 transformation_matrix(1.0f);

			aiMatrix4x4 translation_matrix;
			aiMatrix4x4 scaling_matrix;

			aiVector3D translation;
			calc_interpolated_position(translation, _animationTime, node_anim);

			translation_matrix = aiMatrix4x4::Translation(translation, translation_matrix);

			aiQuaternion rotation;
			CalcInterpolatedRotation(rotation, _animationTime, node_anim);

			const aiMatrix4x4 rotation_matrix = aiMatrix4x4(rotation.GetMatrix());

			aiVector3D scaling;
			CalcInterpolatedScaling(scaling, _animationTime, node_anim);
			scaling_matrix = aiMatrix4x4::Scaling(scaling, scaling_matrix);

			//node_transformation = scaling_matrix * rotation_matrix * translation_matrix;
			node_transformation = translation_matrix * rotation_matrix * scaling_matrix;

		}

		const aiMatrix4x4 global_transformation = _parentTransform * node_transformation;

		if (boneMapping.find(node_name) != boneMapping.end()) {

			// Update the Global Transformation.
			auto bone_index = boneMapping[node_name];

			boneData[bone_index].final_transformation = globalInverseTransform * global_transformation * boneData[bone_index].bone_offset;
			//boneInfoData[bone_index].finalTransformation = boneInfoData[bone_index].boneOffset * global_transformation * globalInverseTransform;
		}

		for (auto i = 0; i < _node->mNumChildren; i++) {
			ProcessNodeHierarchyAnimation(_animationTime, _node->mChildren[i], selectedAnimationIndex , global_transformation);
		}

	}

	const aiNodeAnim* Object::FindNodeAnim(const aiAnimation* _animation, const std::string& _nodeName)
	{

		for (auto i = 0; i < _animation->mNumChannels; i++)
		{
			const aiNodeAnim * node_anim = _animation->mChannels[i];

			if (std::string(node_anim->mNodeName.data) == _nodeName) {
				return node_anim;
			}
		}

		return nullptr;

	}

	void Object::CopyBoneMatrices(std::vector<glm::mat4>& _matrices)
	{
		for (int i = 0; i < _matrices.size(); i++) {
			_matrices[i] = glm::transpose(get_glm_matrix(boneData[i].final_transformation));
		}
	}
}
