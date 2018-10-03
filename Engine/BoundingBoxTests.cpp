#pragma once

#if DEBUG

#include "AllTests.h"
#include <glm/gtc/matrix_transform.inl>

TEST_F(AllTests, BoundingBoxInitializationCheck)
{

	piolot::BoundingBox bbox = piolot::BoundingBox(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	EXPECT_EQ(bbox.GetVerticesSize(), 24);

}

TEST_F(AllTests, BoundingBoxRayCollisionCheck)
{
	piolot::BoundingBox bbox = piolot::BoundingBox(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	EXPECT_EQ(bbox.GetVerticesSize(), 24);

	glm::vec3 rayOrigin(0.0f, 0.0f, 10.0f);
	glm::vec3 ray_direction(0.0f, 0.0f, -1.0f);

	// It should intersect the bbox, if the bbox is at origin.
	float distance = 0.0f;
	bool intersect = bbox.CheckForCollisionWithRay(glm::mat4(1.0f), glm::vec3(1.0f), rayOrigin, ray_direction, distance);

	EXPECT_TRUE(intersect);
	EXPECT_FLOAT_EQ(distance, 9.0f);
}

TEST_F(AllTests, BoundingBoxRayCollisionCheckWhenMoved)
{
	
	piolot::BoundingBox bbox = piolot::BoundingBox(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	EXPECT_EQ(bbox.GetVerticesSize(), 24);

	glm::vec3 rayOrigin(0.0f, 0.0f, 10.0f);
	glm::vec3 ray_direction(0.0f, 0.0f, -1.0f);

	glm::mat4 model_matrix(1.0f);
	model_matrix = glm::translate(model_matrix, glm::vec3(2.0f, 0.0f, 0.0f));

	glm::vec3 scale(1.0f);

	// It should intersect the bbox, if the bbox is at origin.
	float distance = 0.0f;
	bool intersect = bbox.CheckForCollisionWithRay(model_matrix, scale, rayOrigin, ray_direction, distance);

	EXPECT_FALSE(intersect);
	EXPECT_FLOAT_EQ(distance, 0.0f);

	model_matrix = glm::mat4(1.0f);
	model_matrix = glm::translate(model_matrix, glm::vec3(0.9f, 0.0f, 0.0f));

	intersect = bbox.CheckForCollisionWithRay(model_matrix, scale, rayOrigin, ray_direction, distance);

	EXPECT_TRUE(intersect);
	EXPECT_FLOAT_EQ(distance, 9.0f);

}

TEST_F(AllTests, BoundingBoxCollisionCheckWhenScaled)
{
	
	piolot::BoundingBox bbox = piolot::BoundingBox(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	EXPECT_EQ(bbox.GetVerticesSize(), 24);

	glm::vec3 rayOrigin(0.0f, 0.0f, 10.0f);
	glm::vec3 ray_direction(0.0f, 0.0f, -1.0f);

	glm::vec3 scale(2.0f, 2.0f, 1.0f);

	glm::mat4 model_matrix(1.0f);
	model_matrix = glm::translate(model_matrix, glm::vec3(1.5f, 0.0f, 0.0f));	// So the left face of the box is at 0.5f.
	model_matrix = glm::scale(model_matrix, scale);		// So the left face is now at -0.5f;

	// It should intersect the bbox, if the bbox is at origin.
	float distance = 0.0f;
	bool intersect = bbox.CheckForCollisionWithRay(model_matrix, scale, rayOrigin, ray_direction, distance);

	EXPECT_TRUE(intersect);
	EXPECT_FLOAT_EQ(distance, 9.0f);

}

#endif