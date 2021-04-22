#include "Texture.h"
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Utils/Console.h"

namespace Tristeon
{
	const std::string Texture::defaultPath = "Internal/Textures/white.jpg";

	Texture::Texture(std::string const& path)
	{
		stbi_set_flip_vertically_on_load(true);
		int w = 0, h = 0, c = 0;
		auto* pixels = stbi_load(path.c_str(), &w, &h, &c, STBI_rgb_alpha);

		_width = (unsigned int)w;
		_height = (unsigned int)h;
		_channels = (unsigned int)c;
		
		if (!pixels || _width == 0 || _height == 0)
		{
			if (pixels)
				stbi_image_free(pixels);
			_valid = false;
			TRISTEON_WARNING("Failed to load texture " + path);
			return;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

		float borderColor[] = { 1, 1, 1, 1 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenTextures(1, &_texture);

		glBindTexture(GL_TEXTURE_2D, _texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(pixels);
		_valid = true;
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &_texture);
	}

	void Texture::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, _texture);
	}

	unsigned int Texture::width() const
	{
		return _width;
	}

	unsigned int Texture::height() const
	{
		return _height;
	}

	VectorI Texture::size() const
	{
		return { (int)_width, (int)_height };
	}

	bool Texture::loaded() const
	{
		return _valid;
	}
}