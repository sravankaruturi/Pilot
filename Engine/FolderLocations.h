//#define SHADER_FOLDER "../../Engine/Shaders/"

#define IS_HOME_PC	FALSE

#if IS_HOME_PC

#define SHADER_FOLDER			"C:/dev/Piolot/Engine/Shaders/"
#define TEXTURE_FOLDER			"C:/dev/Piolot/Engine/Assets/Textures/"
#define MODEL_FOLDER			"C:/dev/Piolot/Engine/Assets/Models/"
#define EXTERNAL_FILES			"C:/dev/Piolot/Engine/external_files/"

#else

#define BUFFER					"Z:/IGMProfile/Desktop/Projects/"

#define SHADER_FOLDER			"Z:/IGMProfile/Desktop/Projects/Piolot/Engine/Shaders/"
#define TEXTURE_FOLDER			"Z:/IGMProfile/Desktop/Projects/Piolot/Engine/Assets/Textures/"
#define MODEL_FOLDER			"Z:/IGMProfile/Desktop/Projects/Piolot/Engine/Assets/Models/"
#define EXTERNAL_FILES			"Z:/IGMProfile/Desktop/Projects/Piolot/Engine/external_files/"

#endif