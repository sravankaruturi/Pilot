#pragma once
#include "Entity.h"

// You need to change this in the Shaders as well if you change it here.
#define MAX_NUMBER_OF_BONES_PER_ENTITY		128

/* Let us implement one animation per Entity for now. We can extend this later on to hold more animations. */

namespace piolot {

	/**
	 * \brief Entity which uses the Animations Stored.
	 *
	 * You cannot convert an Entity to and from AnimatedEntity and expect it to work.
	 * The VertexData would be different.
	 */
	class AnimatedEntity : public piolot::Entity
	{

	protected:

		std::vector<glm::mat4> boneMatrices;

		float animationTotalTime = 0.0f;

	public:

		// Testing purposes.
		void SetAnimationTotalTime(float _animationTotalTime) { animationTotalTime = _animationTotalTime; }

		AnimatedEntity(const std::string& _entityName, const std::string& _objectPath, const std::string& _shaderName, glm::vec3 _bboxLeast = glm::vec3(-1, -1, -1), glm::vec3 _bboxHigh = glm::vec3(1, 1, 1));

		~AnimatedEntity() = default;

		void PlayAnimation(float _deltaTime);

		void Update(float _deltaTime);

		void Render();

	};


}