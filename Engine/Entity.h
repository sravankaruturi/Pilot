#pragma once
#include <string>
#include <glm/mat4x4.hpp>
#include "BoundingBox.h"

namespace piolot
{
	class Entity
	{

	protected:
		std::string shaderName;

		BoundingBox boundingBox;

		bool selectedInScene = false;

	public:
		bool IsSelectedInScene() const
		{
			return selectedInScene;
		}

		void SetSelectedInScene(bool _selectedInScene)
		{
			selectedInScene = _selectedInScene;
		}

		const BoundingBox& GetBoundingBox() const
		{
			return boundingBox;
		}

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
		glm::vec3 rotation{0.0f, 0.0f, 0.0f};
		glm::vec3 scale{1.0f, 1.0f, 1.0f};

		glm::mat4 modelMatrix{};

		bool matrixDirty = true;

		void UpdateMatrices();

	public:

		Entity(const std::string& _objectName, const std::string& _shaderName);
		~Entity() = default;

		void Update(float _deltaTime);
		void Render();

	};
}
