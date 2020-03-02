#include "AnimationSprite.h"

#include <QOpenGLShaderProgram>

namespace Tristeon
{
	SpriteRenderMode AnimationSprite::getRenderMode()
	{
		return SpriteRenderMode::Animated;
	}

	void AnimationSprite::render(QOpenGLShaderProgram* program)
	{
		program->setUniformValue("animation.frame", frame);
		program->setUniformValue("animation.cols", cols);
		program->setUniformValue("animation.rows", rows);
		
		Sprite::render(program);
	}

	void AnimationSprite::update()
	{
		currentFrame += 0.1f;
		if (currentFrame > cols * rows)
			currentFrame = 0;
		frame = floor(currentFrame);
	}
}
