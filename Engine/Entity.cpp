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
		// This allocation would be cleared from the Asset Manager.
		Object * object = DBG_NEW Object(MDOEL_FOLDER + _objectPath);

		objectName = object->GetObjectName();

		if (!ASMGR.IsObjectLoaded(objectName))
		{
			ASMGR.objects.insert_or_assign(objectName, object);
		}
		
		modelMatrix = glm::mat4(1.0f);

		//boundingBox = BoundingBox(glm::vec3(-1.5, -1.5f, -1.5f), glm::vec3(1.5f, 1.5f, 1.5f));
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
		ASMGR.shaders.at(shaderName)->setMat4("model", modelMatrix);

		// Render
		ASMGR.objects.at(objectName)->Render(shaderName);
		// Render.
		//object->Render(shaderName);

		ASMGR.shaders.at("bbox")->use();
		ASMGR.shaders.at("bbox")->setMat4("model", glm::scale(modelMatrix, glm::vec3(1.0001f, 1.0001f, 1.0001f)));
		if ( selectedInScene )
		{
			boundingBox.Render(glm::vec3(1.0, 1.0, 0.0));
		}else
		{
			boundingBox.Render(glm::vec3(1.0, 0.0, 0.0));
		}
		
		
	}
}
