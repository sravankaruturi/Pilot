#pragma once

#include <memory>
#include <map>
#include <filesystem>

#include "Texture.h"
#include "GLShader.h"
#include "FolderLocations.h"

#include "LoggingManager.h"


#ifdef _DEBUG
/*@see https://msdn.microsoft.com/en-us/library/x98tx3cf.aspx */
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#define ASMGR piolot::AssetManager::getInstance()

namespace piolot
{

	class Object;

	/**
	 * \brief The Singleton class that holds all the data for Shaders, Textures and Objects.
	 */
	class AssetManager
	{

	public:
		/**
		* \brief Returns the instance of the Singleton.
		* \return The reference to the static object, instance.
		*/
		static AssetManager& getInstance();

		/**
		 * \brief Remove all the data and clear the Asset Manager.
		 */
		void ClearAllData();

		/**
		* \brief Default Constructor.
		*/
		//AssetManager() = default;

		/**
		* \brief Destructor. Deletes all the Shaders and Textures stored here.
		*/
		~AssetManager();

		/**
		* \brief A Map of all the shaders that we load.
		*/
		std::map<std::string, std::shared_ptr<GLShader>> shaders;
		/**
		* \brief A Map of all the textures that we load.
		*/
		std::map<std::string, std::shared_ptr<Texture>> textures;

		/**
		* \brief A Map of all the Renderables loaded.
		*/
		std::map<std::string, std::shared_ptr<Object>> objects;

		/**
		* \brief The Shader Directory from where all the shaders are loaded.
		*/
		std::string shaderDir = SHADER_FOLDER;
		/**
		* \brief The Texture Directory from where all the Textures are loaded.
		*/
		std::string textureDir = TEXTURE_FOLDER;

		/**
		* \brief Loads all the shaders from the Shader Directory.
		*/
		bool LoadShaders();

		/**
		* \brief Loads all the Textures from the Texture Directory.
		*/
		bool LoadTextures();

		/**
		* \brief Returns if the texture, key is loaded.
		* \param _key The string, key to look for in the map.
		* \return Returns True if we can find the texture in the map.
		*/
		bool IsTextureLoaded(std::string _key);

		/**
		* \brief Returns if the mesh is loaded.
		* \param _key Key
		* \return True if we find the mesh with the key.
		*/
		bool IsObjectLoaded(std::string _key);

		/**
		* \brief Add a texture to the exisiting map.
		* \param _name The Key to place the Texture.
		* \param _texture The Texture to load and place.
		* \return Returns true if the Texture is succesfully loaded. False if something occurs.
		*/
		bool AddToTextures(const std::string& _name, std::shared_ptr<Texture> _texture);

		/**
		 * \brief Add an object to the Map.
		 * \param _name The Key
		 * \param _object The Object
		 * \return If the Object is already loaded, returns false. Else, returns True.
		 */
		bool AddToObjects(const std::string& _name, std::shared_ptr<Object> _object);


		/**
		 * \brief The ImGUI calls for the Asset Manager.
		 * \param _windowFlag The Window is Open Boolean
		 */
		void GuiRender(bool* _windowFlag);
	};

}
