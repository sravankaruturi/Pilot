#include "Texture.h"
#include <glad/glad.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../EngineDeps/external_files/stb/stb_image.h"
#endif

namespace piolot
{
	Texture::Texture(const std::string& _imagePath, bool _flip_image)
	{

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_set_flip_vertically_on_load(_flip_image);

		int width, height, nrChannels;

		unsigned char * data = stbi_load(_imagePath.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			// TODO: Base the RGB vs RGBA parameters based on the nrChannels.
			if (nrChannels == 4)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}

			glGenerateMipmap(GL_TEXTURE_2D);
			loaded = true;
		}
		else
		{
			loaded = false;
		}

		stbi_image_free(data);

		glBindTexture(GL_TEXTURE_2D, 0);

	}

}