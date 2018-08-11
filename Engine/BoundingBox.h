#pragma once
#include <glm/detail/type_vec3.hpp>
#include <vector>

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

		bool CheckForCollisionWithRay(glm::vec3 RayOrigin, glm::vec3 RayDirection) const;

		void Render(glm::vec3 _colour);

	};

}

