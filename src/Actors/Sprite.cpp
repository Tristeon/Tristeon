#include "Sprite.h"

#include <Actors/Actor.h>
#include <Rendering/Texture.h>

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <Rendering/SpriteRenderMode.h>

#include "Collector.h"

namespace Tristeon
{
	REGISTER_TYPE_CPP(Sprite)
	
	Sprite::Sprite()
	{
		ActorCollector<Sprite>::add(this);
		texture = std::make_unique<Texture>();
	}

	Sprite::~Sprite()
	{
		ActorCollector<Sprite>::remove(this);
	}

	json Sprite::serialize()
	{
		json j = Actor::serialize();
		j["typeID"] = TRISTEON_TYPENAME(Sprite);
		j["width"] = width;
		j["height"] = height;
		j["texturePath"] = texture->getPath();

		j["spacing"]["left"] = spacingLeft;
		j["spacing"]["right"] = spacingLeft;
		j["spacing"]["top"] = spacingTop;
		j["spacing"]["bottom"] = spacingBottom;
		return j;
	}

	void Sprite::deserialize(json j)
	{
		Actor::deserialize(j);
		
		width = j["width"];
		height = j["height"];

		std::string texturePath = j["texturePath"];
		if (texturePath != texture->getPath()) //Update if new path
			texture = std::make_unique<Texture>(texturePath);

		spacingLeft = j["spacing"]["left"];
		spacingRight = j["spacing"]["right"];
		spacingTop = j["spacing"]["top"];
		spacingBottom = j["spacing"]["bottom"];
	}

	SpriteRenderMode Sprite::getRenderMode()
	{
		return SpriteRenderMode::Normal;
	}

	void Sprite::setTexture(std::string const& path, bool setSize)
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
		program->setUniformValue("sprite.renderMode", static_cast<int>(getRenderMode()));
		program->setUniformValue("sprite.width", width);
		program->setUniformValue("sprite.height", height);
		program->setUniformValue("actor.position", position.x, position.y);
		program->setUniformValue("actor.scale", scale.x, scale.y);
		program->setUniformValue("actor.rotation", rotation);

		program->setUniformValue("spacing.left", spacingLeft);
		program->setUniformValue("spacing.right", spacingRight);
		program->setUniformValue("spacing.top", spacingTop);
		program->setUniformValue("spacing.bottom", spacingBottom);

		f->glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
