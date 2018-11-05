#pragma once
#include <glm/mat4x4.hpp>
#include <vector>

namespace pilot
{

	/**
	 * \brief The Animation class contains the Bone Matrix Data that is specific to each Animation
	 */
	class Animation
	{

		std::vector<glm::mat4> boneMatrices;

	public:
		

		explicit Animation(std::string _path);

	};
	
}
