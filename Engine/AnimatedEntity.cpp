#include "AnimatedEntity.h"
#include "AssetManager.h"
#include "Object.h"

namespace piolot {



	AnimatedEntity::AnimatedEntity(const std::string & _entityName, const std::string & _objectPath, const std::string & _shaderName)
		: Entity(_entityName, _objectPath, _shaderName)
	{

		///* Load in the Animation Data here. */
		//this->entityName = _entityName;
		//this->shaderName = _shaderName;
		//this->boundingBox = BoundingBox(glm::vec3(-1.0, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		//this->objectName = _objectPath.substr(_objectPath.find_last_of('/') + 1, _objectPath.find_last_of('.') - _objectPath.find_last_of('/') - 1);

		//// We append the Object Name with _animated here.
		//this->objectName += "_animated";

		//if (!ASMGR.IsObjectLoaded(objectName))
		//{
		//	// This allocation would be cleared from the Asset Manager.
		//	std::shared_ptr<Object> object = std::make_shared<Object>(MODEL_FOLDER + _objectPath);
		//	ASMGR.objects.insert_or_assign(objectName, object);
		//}

		//matrixDirty = true;

	}
}
