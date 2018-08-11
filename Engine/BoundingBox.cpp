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
