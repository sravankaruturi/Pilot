#pragma once
#include <glm/detail/type_vec3.hpp>
#include <vector>
#include <glm/mat4x2.hpp>

namespace piolot
{
	
	class BoundingBox
	{

	protected:

		glm::vec3 minimumPoint;
		glm::vec3 maximumPoint;

		std::vector<glm::vec3> vertices;

		unsigned int VAO;
		unsigned int VBO;

	public:
		const glm::vec3& GetMinimumPoint() const
		{
			return minimumPoint;
		}

		const glm::vec3& GetMaximumPoint() const
		{
			return maximumPoint;
		}

	public:

		BoundingBox(glm::vec3 _minimumPoint, glm::vec3 _maximumPoint);

		unsigned int GetVerticesSize() const
		{
			return vertices.size();
		}

		bool CheckForCollisionWithRay(const glm::mat4 _modelMatrix, const glm::vec3 _rayOrigin, const glm::vec3 _rayDirection, float& _intersectionDistance) const;

		void Render(glm::vec3 _colour);

	};

}

