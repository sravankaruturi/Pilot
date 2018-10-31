#pragma once
#include <glm/detail/type_vec3.hpp>
#include <vector>
#include <glm/mat4x2.hpp>

namespace pilot
{
	
	/**
	 * \brief This class is used to create a simple cuboid around the Objects as a Bounding Box.
	 */
	class BoundingBox
	{

	protected:

		/**
		 * \brief The Minimum point of the Cuboid in 3D Space. Typically, the Bottom, Left, -Z Point.
		 */
		glm::vec3 minimumPoint;
		/**
		 * \brief The Maximum point of the Cuboid. Typically, the Right, Top, +Z Point.
		 */
		glm::vec3 maximumPoint;

		/**
		 * \brief The Vertices for the Cuboid. Will be computed based on the Min and Max Points.
		 */
		std::vector<glm::vec3> vertices;

		/**
		 * \brief Vertex Array Object ID. Used for Rendering the BBox.
		 */
		unsigned int VAO;
		/**
		 * \brief Vertex Buffer Object ID. Used for Rendering the BBox.
		 */
		unsigned int VBO;

	public:

		/** @defgroup Getters
		*
		* @{*/

		const glm::vec3& GetMinimumPoint() const
		{
			return minimumPoint;
		}

		const glm::vec3& GetMaximumPoint() const
		{
			return maximumPoint;
		}

		unsigned int GetVerticesSize() const
		{
			return vertices.size();
		}

		/* 
		* @} 
		*/

		/**
		 * \brief Constructor
		 * \param _minimumPoint Min Point
		 * \param _maximumPoint Max Point
		 */
		explicit BoundingBox(glm::vec3 _minimumPoint = glm::vec3(-0.2f, -0.2f, -0.2f), glm::vec3 _maximumPoint = glm::vec3(0.2f, 0.2f, 0.2f));

		/**
		 * \brief Check if the Bounding Box Collides with the Ray.
		 * \param _modelMatrix Model Matrix of the Entity
		 * \param _scale Scale of the Entity.
		 * \param _rayOrigin Ray origin
		 * \param _rayDirection Ray Direction
		 * \param _intersectionDistance Return the Intersection Distance
		 * \return If Intersected, return true. Else, False.
		 */
		bool CheckForCollisionWithRay(const glm::mat4 _modelMatrix, const glm::vec3 _scale, const glm::vec3 _rayOrigin,
		                              const glm::vec3 _rayDirection, float& _intersectionDistance) const;

		/**
		 * \brief Render the Bounding Box.
		 * \param _colour The Colour to Render the BBox in
		 */
		void Render(glm::vec3 _colour);

	};

}

