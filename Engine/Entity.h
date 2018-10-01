#pragma once
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

		bool selectedInScene = false;

		std::string entityName;

		std::string objectName;

		glm::vec3 position{};

		glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

		glm::mat4 modelMatrix{};

		bool matrixDirty = true;

		glm::vec3 boundingBoxLeastVertex{-1, -1, -1};
		glm::vec3 boundingBoxHighVertex{1, 1, 1};

		BoundingBox boundingBox;

		/**
		 * \brief End Node for the Pathing.
		 * 
		 * We use the Current Position as the Start Position.
		 */
		glm::ivec2 targetNode{};

	public:
		glm::ivec2 GetTargetPosition() const
		{
			return targetNode;
		}

		void setTargetNode(const glm::ivec2& _targetNode)
		{
			targetNode = _targetNode;
		}

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
			this->objectName = _objectName;
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

		/**
		 * \brief Save the Entity to the OutputStream.
		 * \param _out The Output Stream.
		 */
		void SaveToFile(std::ofstream& _out);

		/**
		 * \brief Load the Entity from the InputStream
		 * \param _in The Input Stream
		 */
		void LoadFromFile(std::ifstream& _in);

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

		void UpdateMatrices();

	public:

		Entity() = default;

		Entity(const std::string& _entityName, const std::string& _objectPath, const std::string& _shaderName, glm::vec3 _boundingBoxLeast = glm::vec3( -1, -1, -1 ), glm::vec3 _boundingBoxHighest = glm::vec3( 1, 1, 1 ));
		~Entity() = default;

		void Update(float _deltaTime);
		void Render();

		bool CheckIfMouseOvered(const glm::vec3 _cameraPosition, const glm::vec3 _mouseRayDirection, float& _distance) const;

		void DisplayDetailsImgui();

	};
}
