#pragma once
#include "Animation.h"
#include "AssetManager.h"
#include "assimp/Importer.hpp"
#include <assimp/postprocess.h>

#ifndef MAX_NUMBER_OF_BONES_PER_ENTITY
#define MAX_NUMBER_OF_BONES_PER_ENTITY 128
#endif

namespace pilot
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

	Animation::Animation(std::string _path)
	{

		// Read the file using Assimp.
		this->boneMatrices.reserve(MAX_NUMBER_OF_BONES_PER_ENTITY * sizeof(glm::mat4));

		std::string directory = _path.substr(0, _path.find_last_of('/'));
		std::string animation_name = _path.substr(_path.find_last_of('/') + 1, _path.find_last_of('.') - _path.find_last_of('/') - 1);

		if (ASMGR.IsAnimationLoaded(animation_name))
		{
			LOGGER.AddToLog("File, " + animation_name + " skipped loading.");
			return;
		}

		Assimp::Importer assimp_importer;
		assimpScene = assimp_importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!assimpScene || assimpScene->mFlags & !AI_SCENE_FLAGS_INCOMPLETE || !assimpScene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << assimp_importer.GetErrorString() << std::endl;
			return;
		}

		PE_EXPECT(assimpScene->mNumAnimations > 0);

		const aiAnimation * current_animation = assimpScene->mAnimations[animationIndex];

		const aiAnimation * p_animation = assimpScene->mAnimations[animationIndex];

		animNode = FindNodeAnim(p_animation, assimpScene->mRootNode->mName.data);

		// TODO: Store the Number of Bones, in the numberOfBonesLoaded Variable.


	}

	const aiNodeAnim * Animation::FindNodeAnim(const aiAnimation * _animation, const std::string & _nodeName)
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

	void Animation::BoneTransform(float _totalTime, std::vector<glm::mat4>& _matrices)
	{

		_matrices.resize(numberOfBonesLoaded);

		aiMatrix4x4 root_node_matrix;

		animationIndex = 0;

		if (assimpScene->mNumAnimations > 14) {
			animationIndex = 14;
		}

		if (0 == assimpScene->mNumAnimations)
		{
			for (auto i = 0; i < numberOfBonesLoaded; i++) {
				_matrices[i] = glm::mat4(1.0f);
			}
			return;
		}

		// TODO: Check if valid scene, before accessing Animations here.
		const auto ticks_per_second = assimpScene->mAnimations[animationIndex]->mTicksPerSecond != 0 ? assimpScene->mAnimations[0]->mTicksPerSecond : 25.0f;
		auto time_in_ticks = _totalTime * ticks_per_second;
		auto animation_time = fmod(time_in_ticks, assimpScene->mAnimations[animationIndex]->mDuration);

		ProcessNodeHierarchyAnimation(animation_time, assimpScene->mRootNode, root_node_matrix);

		// For now, set them to Identity.
		for (auto i = 0; i < numberOfBonesLoaded; i++) {
			_matrices[i] = glm::transpose(get_glm_matrix(boneData[i].final_transformation));
		}

	}

	void Animation::ProcessNodeHierarchyAnimation(float _animationTime, const aiNode* _node, const aiMatrix4x4& _parentTransform)
	{

		std::string node_name = _node->mName.data;

		const aiAnimation * p_animation = assimpScene->mAnimations[animationIndex];

		aiMatrix4x4 node_transformation = _node->mTransformation;

		if (animNode)
		{
			//glm::mat4 transformation_matrix(1.0f);

			aiMatrix4x4 translation_matrix;
			aiMatrix4x4 scaling_matrix;

			aiVector3D translation;
			calc_interpolated_position(translation, _animationTime, animNode);

			translation_matrix = aiMatrix4x4::Translation(translation, translation_matrix);

			aiQuaternion rotation;
			CalcInterpolatedRotation(rotation, _animationTime, animNode);

			const aiMatrix4x4 rotation_matrix = aiMatrix4x4(rotation.GetMatrix());

			aiVector3D scaling;
			CalcInterpolatedScaling(scaling, _animationTime, animNode);
			scaling_matrix = aiMatrix4x4::Scaling(scaling, scaling_matrix);

			//node_transformation = scaling_matrix * rotation_matrix * translation_matrix;
			node_transformation = translation_matrix * rotation_matrix * scaling_matrix;
		}

		const aiMatrix4x4 global_transformation = _parentTransform * node_transformation;

	}
}
