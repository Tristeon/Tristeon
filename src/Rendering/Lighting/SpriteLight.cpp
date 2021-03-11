#include "SpriteLight.h"
#include <Rendering/Shader.h>
#include "AssetManagement/Resources.h"
#include "glad/glad.h"
#include "Rendering/Texture.h"

namespace Tristeon
{
	json SpriteLight::serialize()
	{
		json j = CompositeLight::serialize();
		j["typeID"] = Type<SpriteLight>::fullName();
		j["texture"] = _texturePath;
		j["width"] = _width;
		j["height"] = _height;
		j["flipX"] = _flipX;
		j["flipY"] = _flipY;
		j["colour"] = _colour;
		return j;
	}

	void SpriteLight::deserialize(json j)
	{
		CompositeLight::deserialize(j);
		_texturePath = j.value("texture", Texture::defaultPath);
		_texture = Resources::assetLoad<Texture>(_texturePath);
		if (!_texture)
			_texture = Resources::assetLoad<Texture>(Texture::defaultPath);

		_width = j.value("width", 1u);
		_height = j.value("height", 1u);

		_flipX = j.value("flipX", false);
		_flipY = j.value("flipY", false);

		_colour = j.value("colour", Colour());
	}

	void SpriteLight::render()
	{
		if (!_texture)
			_texture = Resources::assetLoad<Texture>(Texture::defaultPath);
		
		Shader* s = shader();
		s->bind();

		s->setUniformValue("sprite.width", _width);
		s->setUniformValue("sprite.height", _height);
		s->setUniformValue("sprite.colour", _colour.r, _colour.g, _colour.b, _colour.a);

		s->setUniformValue("sprite.flipX", _flipX);
		s->setUniformValue("sprite.flipY", _flipY);

		s->setUniformValue("actor.position", actor()->position.x, actor()->position.y);
		s->setUniformValue("actor.scale", actor()->scale.x, actor()->scale.y);
		s->setUniformValue("actor.rotation", -actor()->rotation);

		s->setUniformValue("intensity", _intensity);
		
		s->setUniformValue("texture", 0);
		glActiveTexture(GL_TEXTURE0);
		_texture->bind();

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	Shader* SpriteLight::shader()
	{
		static Shader shader{ "Internal/Shaders/Sprite.vert", "Internal/Shaders/SpriteLight.frag" };
		return &shader;
	}
}
