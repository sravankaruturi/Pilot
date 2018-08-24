#pragma once
#include <glm/detail/type_vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <string>

namespace piolot
{

	const float default_camera_speed = 2.0f;
	const float default_camera_mouse_sensitivity = 1.0f / 128;

	class Camera
	{

	protected:

		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;

		glm::vec3 worldUp;

		float movementSpeed = 0.0f;
		float mouseSensitivity = 0.0f;

		glm::mat4 viewMatrix;

		std::string cameraName;

	public:
		std::string& GetCameraName()
		{
			return cameraName;
		}


		Camera(const std::string& _cameraName, glm::vec3 _position, glm::vec3 _front, glm::vec3 _worldUp, float _movementSpeed = default_camera_speed, float _mouseSensitivity = default_camera_mouse_sensitivity)
			: position(_position),
			  front(_front),
			  worldUp(_worldUp),
			  movementSpeed(_movementSpeed),
			  mouseSensitivity(_mouseSensitivity),
			  cameraName(_cameraName)
		{
			UpdateVectors();
		}

		glm::vec3& GetPosition()
		{
			return position;
		}

		void SetPosition(const glm::vec3& _position)
		{
			position = _position;
		}

		const glm::vec3& GetFront() const
		{
			return front;
		}

		void SetFront(const glm::vec3& _front)
		{
			front = _front;
			UpdateVectors();
		}

		const glm::vec3& GetUp() const
		{
			return up;
		}

		const glm::vec3& GetRight() const
		{
			return right;
		}

		const glm::vec3& GetWorldUp() const
		{
			return worldUp;
		}

		float GetMovementSpeed() const
		{
			return movementSpeed;
		}

		void SetMovementSpeed(float _movementSpeed)
		{
			movementSpeed = _movementSpeed;
		}

		float GetMouseSensitivity() const
		{
			return mouseSensitivity;
		}

		void SetMouseSensitivity(float _mouseSensitivity)
		{
			mouseSensitivity = _mouseSensitivity;
		}

		const glm::mat4& GetViewMatrix() const
		{
			return viewMatrix;
		}

		void SetViewMatrix(const glm::mat4& _viewMatrix)
		{
			viewMatrix = _viewMatrix;
		}

		void DisplayCameraDetailsImgui();

		enum camera_movement
		{
			forward,
			back,
			rightside,
			leftside
		};

		void ProcessKeyboard(camera_movement _direction, float _deltaTime);
		void ProcessMouseMovement(float _xoffset, float _yoffset);

		glm::vec3 GetMouseRayDirection(float _mouseX, float _mouseY, int _windowWidth, int _windowHeight, glm::mat4 _projectionMatrix);

		void UpdateVectors();
		void UpdateMatrices();
	};

}

