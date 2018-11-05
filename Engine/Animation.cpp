#pragma once
#include "Animation.h"
#include "AssetManager.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include <assimp/postprocess.h>

#ifndef MAX_NUMBER_OF_BONES_PER_ENTITY
#define MAX_NUMBER_OF_BONES_PER_ENTITY 128
#endif

namespace pilot
{
	
	Animation::Animation(std::string _path)
	{

		// Read the file using Assimp.
		this->boneMatrices.reserve(MAX_NUMBER_OF_BONES_PER_ENTITY * sizeof(glm::mat4));

		std::string directory = _path.substr(0, _path.find_last_of('/'));
		std::string animation_name = _path.substr(_path.find_last_of('/') + 1, _path.find_last_of('.') - _path.find_last_of('/') - 1);

		if( ASMGR.IsAnimationLoaded(animation_name) )
		{
			LOGGER.AddToLog("File, " + animation_name + " skipped loading.");
			return;
		}

		Assimp::Importer assimp_importer;
		const aiScene * assimp_scene = assimp_importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		
		if (!assimp_scene || assimp_scene->mFlags & !AI_SCENE_FLAGS_INCOMPLETE || !assimp_scene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << assimp_importer.GetErrorString() << std::endl;
			return;
		}

		PE_EXPECT(assimp_scene->mNumAnimations > 0);

		const aiAnimation * current_animation = assimp_scene->mAnimations[0];



	}

}
