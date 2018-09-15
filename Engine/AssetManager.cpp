#include "AssetManager.h"

namespace piolot {

	inline AssetManager& AssetManager::getInstance()
	{
		static AssetManager instance; // Guaranteed to be destroyed and Instantiated on first use.
		return instance;
	}

	inline void AssetManager::ClearAllData()
	{
		//for (auto it : textures)
		//{
		//	delete it.second;
		//}

		///*for (auto it : shaders)
		//{
		//	delete it.second;
		//}*/

		//for (auto it: objects)
		//{
		//	// This is not an error since we have a properly defined destructor. 
		//	// @see https://stackoverflow.com/questions/4325154/delete-objects-of-incomplete-type#4325223
		//	// This is because of the Forward Declaration.
		//	delete it.second;
		//}

		textures.clear();
		shaders.clear();
		objects.clear();
	}

	inline AssetManager::~AssetManager()
	{
		ClearAllData();
	}

	inline bool AssetManager::LoadShaders()
	{
		LOGGER.AddToLog("Loading Shaders...");
		// Load all the shaders in the directory and compile them.
		for (auto& p : std::experimental::filesystem::directory_iterator(shaderDir))
		{
			// *. Check only vert extensions.
			std::string extension = p.path().extension().generic_string();
			if (".vert" != extension && ".shader" != extension)
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
				if (".vert" == extension)
				{
					for (auto i = 0; i < 5; i++)
						file_name.pop_back();
				}
				else if (".shader" == extension)
				{
					for (auto i = 0; i < 7; i++)
						file_name.pop_back();
				}


				// *. Get the key for the Map.
				// *. Check if the Shader already exists and skip if it does log the anamoly.
				if (this->shaders.count(file_name))
				{
					continue;
				}

				if (".vert" == extension)
				{
					// *. Get the second file as well.
					// *. Create the Shader and load it in the map.
					this->shaders.insert_or_assign(file_name, std::make_shared<GLShader>(
						p.path().generic_string().c_str(),
						(shaderDir + std::string("/") + file_name + std::string(".frag"))
						.c_str()));
				}
				else if (".shader" == extension)
				{
					this->shaders.insert_or_assign(file_name, std::make_shared<GLShader>(
						p.path().generic_string().c_str()
						)
					);
				}

				LOGGER.AddToLog("Loaded " + file_name + " Shader.");
			}
			catch (...)
			{
				return false;
			}
		}

		LOGGER.AddToLog("Loaded Shaders Successfully. \n");

		return true;
	}

	inline bool AssetManager::LoadTextures()
	{
		LOGGER.AddToLog("Loading Textures..");
		for (auto& p : std::experimental::filesystem::directory_iterator(textureDir))
		{
			std::string file_name;
			try
			{
				file_name = p.path().filename().generic_string();
				while (file_name.back() != '.')
				{
					file_name.pop_back();
				}
				file_name.pop_back();

				this->textures.insert_or_assign(file_name, std::make_shared<Texture>(p.path().generic_string()));
			}
			catch (...)
			{
				return false;
			}
			LOGGER.AddToLog("Loaded " + file_name + " Texture");
		}

		LOGGER.AddToLog("Loaded Textures.. \n");

		return true;
	}

	inline bool AssetManager::IsTextureLoaded(std::string _key)
	{
		return (!(textures.find(_key) == textures.end()));
	}

	inline bool AssetManager::IsObjectLoaded(std::string _key)
	{
		return (!(objects.find(_key) == objects.end()));
	}

	inline bool AssetManager::AddToTextures(const std::string& _name, std::shared_ptr<Texture> _texture)
	{
		if (IsTextureLoaded(_name)) return false;
		try
		{
			this->textures.insert_or_assign(_name, _texture);
			LOGGER.AddToLog("Adding " + _name + " to textures.");
		}
		catch (...)
		{
			return false;
		}
		return true;
	}

	inline bool AssetManager::AddToObjects(const std::string& _name, std::shared_ptr<Object> _object)
	{
		if (IsObjectLoaded(_name)) return false;
		try
		{
			this->objects.insert_or_assign(_name, _object);
			LOGGER.AddToLog("Adding " + _name + " to Objects.");
		}
		catch (...)
		{
			return false;
		}
		return true;
	}

	inline void AssetManager::GuiRender(bool* _windowFlag)
	{
		// A Hierarchy listing all the Assets currently in the ASMGR.
		ImGui::Begin("Asset Manager Details", _windowFlag);
		ImGui::Columns(1);
		if (ImGui::CollapsingHeader("Shaders"))
		{
			for (auto it : this->shaders)
			{
				ImGui::Columns(2);
				ImGui::Text(it.first.c_str());
				ImGui::NextColumn();
				ImGui::Text("%d", it.second->shaderId);
				ImGui::NextColumn();
			}
		}

		ImGui::Columns(1);
		if (ImGui::CollapsingHeader("Textures"))
		{
			for (auto it : this->textures)
			{
				ImGui::Columns(3);
				ImGui::Text(it.first.c_str());
				ImGui::NextColumn();
				ImGui::Text("%d", it.second->GetTextureId());
				ImGui::NextColumn();
				ImGui::Image((void *)it.second->GetTextureId(), ImVec2(30, 30));
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Image((void*)it.second->GetTextureId(), ImVec2(200, 200));
					ImGui::EndTooltip();
				}
				ImGui::NextColumn();
			}
		}
		ImGui::End();
	}

}