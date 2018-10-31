#include "Ray.h"

#include "PE_GL.h"
#include "AssetManager.h"

namespace pilot {

	// TODO: We are updating Buffers every frame. This is seriously expensive as we draw the grid,
	void Ray::UpdateBuffers()
	{

		vertices.clear();

		// Empty the vertices.
		vertices.clear();

		// Add all the vertices.
		vertices.push_back(origin);

		for (int i = 1; i < 20; i++)
		{
			vertices.push_back(origin + glm::vec3(direction.x * i, direction.y * i, direction.z * i));
		}

		//ASMNGR.shaders.at("axes")->use();

		// Render the Ray.
		//unsigned int vbo, vao;
		PE_GL(glGenVertexArrays(1, &vao));
		PE_GL(glGenBuffers(1, &vbo));

		PE_GL(glBindVertexArray(vao));

		PE_GL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		PE_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW));

		PE_GL(glBindVertexArray(vao));

		PE_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0));
		PE_GL(glEnableVertexAttribArray(0));
	}

	Ray::Ray(glm::vec3 _origin, glm::vec3 _direction)
		: origin(_origin)
	{
		direction = glm::normalize(_direction);
		UpdateBuffers();
	}

	void Ray::Update(const glm::mat4 _viewMatrix, const glm::mat4 _projectionMatrix, const glm::vec3 _colour)
	{
		//UpdateBuffers();
		const auto current_shader = ASMGR.shaders.at("axes");

		current_shader->use();
		current_shader->setMat4("u_ModelMatrix", glm::mat4(1.0f));
		current_shader->setMat4("u_ViewMatrix", _viewMatrix);
		current_shader->setMat4("u_ProjectionMatrix", _projectionMatrix);
		current_shader->setVec3("u_Colour", _colour);

	}

	void Ray::Render(std::shared_ptr<GLShader> _shader, const glm::vec3 _colour) const
	{
		_shader->use();
		_shader->setVec3("u_Colour", _colour);
		PE_GL(glBindVertexArray(vao));
		PE_GL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		PE_GL(glDrawArrays(GL_LINE_STRIP, 0, vertices.size()));
	}
}
