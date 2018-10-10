#include "AnimatedEntity.h"
#include "AssetManager.h"
#include "Object.h"

#include <fstream>

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
		else {
			current_object->CopyBoneMatrices(boneMatrices);
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

	void AnimatedEntity::SaveToFile(std::ofstream& _out)
	{
		// We store the base entity first.
		Entity::SaveToFile(_out);

		// Now We store the Bone Matrices
		// Store the number of Bone Matrices
		int number_of_bone_matrices = boneMatrices.size();
		_out.write((char*)&number_of_bone_matrices, sizeof(int));
		for ( int i = 0 ; i < number_of_bone_matrices ; i++)
		{
			_out.write((char*)&(boneMatrices[i]), sizeof(glm::mat4));
		}

		_out.write((char*)&animationTotalTime, sizeof(float));
	}

	void AnimatedEntity::LoadFromFile(std::ifstream& _in)
	{

		// We load the base entity first.
		Entity::LoadFromFile(_in);
		int number_of_bone_matrices = 0;
		_in.read((char*)&number_of_bone_matrices, sizeof(number_of_bone_matrices));
		boneMatrices.clear();
		boneMatrices.resize(number_of_bone_matrices);
		for ( int i = 0 ; i < number_of_bone_matrices ; i++)
		{
			_in.read((char*)&(boneMatrices[i]), sizeof(glm::mat4));
		}

		_in.read((char*)&animationTotalTime, sizeof(float));
	}
}
