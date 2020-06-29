#include "Texture.h"
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Tristeon
{
	const std::string Texture::defaultPath = "Internal/Textures/white.jpg";

	Texture::Texture(std::string const& path)
	{
		stbi_set_flip_vertically_on_load(true);
		auto* pixels = stbi_load(path.c_str(), &w, &h, &c, STBI_rgb_alpha);
		
		if (!pixels || w == 0 || h == 0)
		{
			succeeded = false;
			std::cout << "Failed to load texture " << path << "\n";
			return;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

		float borderColor[] = { 1, 1, 1, 1 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(pixels);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &texture);
	}

	void Texture::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	int Texture::width() const
	{
		return w;
	}

	int Texture::height() const
	{
		return h;
	}

	Vector2Int Texture::size() const
	{
		return { w, h };
	}

	bool Texture::loaded() const
	{
		return succeeded;
	}
}
