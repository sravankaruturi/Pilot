#pragma once
#include "Entity.h"
#include "Object.h"

// You need to change this in the Shaders as well if you change it here.
#define MAX_NUMBER_OF_BONES_PER_ENTITY		128

/* Let us implement one animation per Entity for now. We can extend this later on to hold more animations. */

namespace pilot {

	/**
	 * \brief Entity which uses the Animations Stored.
	 *
	 * You cannot convert an Entity to and from AnimatedEntity and expect it to work.
	 * The VertexData would be different.
	 */
	class AnimatedEntity : public pilot::Entity
	{

	protected:

		std::vector<glm::mat4> boneMatrices;

		float animationTotalTime = 0.0f;

	public:

		/**
		* \brief This can be where we load all the animations that are stored in a separate file.
		*/
		std::vector<std::shared_ptr<Object>> animationObjects;

		/**
		* \brief We Get the Bone Matrices from the current Animation Object.
		*/
		std::shared_ptr<Object> currentAnimationObject = nullptr;

		// Testing purposes.
		void SetAnimationTotalTime(float _animationTotalTime) { animationTotalTime = _animationTotalTime; }

		/**
		 * \brief This function is used when you are loading in Animated Entities from the File.
		 */
		AnimatedEntity() = default;

		AnimatedEntity(const std::string& _entityName, const std::string& _objectPath, const std::string& _shaderName, glm::vec3 _bboxLeast = glm::vec3(-1, -1, -1), glm::vec3 _bboxHigh = glm::vec3(1, 1, 1));

		~AnimatedEntity() = default;

		/**
		 * \brief Update the Bone Transforms based on the Time.
		 * \param _deltaTime DeltaTime
		 * \param _currentTime The time since it was initialized.
		 */
		void PlayAnimation(float _deltaTime, float _currentTime);

		/**
		 * \brief Update the Entity
		 * \param _deltaTime DeltaTime
		 * 
		 * Updates the Matrices.
		 */
		void Update(float _deltaTime);

		void Render();

		/**
		 * \brief Save the Entity to the Output Stream
		 * \param _out The Output Stream
		 */
		void SaveToFile(std::ofstream& _out);

		/**
		 * \brief Load the Entity from the Input Stream
		 * \param _in The Input Stream
		 */
		void LoadFromFile(std::ifstream& _in);

	};


}