#pragma once
#include <string>
#include <glm/mat4x4.hpp>


namespace piolot
{
	class Entity
	{

	protected:
		std::string shaderName;

	public:
		const std::string& GetShaderName() const
		{
			return shaderName;
		}

		const std::string& GetObjectName() const
		{
			return objectName;
		}

		const glm::vec3& GetPosition() const
		{
			return position;
		}

		const glm::vec3& GetRotation() const
		{
			return rotation;
		}

		const glm::vec3& GetScale() const
		{
			return scale;
		}

		const glm::mat4& GetModelMatrix() const
		{
			return modelMatrix;
		}

		bool IsMatrixDirty() const
		{
			return matrixDirty;
		}

	protected:
		std::string objectName;

		glm::vec3 position{};
	public:
		void SetPosition(const glm::vec3& _position)
		{
			position = _position;
			matrixDirty = true;
		}

		void SetRotation(const glm::vec3& _rotation)
		{
			rotation = _rotation;
			matrixDirty = true;
		}

		void SetScale(const glm::vec3& _scale)
		{
			scale = _scale;
			matrixDirty = true;
		}

	protected:
		glm::vec3 rotation{};
		glm::vec3 scale{};

		glm::mat4 modelMatrix{};

		bool matrixDirty = false;

		void UpdateMatrices();

	public:

		Entity(const std::string& _objectName, const std::string& _shaderName);
		~Entity() = default;

		void Update(float _deltaTime);
		void Render();

	};
}
