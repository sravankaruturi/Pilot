#pragma once
#include "Texture.h"
#include <map>
#include "GLShader.h"
#include <filesystem>
#include "FolderLocations.h"

#ifdef _DEBUG
/*@see https://msdn.microsoft.com/en-us/library/x98tx3cf.aspx */
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

namespace piolot
{

	class AssetManager
	{
	public:
		///**
		//* \brief Returns the instance of the Singleton.
		//* \return The reference to the static object, instance.
		//*/
		//static AssetManager& getInstance()
		//{
		//	static AssetManager    instance; // Guaranteed to be destroyed and Instantiated on first use.
		//	return instance;
		//}

		/**
		* \brief Default Constructor.
		*/
		AssetManager() = default;

		/**
		* \brief Destructor. Deletes all the Shaders and Textures stored here.
		*/
		~AssetManager()
		{
			for (auto it : textures)
			{
				delete it.second;
			}

			for (auto it : shaders)
			{
				delete it.second;
			}

			//for (auto vectorOfRenderables : meshes)
			//{
			//	for (GLRenderable* eachRenderable : vectorOfRenderables.second)
			//	{
			//		// TODO: This is not an error since we have a properly defined constructor. 
			//		// @see https://stackoverflow.com/questions/4325154/delete-objects-of-incomplete-type#4325223
			//		// This is because of the Forward Declaration.
			//		// One way to fix this would be to move the implementaion of the singleton to its C++ file and then include the Header for GLRenderable.
			//		delete eachRenderable;
			//	}
			//}
		}

		/**
		* \brief A Map of all the shaders that we load.
		*/
		std::map<std::string, GLShader *> shaders;
		/**
		* \brief A Map of all the textures that we load.
		*/
		std::map<std::string, Texture *> textures;

		/**
		* \brief A Map of all the Renderables loaded.
		*/
		/*std::map<std::string, std::vector<GLRenderable *>> meshes;*/

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
		bool LoadShaders()
		{
			// Load all the shaders in the directory and compile them.
			for (auto& p : std::experimental::filesystem::directory_iterator(shaderDir))
			{
				// *. Check only vert extensions.
				std::string extension = p.path().extension().generic_string();
				if (".vert" != extension)
				{
					if (".frag" != extension)
					{
						// Extensions should be Vert or Frag.
						return false;
					}
					continue;
				}

				try
				{
					std::string file_name = p.path().filename().generic_string();

					// Important: This only works if the file extension is .vert.
					for (auto i = 0; i < 5; i++)
						file_name.pop_back();

					// *. Get the key for the Map.
					// *. Check if the Shader already exists and skip if it does log the anamoly.
					if (this->shaders.count(file_name))
					{
						continue;
					}

					// *. Get the second file as well.
					// *. Create the Shader and load it in the map.
					this->shaders.insert_or_assign(file_name, DBG_NEW GLShader(p.path().generic_string().c_str(), (shaderDir + std::string("/") + file_name + std::string(".frag")).c_str()));
				}catch(...)
				{
					return false;
				}

			}
			return true;
		}

		/**
		* \brief Loads all the Textures from the Texture Directory.
		*/
		bool LoadTextures()
		{
			for (auto& p : std::experimental::filesystem::directory_iterator(textureDir))
			{
				try
				{
					std::string file_name = p.path().filename().generic_string();
					while (file_name.back() != '.')
					{
						file_name.pop_back();
					}
					file_name.pop_back();

					this->textures.insert_or_assign(file_name, DBG_NEW Texture(p.path().generic_string()));
				}catch(...)
				{
					return false;
				}
				return true;
			}
		}

		/**
		* \brief Returns if the texture, key is loaded.
		* \param _key The string, key to look for in the map.
		* \return Returns True if we can find the texture in the map.
		*/
		bool IsTextureLoaded(std::string _key)
		{
			return (!(textures.find(_key) == textures.end()));
		}

		///**
		//* \brief Returns if the mesh is loaded.
		//* \param _key Key
		//* \return True if we find the mesh with the key.
		//*/
		//bool IsMeshLoaded(std::string _key)
		//{
		//	return (!(meshes.find(_key) == meshes.end()));
		//}

		/**
		* \brief Add a texture to the exisiting map.
		* \param _name The Key to place the Texture.
		* \param _texture The Texture to load and place.
		* \return Returns true if the Texture is succesfully loaded. False if something occurs.
		*/
		bool AddToTextures(const std::string& _name, Texture * _texture)
		{
			if (IsTextureLoaded(_name)) return false;

			try
			{
				this->textures.insert_or_assign(_name, _texture);
			}
			catch (...)
			{
				return false;
			}
			return true;
		}

	};
	
}
