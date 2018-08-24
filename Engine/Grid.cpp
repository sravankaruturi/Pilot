#include "Grid.h"
#include "AssetManager.h"
#include "Colours.h"

namespace piolot {

	// TODO: This way of drawing grid uses multiple buffers. One fro each ray, which is quite stupid considering we draw this as a single entity most of the time.
	// TODO: Fix this.
	void Grid::Init()
	{

		// Init all the values in the X , Y and Z rays and the Colours.
		x_rays.push_back(Ray(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
		x_rays.push_back(Ray(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));

		y_rays.push_back(Ray(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		y_rays.push_back(Ray(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

		z_rays.push_back(Ray(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		z_rays.push_back(Ray(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));

		// TODO: Find a more elegant way
		// Push back all the remaining rays.
		for (auto i = 1; i < 10; i++)
		{
			x_rays.push_back(Ray(glm::vec3(0, i, 0), glm::vec3(1, 0, 0)));
			x_rays.push_back(Ray(glm::vec3(0, 0, i), glm::vec3(1, 0, 0)));

			x_rays.push_back(Ray(glm::vec3(0, i, 0), glm::vec3(-1, 0, 0)));
			x_rays.push_back(Ray(glm::vec3(0, 0, i), glm::vec3(-1, 0, 0)));

			x_rays.push_back(Ray(glm::vec3(0, -i, 0), glm::vec3(1, 0, 0)));
			x_rays.push_back(Ray(glm::vec3(0, 0, -i), glm::vec3(1, 0, 0)));

			x_rays.push_back(Ray(glm::vec3(0, -i, 0), glm::vec3(-1, 0, 0)));
			x_rays.push_back(Ray(glm::vec3(0, 0, -i), glm::vec3(-1, 0, 0)));


			y_rays.push_back(Ray(glm::vec3(0, 0, i), glm::vec3(0, 1, 0)));
			y_rays.push_back(Ray(glm::vec3(i, 0, 0), glm::vec3(0, 1, 0)));

			y_rays.push_back(Ray(glm::vec3(0, 0, i), glm::vec3(0, -1, 0)));
			y_rays.push_back(Ray(glm::vec3(i, 0, 0), glm::vec3(0, -1, 0)));

			y_rays.push_back(Ray(glm::vec3(0, 0, -i), glm::vec3(0, 1, 0)));
			y_rays.push_back(Ray(glm::vec3(-i, 0, 0), glm::vec3(0, 1, 0)));

			y_rays.push_back(Ray(glm::vec3(0, 0, -i), glm::vec3(0, -1, 0)));
			y_rays.push_back(Ray(glm::vec3(-i, 0, 0), glm::vec3(0, -1, 0)));


			z_rays.push_back(Ray(glm::vec3(i, 0, 0), glm::vec3(0, 0, 1)));
			z_rays.push_back(Ray(glm::vec3(0, i, 0), glm::vec3(0, 0, 1)));

			z_rays.push_back(Ray(glm::vec3(i, 0, 0), glm::vec3(0, 0, -1)));
			z_rays.push_back(Ray(glm::vec3(0, i, 0), glm::vec3(0, 0, -1)));

			z_rays.push_back(Ray(glm::vec3(-i, 0, 0), glm::vec3(0, 0, 1)));
			z_rays.push_back(Ray(glm::vec3(0, -i, 0), glm::vec3(0, 0, 1)));

			z_rays.push_back(Ray(glm::vec3(-i, 0, 0), glm::vec3(0, 0, -1)));
			z_rays.push_back(Ray(glm::vec3(0, -i, 0), glm::vec3(0, 0, -1)));
		}

		shader = ASMGR.shaders.at("axes");


	}

	void Grid::Update(const glm::mat4 _viewMatrix, const glm::mat4 _projectionMatrix)
	{

		viewMatrix = _viewMatrix;
		projectionMatrix = _projectionMatrix;

		shader->use();
		const glm::mat4 model(1.0f);

		shader->setMat4("u_ModelMatrix", model);
		shader->setMat4("u_ViewMatrix", viewMatrix);
		shader->setMat4("u_ProjectionMatrix", projectionMatrix);

	}

	void Grid::Render() const
	{
		// Draw the main axes first.
		for (int i = 0; i < 2; i++)
		{
			x_rays[i].Render(shader, red);
			y_rays[i].Render(shader, green);
			z_rays[i].Render(shader, blue);
		}

		for (int i = 2; i < glm::min(glm::min(x_rays.size(), y_rays.size()), z_rays.size()); i++)
		{
			x_rays[i].Render(shader, grey);
			y_rays[i].Render(shader, grey);
			z_rays[i].Render(shader, grey);
		}

	}
}
