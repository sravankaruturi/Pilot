#include "Entity.h"
#include "Object.h"
#include "FolderLocations.h"
#include "AssetManager.h"
#include <glm/gtc/matrix_transform.inl>

namespace piolot
{
	void Entity::UpdateMatrices()
	{
		glm::mat4 model_matrix(1.0f);
		model_matrix = glm::translate(model_matrix, position);
		model_matrix = glm::rotate(model_matrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		model_matrix = glm::rotate(model_matrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		model_matrix = glm::rotate(model_matrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		model_matrix = glm::scale(model_matrix, scale);
		modelMatrix = model_matrix;
	}

	Entity::Entity(const std::string& _objectPath, const std::string& _shaderName)
		:shaderName(_shaderName), boundingBox(BoundingBox(glm::vec3(-1.0, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)))
	{
		
		objectName = _objectPath.substr(_objectPath.find_last_of('/') + 1, _objectPath.find_last_of('.') - _objectPath.find_last_of('/') - 1);

		if (!ASMGR.IsObjectLoaded(objectName))
		{
			// This allocation would be cleared from the Asset Manager.
			std::shared_ptr<Object> object = std::make_shared<Object>(MODEL_FOLDER + _objectPath);
			ASMGR.objects.insert_or_assign(objectName, object);
		}
		
		matrixDirty = true;

	}

	void Entity::Update(float _deltaTime)
	{
		if ( matrixDirty)
		{
			UpdateMatrices();
			matrixDirty = false;
		}
	}

	void Entity::Render()
	{

		// Use the Current Shader.
		ASMGR.shaders.at(shaderName)->use();
		// Set the Model Matrix.
		ASMGR.shaders.at(shaderName)->setMat4("u_ModelMatrix", modelMatrix);

		// Render
		ASMGR.objects.at(objectName)->Render(shaderName);
		// Render.
		//object->Render(shaderName);

		ASMGR.shaders.at("bbox")->use();
		ASMGR.shaders.at("bbox")->setMat4("u_ModelMatrix", glm::scale(modelMatrix, glm::vec3(1.001f, 1.001f, 1.001f)));
		if ( selectedInScene )
		{
			boundingBox.Render(glm::vec3(1.0, 1.0, 0.0));
		}else
		{
			boundingBox.Render(glm::vec3(1.0, 0.0, 0.0));
		}
		
		
	}

	bool Entity::CheckIfMouseOvered(const glm::vec3 _cameraPosition, const glm::vec3 _mouseRayDirection, float& _distance) const
	{
		// Checks if this entity is under the cursor. And updates the _distance to hold it.
		return this->boundingBox.CheckForCollisionWithRay(this->modelMatrix, this->scale, _cameraPosition, _mouseRayDirection, _distance);
	}
}
