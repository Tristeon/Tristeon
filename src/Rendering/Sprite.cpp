#include "Sprite.h"
#include <AssetManagement/Resources.h>
#include <Serialization/MetaWrappers/TextureField.h>

#include <glad/glad.h>
#include <magic_enum.hpp>

#include <Rendering/DebugGL.h>

namespace Tristeon
{
	json Sprite::serialize()
	{
		json j = Graphic::serialize();
		j["typeID"] = Type<Sprite>::fullName();
		j["size"] = size;
		j["flip"] = flip;
		j["colour"] = colour;
		j["albedo"] = _albedo;
		j["normal"] = _normal;
		j["lightMask"] = _lightMask;
		j["normalMapStrength"] = _normalMapStrength;
		return j;
	}

	void Sprite::deserialize(json j)
	{
		Graphic::deserialize(j);

		size = j.value("size", VectorU { 64, 64 });
		flip = j.value("flip", VectorB(false, false));

		colour = j.value("colour", Colour());

		_albedo = j.value("albedo", TextureField(0));
		_normal = j.value("normal", TextureField(0));
		_lightMask = j.value("lightMask", TextureField(0));
		_normalMapStrength = j.value("normalMapStrength", 1.0f);
	}

	void Sprite::setTexture(std::string const& pPath, bool const& pSetSize, const TextureType& pType)
	{
		switch (pType)
		{
			case TextureType::Albedo:
			{
				_albedo = TextureField(pPath);
				if (pSetSize)
					size = { _albedo->width(), _albedo->height() };
				break;
			}
			case TextureType::Normal:
			{
				_normal = TextureField(pPath);
				if (pSetSize)
					size = { _normal->width(), _normal->height() };
				break;
			}
			case TextureType::LightMask:
			{
				_lightMask = TextureField(pPath);
				if (pSetSize)
					size = { _lightMask->width(), _lightMask->height() };
				break;
			}
		}
	}

	Texture* Sprite::texture(const TextureType& pType)
	{
		switch (pType)
		{
		case TextureType::Albedo:
			return *_albedo;
		case TextureType::Normal:
			return *_normal;
		case TextureType::LightMask:
			return *_lightMask;
		}

		TRISTEON_LOG("Invalid argument passed, type needs to be a valid enum value");
		return nullptr;
	}

	void Sprite::render()
	{
		auto* shader = getShader();
		shader->bind();

		shader->setUniformValue("albedoMap", 0);
		glActiveTexture(GL_TEXTURE0);
		TRISTEON_DEBUG_GL();
		_albedo->bind();
		TRISTEON_DEBUG_GL();

		shader->setUniformValue("normalMap", 1);
		shader->setUniformValue("normalMapStrength", _normalMapStrength);
		shader->setUniformValue("normalMapEnabled", (bool)_normal);
		if (_normal)
		{
			glActiveTexture(GL_TEXTURE1);
			TRISTEON_DEBUG_GL();
			_normal->bind();
			TRISTEON_DEBUG_GL();
		}

		shader->setUniformValue("lightMask", 2);
		shader->setUniformValue("lightMaskEnabled", (bool)_lightMask);
		if (_lightMask)
		{
			glActiveTexture(GL_TEXTURE2);
			TRISTEON_DEBUG_GL();
			_lightMask->bind();
			TRISTEON_DEBUG_GL();
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
		TRISTEON_DEBUG_GL();
	}

	Shader* Sprite::getShader()
	{
		static Shader shader = Shader("Engine://Shaders/Sprite.vert", "Engine://Shaders/Sprite.frag");
		return &shader;
	}

	Graphic::Bounds Sprite::bounds()
	{
		Vector const halfSize = { size.x / 2.0f * scale.x, size.y / 2.0f * scale.y };
		return Bounds{ position - halfSize, position + halfSize };
	}
}
