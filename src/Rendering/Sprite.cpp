#include "Sprite.h"
#include <AssetManagement/Resources.h>

#include "glad/glad.h"

namespace Tristeon
{
	Sprite::Sprite()
	{
		_albedo = Resources::load<Texture>(Texture::defaultPath);
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
		j["texturePath"] = _albedoPath;
		j["normalPath"] = _normalPath;
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

		std::string const newAlbedoPath = j.value("texturePath", "");
		if (newAlbedoPath != _albedoPath)
		{
			_albedo = Resources::assetLoad<Texture>(newAlbedoPath);
			_albedoPath = newAlbedoPath;
		}
		if (!_albedo)
			_albedo = Resources::assetLoad<Texture>(Texture::defaultPath);
		
		std::string const newNormalPath = j.value("normalPath", "");
		if (newNormalPath != _normalPath)
		{
			_normal = Resources::assetLoad<Texture>(newNormalPath);
			_normalPath = newNormalPath;
		}
	}

	void Sprite::setTexture(std::string const& path, bool const& setSize, const TextureType& type)
	{
		Texture** texPtr = nullptr;
		String* pathPtr = nullptr;

		switch (type)
		{
			case TextureType::Albedo:
			{
				texPtr = &_albedo;
				pathPtr = &_albedoPath;
				break;
			}
			case TextureType::Normal:
			{
				texPtr = &_normal;
				pathPtr = &_normalPath;
				break;
			}
		}

		*texPtr = Resources::assetLoad<Texture>(path);
		*pathPtr = path;

		if (!*texPtr && type == TextureType::Albedo)
		{
			*texPtr = Resources::assetLoad<Texture>(Texture::defaultPath);
			*pathPtr = Texture::defaultPath;
		}

		if (setSize)
		{
			width = (*texPtr)->width();
			height = (*texPtr)->height();
		}
	}

	Texture* Sprite::texture(const TextureType& type)
	{
		switch (type)
		{
		case TextureType::Albedo:
			return _albedo;
		case TextureType::Normal:
			return _normal;
		}

		TRISTEON_LOG("Invalid argument passed, type needs to be a valid enum value");
		return nullptr;
	}

	void Sprite::render()
	{
		if (!_albedo)
			return;

		auto* shader = getShader();
		shader->bind();

		glActiveTexture(GL_TEXTURE0);
		_albedo->bind();

		if (_normal)
		{
			glActiveTexture(GL_TEXTURE1);
			_normal->bind();
			shader->setUniformValue("normalMapEnabled", true);
		}
		else
		{
			shader->setUniformValue("normalMapEnabled", false);
		}

		shader->setUniformValue("albedoMap", 0);
		shader->setUniformValue("normalMap", 1);
		
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