#pragma once
#include <vector>
#include "Ray.h"


namespace pilot {

	class Grid
	{

	public:

		std::vector<Ray> x_rays;
		std::vector<Ray> y_rays;
		std::vector<Ray> z_rays;

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

		std::shared_ptr<GLShader> shader;

		Grid() = default;

		void Init();

		void Update(const glm::mat4 _viewMatrix, const glm::mat4 _projectionMatrix);

		void Render() const;

	};

}
