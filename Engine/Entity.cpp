#include "Entity.h"
#include "Object.h"
#include "FolderLocations.h"
#include "AssetManager.h"

namespace piolot
{
	Entity::Entity(const std::string& _objectPath, const std::string& _shaderName)
		:shaderName(_shaderName)
	{
		Object * object = DBG_NEW Object(MDOEL_FOLDER + _objectPath);

		objectName = object->GetObjectName();

		if (!ASMGR.IsObjectLoaded(objectName))
		{
			ASMGR.objects.insert_or_assign(objectName, object);
		}
		
		modelMatrix = glm::mat4(1.0f);
	}

	void Entity::Update(float _deltaTime)
	{
		
	}

	void Entity::Render()
	{

		// Use the Current Shader.
		ASMGR.shaders.at(shaderName)->use();
		// Set the Model Matrix.
		ASMGR.shaders.at(shaderName)->setMat4("model", modelMatrix);

		const auto object = ASMGR.objects.at(objectName);
		// Render.
		object->Render(shaderName);
		
	}
}
