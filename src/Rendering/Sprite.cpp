#include "Sprite.h"
#include <AssetManagement/Resources.h>

#include "glad/glad.h"

namespace Tristeon
{
	Sprite::Sprite()
	{
		_texture = Resources::load<Texture>(Texture::defaultPath);
	}

	json Sprite::serialize()
	{
		json j = Graphic::serialize();
		j["typeID"] = Type<Sprite>::fullName();
		j["width"] = width;
		j["height"] = height;
		j["flipX"] = flipX;
		j["flipY"] = flipY;
		j["colour"] = colour;
		j["texturePath"] = _texturePath;
		return j;
	}

	void Sprite::deserialize(json j)
	{
		Graphic::deserialize(j);
		
		width = j.value("width", 1u);
		height = j.value("height", 1u);

		flipX = j.value("flipX", false);
		flipY = j.value("flipY", false);

		colour = j.value("colour", Colour());

		std::string const newPath = j.value("texturePath", "");
		if (newPath != _texturePath) //Update if new path
		{
			_texture = Resources::assetLoad<Texture>(newPath);
			_texturePath = newPath;
		}
		
		if (!_texture)
			_texture = Resources::assetLoad<Texture>(Texture::defaultPath);
	}

	void Sprite::setTexture(std::string const& path, bool const& setSize)
	{
		_texture = Resources::assetLoad<Texture>(path);
		_texturePath = path;
		
		if (!_texture)
		{
			_texture = Resources::assetLoad<Texture>(Texture::defaultPath);
			_texturePath = Texture::defaultPath;
		}
		
		if (setSize)
		{
			width = _texture->width();
			height = _texture->height();
		}
	}

	Texture* Sprite::texture()
	{
		return _texture;
	}

	void Sprite::render()
	{
	    if (!_texture)
	        return;

		auto* shader = getShader();
		
		glActiveTexture(GL_TEXTURE0);
		_texture->bind();

		//Sprite info
		shader->setUniformValue("sprite.width", width);
		shader->setUniformValue("sprite.height", height);
		shader->setUniformValue("sprite.colour", colour.r, colour.g, colour.b, colour.a);
		
		shader->setUniformValue("sprite.flipX", flipX);
		shader->setUniformValue("sprite.flipY", flipY);
		
		shader->setUniformValue("actor.position", position.x, position.y);
		shader->setUniformValue("actor.scale", scale.x, scale.y);
		shader->setUniformValue("actor.rotation", -rotation);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	Shader* Sprite::getShader()
	{
		static Shader shader = Shader("Internal/Shaders/Sprite.vert", "Internal/Shaders/Sprite.frag");
		return &shader;
	}

	Graphic::Bounds Sprite::bounds()
	{
		Vector const halfSize = { width / 2.0f * scale.x, height / 2.0f * scale.y };
		return Bounds{ position - halfSize, position + halfSize };
	}
}