#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace piolot
{
	void Camera::ProcessKeyboard(camera_movement _direction, float _deltaTime)
	{
		auto velocity = movementSpeed * _deltaTime;
		switch (_direction)
		{
		case forward:
			position += front * velocity;
			break;
		case back:
			position -= front * velocity;
			break;
		case rightside:
			position += right * velocity;
			break;
		case leftside:
			position -= right * velocity;
			break;
		}
		// Updating vectors is useless.
		UpdateMatrices();
	}

	void Camera::ProcessMouseMovement(float _xoffset, float _yoffset)
	{
		_xoffset *= mouseSensitivity;
		_yoffset *= mouseSensitivity;

		front += _xoffset * right;
		front += _yoffset * up;

		UpdateVectors();

	}

	glm::vec3 Camera::GetMouseRayDirection(float _mouseX, float _mouseY, int _windowWidth, int _windowHeight, glm::mat4 _projectionMatrix)
	{

		// Following the video at https://www.youtube.com/watch?v=DLKN0jExRIM

		const float normalized_x = (2.0f * _mouseX) / _windowWidth - 1.0f;
		const float normalized_y = 1.0f - (2.0f * _mouseY) / _windowHeight;

		glm::vec4 clip_coordinates = glm::vec4(normalized_x, normalized_y, -1.0f, 0.f);

		// Try using translate if multiplication does not work.
		glm::vec4 eye_coordinates = glm::inverse(_projectionMatrix) * clip_coordinates;

		eye_coordinates.z = -1.0f;
		eye_coordinates.w = 0.0f;

		glm::vec4 temp_word_coordinates = glm::inverse(viewMatrix) * eye_coordinates;

		// For some reason all it returns is in negative.. to the way it should be. Are we missing something somewhere?
		glm::vec3 world_coordinates = glm::vec3(temp_word_coordinates.x, temp_word_coordinates.y, temp_word_coordinates.z);
		world_coordinates = glm::normalize(world_coordinates);

		return world_coordinates;
	}

	void Camera::UpdateVectors()
	{

		front	= glm::normalize(front);
		right	= glm::normalize(glm::cross(front, worldUp));
		up		= glm::normalize(glm::cross(right, front));

		UpdateMatrices();

	}

	void Camera::UpdateMatrices()
	{
		viewMatrix = glm::lookAt(position, front + position, up);
	}
}
