﻿#pragma once

#include <vector>

#include <glm/detail/type_vec3.hpp>
#include <glm/mat4x4.hpp>
#include <memory>

namespace piolot {

	class GLShader;

	class Ray
	{

		glm::vec3 origin;
		glm::vec3 direction;

		unsigned int vbo;
		unsigned int vao;

		std::vector<glm::vec3> vertices;

		void UpdateBuffers();

	public:

		explicit Ray(glm::vec3 _origin, glm::vec3 _direction);

		void Update(const glm::mat4 _viewMatrix, const glm::mat4 _projectionMatrix, const glm::vec3 _colour);

		void Render(std::shared_ptr<GLShader> _shader, glm::vec3 _colour);

	};


}
