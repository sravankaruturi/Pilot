//#define SHADER_FOLDER "../../Engine/Shaders/"
#pragma once
#include "Configurations.h"

#if IS_HOME_PC

#define SHADER_FOLDER			"C:/dev/Piolot/Engine/Shaders/"
#define TEXTURE_FOLDER			"C:/dev/Piolot/Assets/Textures/"
#define MODEL_FOLDER			"C:/dev/Piolot/Assets/Models/"
#define SCENES_FOLDER			"C:/dev/Piolot/Assets/Scenes/"

#define EXTERNAL_FILES			"C:/dev/Piolot/EngineDeps/external_files/"

#else

#define BUFFER					"Z:/IGMProfile/Desktop/Projects/"

#define SHADER_FOLDER			"Z:/IGMProfile/Desktop/Projects/Piolot/Engine/Shaders/"
#define TEXTURE_FOLDER			"Z:/IGMProfile/Desktop/Projects/Piolot/Engine/Assets/Textures/"
#define MODEL_FOLDER			"Z:/IGMProfile/Desktop/Projects/Piolot/Engine/Assets/Models/"
#define SCENES_FOLDER			"Z:/IGMProfile/Desktop/Projects/Piolot/Engine/Assets/Scenes/"

#define EXTERNAL_FILES			"Z:/IGMProfile/Desktop/Projects/Piolot/Engine/external_files/"

#endif