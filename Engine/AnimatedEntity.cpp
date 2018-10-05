#include "AnimatedEntity.h"
#include "AssetManager.h"
#include "Object.h"

namespace piolot {



	AnimatedEntity::AnimatedEntity(const std::string & _entityName, const std::string & _objectPath, const std::string & _shaderName, glm::vec3 _bboxLeast, glm::vec3 _bboxHigh)
		: Entity(_entityName, _objectPath, _shaderName, _bboxLeast, _bboxHigh)
	{

		//matrixDirty = true;
		for (int i = 0; i < 32; i++) {
			boneMatrices.push_back(glm::mat4(1.0f));
		}

	}

	void AnimatedEntity::PlayAnimation(float _deltaTime, float _currentTime)
	{

		Object * current_object = ASMGR.objects.at(objectName).get();

		float animation_time = current_object->GetLastAnimationUpdateTime();

		if ( _currentTime - animation_time >= _deltaTime) {

			animationTotalTime += _deltaTime;

			current_object->BoneTransform(animationTotalTime, this->boneMatrices);
			current_object->SetLastAnimationUpdateTime(_currentTime);

		}

	}

	void AnimatedEntity::Update(float _deltaTime)
	{
		Entity::Update(_deltaTime);
	}

	void AnimatedEntity::Render()
	{

		ASMGR.shaders.at(shaderName)->use();

		auto loc = ASMGR.shaders.at(shaderName)->GetUniformLocation("u_BoneMatrices");

		PE_GL(glUniformMatrix4fv(loc, boneMatrices.size(), GL_FALSE, &boneMatrices[0][0][0]));

		Entity::Render();

	}
}
