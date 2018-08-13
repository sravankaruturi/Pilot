#include "BoundingBox.h"
#include "PE_GL.h"
#include "AssetManager.h"

namespace piolot
{
	BoundingBox::BoundingBox(glm::vec3 _minimumPoint, glm::vec3 _maximumPoint)
	: minimumPoint(_minimumPoint),
		maximumPoint(_maximumPoint)
	{

		// We populate all the vertices.
		auto length = maximumPoint.x - minimumPoint.x;
		auto height = maximumPoint.y - minimumPoint.y;
		auto breadth = maximumPoint.z - minimumPoint.z;

		auto centre = (minimumPoint + maximumPoint) / 2.0f;

		// We need to populate it in the order which we would like to draw.
		// For my sanity, I am drawing these using line segments.

		// Vertices. @self, For Reference, check the red notes.
		vertices.push_back(glm::vec3(minimumPoint.x, minimumPoint.y, maximumPoint.z));
		vertices.push_back(glm::vec3(maximumPoint.x, minimumPoint.y, maximumPoint.z));

		vertices.push_back(glm::vec3(maximumPoint.x, minimumPoint.y, maximumPoint.z));
		vertices.push_back(glm::vec3(maximumPoint.x, minimumPoint.y, minimumPoint.z));

		vertices.push_back(glm::vec3(maximumPoint.x, minimumPoint.y, minimumPoint.z));
		vertices.push_back(glm::vec3(minimumPoint));

		vertices.push_back(glm::vec3(minimumPoint));
		vertices.push_back(glm::vec3(minimumPoint.x, minimumPoint.y, maximumPoint.z));

		vertices.push_back(glm::vec3(minimumPoint.x, maximumPoint.y, maximumPoint.z));
		vertices.push_back(glm::vec3(maximumPoint));

		vertices.push_back(glm::vec3(maximumPoint));
		vertices.push_back(glm::vec3(maximumPoint.x, maximumPoint.y, minimumPoint.z));

		vertices.push_back(glm::vec3(maximumPoint.x, maximumPoint.y, minimumPoint.z));
		vertices.push_back(glm::vec3(minimumPoint.x, maximumPoint.y, minimumPoint.z));

		vertices.push_back(glm::vec3(minimumPoint.x, maximumPoint.y, minimumPoint.z));
		vertices.push_back(glm::vec3(minimumPoint.x, maximumPoint.y, maximumPoint.z));

		vertices.push_back(glm::vec3(minimumPoint.x, minimumPoint.y, maximumPoint.z));
		vertices.push_back(glm::vec3(minimumPoint.x, maximumPoint.y, maximumPoint.z));

		vertices.push_back(glm::vec3(maximumPoint.x, minimumPoint.y, maximumPoint.z));
		vertices.push_back(glm::vec3(maximumPoint));

		vertices.push_back(glm::vec3(maximumPoint.x, minimumPoint.y, minimumPoint.z));
		vertices.push_back(glm::vec3(maximumPoint.x, maximumPoint.y, minimumPoint.z));

		vertices.push_back(glm::vec3(minimumPoint));
		vertices.push_back(glm::vec3(minimumPoint.x, maximumPoint.y, minimumPoint.z));

		// Create the VAO, VBO and IBO for the BBox.
		PE_GL(glGenVertexArrays(1, &VAO));
		PE_GL(glGenBuffers(1, &VBO));

		PE_GL(glBindVertexArray(VAO));

		PE_GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		PE_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW));

		PE_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0));
		PE_GL(glEnableVertexAttribArray(0));
	}

	bool BoundingBox::CheckForCollisionWithRay(const glm::mat4 _modelMatrix, const glm::vec3 _scale, const glm::vec3 _rayOrigin, const glm::vec3 _rayDirection, float& _intersectionDistance) const
	{

		// @see http://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-custom-ray-obb-function/
		// @see https://github.com/opengl-tutorials/ogl/tree/master/misc05_picking
		// @see https://github.com/opengl-tutorials/ogl/blob/master/misc05_picking/misc05_picking_custom.cpp
		// TODO: Read this again, as you were quite confused the first time around.
		// The article linked above does not take into consideration, the scaling included in the model matrix.

		float tMin = 0.0f;
		float tMax = 100000.0f;

		const auto scaled_min = glm::vec3(_scale.x * minimumPoint.x, _scale.y * minimumPoint.y, _scale.z * minimumPoint.z);
		const auto scaled_max = glm::vec3(_scale.x * maximumPoint.x, _scale.y * maximumPoint.y, _scale.z * maximumPoint.z);

		glm::vec3 obbposition_worldspace(_modelMatrix[3].x, _modelMatrix[3].y, _modelMatrix[3].z);
		glm::vec3 delta = obbposition_worldspace - _rayOrigin;

		// Testing the intersection with the 2 planes perpendicular to the OBB's X Axes
		{
			const glm::vec3 obb_xaxis = glm::normalize(glm::vec3(_modelMatrix[0].x, _modelMatrix[0].y, _modelMatrix[0].z));
			float e = glm::dot(obb_xaxis, delta);
			float f = glm::dot(_rayDirection, obb_xaxis);

			// The ray is not paraller to the XAxis of the OBB.
			if ( glm::abs(f) > 0.001f )
			{
				float t1 = (e + scaled_min.x) / f;
				float t2 = (e + scaled_max.x) / f;

				if ( t1 > t2)
				{
					// Swap them
					float w = t1;
					t1 = t2;
					t2 = w;
				}

				if ( t2 < tMax)
				{
					tMax = t2;
				}

				if ( t1 > tMin)
				{
					tMin = t1;
				}

				if ( tMax < tMin)
				{
					return false;
				}

			}else
			{
				if ( -e + scaled_min.x > 0.0f || -e + scaled_max.x < 0.0f)
				{
					return false;
				}
			}

		}

		{
			const glm::vec3 obb_yaxis = glm::normalize(glm::vec3 (_modelMatrix[1].x, _modelMatrix[1].y, _modelMatrix[1].z));

			float e = glm::dot(obb_yaxis, delta);
			float f = glm::dot(_rayDirection, obb_yaxis);

			if ( glm::abs(f) > 0.001f)
			{
				float t1 = (e + scaled_min.y) / f;
				float t2 = (e + scaled_max.y) / f;

				if ( t1 > t2 )
				{
					float w = t1;
					t1 = t2;
					t2 = w;
				}

				if ( t2 < tMax)
				{
					tMax = t2;
				}

				if ( t1 > tMin)
				{
					tMin = t1;
				}

				if (tMin > tMax)
				{
					return false;
				}
			}else
			{
				if ( -e + scaled_min.y > 0.0f || -e + scaled_max.y < 0.0f)
				{
					return false;
				}
			}

		}

		// Test the intersection with the 2 planes perpendicular to the OBB's z axis.
		{
			const glm::vec3 obb_zaxis = glm::normalize(glm::vec3(_modelMatrix[2].x, _modelMatrix[2].y, _modelMatrix[2].z));

			float e = glm::dot(obb_zaxis, delta);
			float f = glm::dot(_rayDirection, obb_zaxis);

			if ( glm::abs(f) > 0.001f)
			{
				float t1 = (e + scaled_min.z) / f;
				float t2 = (e + scaled_max.z) / f;

				if ( t1 > t2 )
				{
					float w = t1;
					t1 = t2;
					t2 = w;
				}

				if ( t2 < tMax)
				{
					tMax = t2;
				}

				if (t1 > tMin)
				{
					tMin = t1;
				}

				if ( tMin > tMax)
				{
					return false;
				}
			}else
			{
				if ( -e + scaled_min.z > 0.0f || -e + scaled_max.z < 0.0f)
				{
					return false;
				}
			}
		}

		_intersectionDistance = tMin;
		return true;
	}

	void BoundingBox::Render(glm::vec3 _colour)
	{
		ASMGR.shaders.at("bbox")->use();
		ASMGR.shaders.at("bbox")->setVec3("u_Colour", _colour);

		PE_GL(glBindVertexArray(this->VAO));
		PE_GL(glBindBuffer(GL_ARRAY_BUFFER, this->VBO));
		PE_GL(glDrawArrays(GL_LINES, 0, vertices.size()));
		PE_GL(glBindVertexArray(0));
	}
}
