#include "Sprite.h"

#include <Actors/Actor.h>
#include <Rendering/Texture.h>

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include "BehaviourCollector.h"
#include "Rendering/SpriteRenderMode.h"

namespace Tristeon
{
	Sprite::Sprite()
	{
		BehaviourCollector<Sprite>::add(this);
	}

	Sprite::~Sprite()
	{
		BehaviourCollector<Sprite>::remove(this);
	}

	SpriteRenderMode Sprite::getRenderMode()
	{
		return SpriteRenderMode::Normal;
	}

	void Sprite::setTexture(std::string const& path, bool setSize)
	{
		texturePath = path;
		texture = std::make_unique<Texture>(path); //TODO: Texture caching through an asset loader of some sorts

		if (setSize)
		{
			width = texture->width();
			height = texture->height();
		}
	}

	void Sprite::render(QOpenGLShaderProgram* program)
	{
		texture->bind();

		//Sprite info
		program->setUniformValue("sprite.renderMode", static_cast<int>(getRenderMode()));
		program->setUniformValue("sprite.width", width);
		program->setUniformValue("sprite.height", height);
		program->setUniformValue("actor.position", owner()->position.x, owner()->position.y);
		program->setUniformValue("actor.scale", owner()->scale.x, owner()->scale.y);
		program->setUniformValue("actor.rotation", owner()->rotation);

		//Animation
		QOpenGLContext::currentContext()->functions()->glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
