#pragma once
#include "Entity.h"

#define MAX_BONE_WEIGHTS_PER_VERTEX			8
#define MAX_NUMBER_OF_BONES_PER_ENTITY		100

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

		glm::mat4 boneMatrices[MAX_NUMBER_OF_BONES_PER_ENTITY]{};

		float animationTotalTime = 0.0f;

	public:

		AnimatedEntity(const std::string& _entityName, const std::string& _objectPath, const std::string& _shaderName);

		~AnimatedEntity() = default;

	};


}