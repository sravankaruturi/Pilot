﻿#pragma once
#include <string>
#include <glm/mat4x4.hpp>
#include "BoundingBox.h"
#include <glm/detail/_vectorize.hpp>

namespace piolot
{
	class Entity
	{

	protected:
		std::string shaderName;

		BoundingBox boundingBox;

		bool selectedInScene = false;

		std::string entityName;

	public:
		std::string GetEntityName() const
		{
			return entityName;
		}

		void SetEntityName(const std::string& entity_name)
		{
			entityName = entity_name;
		}

		void SetShaderName(const std::string& shader_name)
		{
			shaderName = shader_name;
		}

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

		void SetObjectName(const std::string& _objectName)
		{
			this->objectName = objectName;
			matrixDirty = true;
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

		Entity() = default;

		Entity(const std::string& _objectPath, const std::string& _shaderName);
		~Entity() = default;

		void Update(float _deltaTime);
		void Render();

		bool CheckIfMouseOvered(const glm::vec3 _cameraPosition, const glm::vec3 _mouseRayDirection, float& _distance) const;

		void DisplayDetailsImgui();

	};
}
