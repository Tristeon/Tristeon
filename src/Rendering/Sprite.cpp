#include "Sprite.h"
#include <AssetManagement/Resources.h>
#include <Serialization/MetaWrappers/TexturePath.h>

#include "glad/glad.h"
#include <magic_enum.hpp>

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
		j["size"] = size;
		j["flip"] = flip;
		j["colour"] = colour;
		j["texturePath"] = TexturePath{ _albedoPath };
		j["normalPath"] = TexturePath{ _normalPath };
		j["lightMaskPath"] = TexturePath{ _lightMaskPath };
		j["normalMapStrength"] = _normalMapStrength;
		return j;
	}

	void Sprite::deserialize(json j)
	{
		Graphic::deserialize(j);

		size = j.value("size", VectorU { 64, 64 });
		flip = j.value("flip", VectorB(false, false));

		colour = j.value("colour", Colour());

		auto const newAlbedoPath = j.value("texturePath", json()).value("path", "");
		if (newAlbedoPath != _albedoPath)
		{
			_albedo = Resources::assetLoad<Texture>(newAlbedoPath);
			_albedoPath = newAlbedoPath;
		}
		if (!_albedo)
			_albedo = Resources::assetLoad<Texture>(Texture::defaultPath);

		auto const newNormalPath = j.value("normalPath", json()).value("path", "");
		if (newNormalPath != _normalPath)
		{
			_normal = Resources::assetLoad<Texture>(newNormalPath);
			_normalPath = newNormalPath;
		}
		_normalMapStrength = j.value("normalMapStrength", 1.0f);
		
		auto const newLightMaskPath = j.value("lightMaskPath", json()).value("path", "");
		if (newLightMaskPath != _lightMaskPath)
		{
			_lightMask = Resources::assetLoad<Texture>(newLightMaskPath);
			_lightMaskPath = newLightMaskPath;
		}
	}

	void Sprite::setTexture(std::string const& pPath, bool const& pSetSize, const TextureType& pType)
	{
		Texture** texPtr = nullptr;
		String* pathPtr = nullptr;

		switch (pType)
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
			case TextureType::LightMask:
			{
				texPtr = &_lightMask;
				pathPtr = &_lightMaskPath;
			}
		}

		*texPtr = Resources::assetLoad<Texture>(pPath);
		*pathPtr = pPath;

		if (!*texPtr && pType == TextureType::Albedo)
		{
			*texPtr = Resources::assetLoad<Texture>(Texture::defaultPath);
			*pathPtr = Texture::defaultPath;
		}

		if (pSetSize)
		{
			size = { (*texPtr)->width(), (*texPtr)->height() };
		}
	}

	Texture* Sprite::texture(const TextureType& pType)
	{
		switch (pType)
		{
		case TextureType::Albedo:
			return _albedo;
		case TextureType::Normal:
			return _normal;
		case TextureType::LightMask:
			return _lightMask;
		}

		TRISTEON_LOG("Invalid argument passed, type needs to be a valid enum value");
		return nullptr;
	}

	String Sprite::texturePath(const TextureType& pType)
	{
		switch (pType)
		{
		case TextureType::Albedo:
			return _albedoPath;
		case TextureType::Normal:
			return _normalPath;
		case TextureType::LightMask:
			return _lightMaskPath;
		}

		TRISTEON_LOG("Invalid argument passed, type needs to be a valid enum value");
		return "";
	}

	void Sprite::render()
	{
		if (!_albedo)
			return;
		
		auto* shader = getShader();
		shader->bind();

		shader->setUniformValue("albedoMap", 0);
		glActiveTexture(GL_TEXTURE0);
		_albedo->bind();

		shader->setUniformValue("normalMap", 1);
		shader->setUniformValue("normalMapStrength", _normalMapStrength);
		shader->setUniformValue("normalMapEnabled", (bool)_normal);
		if (_normal)
		{
			glActiveTexture(GL_TEXTURE1);
			_normal->bind();
		}

		shader->setUniformValue("lightMask", 2);
		shader->setUniformValue("lightMaskEnabled", (bool)_lightMask);
		if (_lightMask)
		{
			glActiveTexture(GL_TEXTURE2);
			_lightMask->bind();
		}
		
		//Sprite info
		shader->setUniformValue("sprite.width", size.x);
		shader->setUniformValue("sprite.height", size.y);
		shader->setUniformValue("sprite.colour", colour.r, colour.g, colour.b, colour.a);

		shader->setUniformValue("sprite.flipX", flip.x);
		shader->setUniformValue("sprite.flipY", flip.y);
		shader->setUniformValue("sprite.renderMask", (int)magic_enum::enum_index<RenderMask>(renderMask).value_or(1)-1);
		
		shader->setUniformValue("actor.position", position.x, position.y);
		shader->setUniformValue("actor.scale", scale.x, scale.y);
		shader->setUniformValue("actor.rotation", -rotation);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		for (auto i = 0; i < 3; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	Shader* Sprite::getShader()
	{
		static Shader shader = Shader("Internal/Shaders/Sprite.vert", "Internal/Shaders/Sprite.frag");
		return &shader;
	}

	Graphic::Bounds Sprite::bounds()
	{
		Vector const halfSize = { size.x / 2.0f * scale.x, size.y / 2.0f * scale.y };
		return Bounds{ position - halfSize, position + halfSize };
	}
}