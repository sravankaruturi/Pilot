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
