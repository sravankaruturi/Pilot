﻿#pragma once
#include <string>

namespace piolot
{

	class Texture
	{

	protected:

		unsigned int textureId;
		bool loaded = false;

	public:
		bool IsLoaded() const
		{
			return loaded;
		}

		void SetLoaded(bool _loaded)
		{
			loaded = _loaded;
		}

		unsigned GetTextureId() const
		{
			return textureId;
		}

		void SetTextureId(unsigned _textureId)
		{
			textureId = _textureId;
		}

		explicit Texture(const std::string& _imagePath);

		~Texture() = default;
	};
	
}