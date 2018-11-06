#pragma once
#include <glm/mat4x4.hpp>
#include <vector>
#include "Scene.h"

namespace pilot
{

	/**
	* \brief The Structure to hold data for each bone in an Object.
	*/
	struct BoneInfo
	{
		/**
		* \brief The Offset from the Object Root.
		*/
		aiMatrix4x4 bone_offset;
		/**
		* \brief The Final Transformation after the Animation is applied.
		*/
		aiMatrix4x4 final_transformation;
	};

	/**
	 * \brief The Animation class contains the Bone Matrix Data that is specific to each Animation
	 */
	class Animation
	{

		std::vector<glm::mat4> boneMatrices;

		const aiScene * assimpScene;

		/**
		* \brief This keeps track of when this particular object was animated last.
		*
		* By Keeping track of this, if we have multiple animated entities sharing the animation, they can use the same Object. If we have two different entities playing different animation, we can create a new object for them.
		*/
		float lastUpdateTime = 0.0f;

		unsigned int numberOfBonesLoaded;

		/**
		* \brief This contains details regarding the Bone Transformations for all the Bones.
		*
		* If you need a particular bone, get the index of that bone from the BoneMapping.
		*
		* @see @Object.boneMapping
		*/
		std::vector<BoneInfo> boneData;

		unsigned int animationIndex = 0;

		const aiNodeAnim * animNode;

		const aiNodeAnim * FindNodeAnim(const aiAnimation* _animation, const std::string& _nodeName);

	public:

		float GetLastUpdateTime() const
		{
			return lastUpdateTime;
		}

		void SetLastUpdateTime(const float _lastUpdateTime)
		{
			lastUpdateTime = _lastUpdateTime;
		}

		const aiScene * AssimpScene() const { return assimpScene; }

		explicit Animation(std::string _path);

		void BoneTransform(float _totalTime, std::vector<glm::mat4>& _matrices);

		void ProcessNodeHierarchyAnimation(float _animationTime, const aiNode * _node, const aiMatrix4x4& _parentTransform);

	};
	
}
