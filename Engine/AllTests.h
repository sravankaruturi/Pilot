#pragma once

#if DEBUG

#include "AssetManager.h"
#include "Window.h"
#include <gtest/gtest.h>
#include "Mesh.h"
#include "Entity.h"

using namespace piolot;

class AllTests : public ::testing::Test
{

protected:

	// The window is needed to initiate OpenGL Calls used in the Shaders.
	Window window = Window(800, 600, "Test");

	const std::string awesomefacetexturepath = TEXTURE_FOLDER + std::string("awesomeface.png");

	Texture awesomeFaceTexture = Texture(awesomefacetexturepath);

	std::string test_vert_file_2 = SHADER_FOLDER + std::string("good_test.vert");
	std::string test_frag_file_2 = SHADER_FOLDER + std::string("good_test.frag");

	GLShader right_shader = GLShader(test_vert_file_2.c_str(), test_frag_file_2.c_str());

public:

	struct VertexDataTestGood
	{
		glm::vec3 position;
		glm::vec3 colour;
		glm::vec3 texCoord;	// Note all the things have to be a vec3.

		VertexDataTestGood(glm::vec3 _position, glm::vec3 _colour, glm::vec3 _texCoord)
			: position(_position), colour(_colour), texCoord(_texCoord)
		{

		}

	};

	std::vector<VertexDataTestGood> vertices = {
		VertexDataTestGood(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
		VertexDataTestGood(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
		VertexDataTestGood(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.5f, 1.0f, 0.0f))
	};

};

#endif