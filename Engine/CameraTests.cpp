#include <gtest/gtest.h>
#include "Camera.h"
#include <glm/gtc/matrix_transform.inl>

class CameraTests : public ::testing::Test
{

protected:
	// At Origin
	glm::vec3 position = glm::vec3(0, 0, 0);
	// Looking at Negative Z Axis.
	glm::vec3 front = glm::vec3(0, 0, -1);
	// World Up is along the Y Axis.
	glm::vec3 world_up = glm::vec3(0, 1, 0);

	piolot::Camera c = piolot::Camera("Test", position, front, world_up);

};

TEST_F(CameraTests, Initialization)
{
	{
		// Check if the Camera Position is set properly
		glm::vec3 camera_position = c.GetPosition();
		EXPECT_FLOAT_EQ(camera_position.x, position.x);
		EXPECT_FLOAT_EQ(camera_position.y, position.y);
		EXPECT_FLOAT_EQ(camera_position.z, position.z);
	}
	
	{
		// Check if the Right Vector is calculated properly. It should be 1, 0, 0.
		// Check if the Camera Position is set properly
		glm::vec3 camera_right = c.GetRight();
		EXPECT_FLOAT_EQ(camera_right.x, 1);
		EXPECT_FLOAT_EQ(camera_right.y, 0);
		EXPECT_FLOAT_EQ(camera_right.z, 0);
	}
}

TEST_F(CameraTests, UpdateFront)
{
	
	{
		// We test by rotating the Camera around Y Axis to see if the right gets updated.
		c.SetFront(glm::vec3(1, 0, 0));

		// Check if the Right Vector is calculated properly. It should be 1, 0, 0.
		// Check if the Camera Position is set properly
		glm::vec3 camera_right = c.GetRight();
		EXPECT_FLOAT_EQ(camera_right.x, 0);
		EXPECT_FLOAT_EQ(camera_right.y, 0);
		EXPECT_FLOAT_EQ(camera_right.z, 1);
	}

	{
		// We test by setting the Camera to look at (1, -1, 0).
		// Now the right should be (0, 0, 1) and Up should be (1, 1, 0). All normalized.
		c.SetFront(glm::vec3(1, -1, 0));

		glm::vec3 camera_right = c.GetRight();

		EXPECT_FLOAT_EQ(camera_right.x, 0);
		EXPECT_FLOAT_EQ(camera_right.y, 0);
		EXPECT_FLOAT_EQ(camera_right.z, 1);

		float d = (1.0f / glm::sqrt(2));
		glm::vec3 camera_up = c.GetUp();
		EXPECT_FLOAT_EQ(camera_up.x, d);
		EXPECT_FLOAT_EQ(camera_up.y, d);
		EXPECT_FLOAT_EQ(camera_up.z, 0);
	}
	
}

TEST_F(CameraTests, ViewMatrixCheck)
{
	/* @see https://learnopengl.com/Getting-started/Camera */

	{
		// Get the first Item from the view matrix.
		float first_item = c.GetViewMatrix()[0][0];

		EXPECT_FLOAT_EQ(first_item, c.GetRight().x);
		EXPECT_FLOAT_EQ(c.GetViewMatrix()[3][3], 1);

	}

	{
		c.SetFront(glm::vec3(1, 0, 0));
		// Since the Right would be 0.
		EXPECT_FLOAT_EQ(c.GetViewMatrix()[0][0], 0);
		EXPECT_FLOAT_EQ(c.GetViewMatrix()[3][3], 1);
	}

}

TEST_F(CameraTests, CheckCameraMovement)
{
	c.ProcessKeyboard(piolot::Camera::forward, 1.0f);
	// The position should be at - default speed along Z Axis.
	glm::vec3 camera_position = c.GetPosition();
	EXPECT_FLOAT_EQ(camera_position.x, 0);
	EXPECT_FLOAT_EQ(camera_position.y, 0);
	EXPECT_FLOAT_EQ(camera_position.z, -1 * c.GetMovementSpeed());
}

TEST_F(CameraTests, CheckMouseRayDirection)
{
	c.SetFront(glm::vec3(0, 0, -1));

	glm::vec3 resultant_ray = c.GetMouseRayDirection(400, 300, 800, 600, glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f));

	EXPECT_FLOAT_EQ(resultant_ray.x, 0);
	EXPECT_FLOAT_EQ(resultant_ray.y, 0);
	EXPECT_FLOAT_EQ(resultant_ray.z, -1);

}