#include "Sprite.h"

#include <Actors/Actor.h>
#include <Rendering/Texture.h>

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

namespace Tristeon
{
	REGISTER_TYPE_CPP(Sprite)
	
	Sprite::Sprite()
	{
		texture = std::make_unique<Texture>();
	}

	json Sprite::serialize()
	{
		json j = Actor::serialize();
		j["typeID"] = TRISTEON_TYPENAME(Sprite);
		j["width"] = width;
		j["height"] = height;
		j["colour"] = colour;
		j["texturePath"] = texture->getPath();
		return j;
	}

	void Sprite::deserialize(json j)
	{
		Actor::deserialize(j);
		
		width = j["width"];
		height = j["height"];

		colour = j["colour"];

		std::string texturePath = j["texturePath"];
		if (texturePath != texture->getPath()) //Update if new path
			texture = std::make_unique<Texture>(texturePath);
	}

	void Sprite::setTexture(std::string const& path, bool const& setSize)
	{
		texture = std::make_unique<Texture>(path); //TODO: Texture caching through an asset loader of some sorts

		if (setSize)
		{
			width = texture->width();
			height = texture->height();
		}
	}

	void Sprite::render(QOpenGLShaderProgram* program)
	{
		QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
		f->glActiveTexture(GL_TEXTURE0);
		texture->bind();

		//Sprite info
		program->setUniformValue("sprite.width", width);
		program->setUniformValue("sprite.height", height);
		program->setUniformValue("sprite.colour", colour.r, colour.g, colour.b, colour.a);
		
		program->setUniformValue("actor.position", position.x, position.y);
		program->setUniformValue("actor.scale", scale.x, scale.y);
		program->setUniformValue("actor.rotation", -rotation);

		f->glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	Shader* Sprite::getShader()
	{
		static Shader shader = Shader("Internal/Shaders/Sprite.vert", "Internal/Shaders/Sprite.frag");
		return &shader;
	}
}