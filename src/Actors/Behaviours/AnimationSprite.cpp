#include "AnimationSprite.h"

#include <QOpenGLShaderProgram>

#include "Animations/AnimationClip.h"

namespace Tristeon
{
	SpriteRenderMode AnimationSprite::getRenderMode()
	{
		return SpriteRenderMode::Animated;
	}

	void AnimationSprite::setAnimationClip(AnimationClip* clip)
	{
		this->clip = clip;
		setTexture(clip->texturePath, false);
		currentFrame = 0;
	}

	void AnimationSprite::render(QOpenGLShaderProgram* program)
	{
		program->setUniformValue("animation.frame", static_cast<int>(floor(currentFrame)) + clip->startIndex);
		program->setUniformValue("animation.cols", clip->cols);
		program->setUniformValue("animation.rows", clip->rows);
		
		Sprite::render(program);
	}

	void AnimationSprite::update()
	{
		if (clip == nullptr)
			return;
		
		if (clip->startIndex + floor(currentFrame) >= clip->endIndex)
		{
			if (clip->loops)
				currentFrame = 0;
			else
				return; //Simply hold onto the last frame if we aren't looping
		}
		currentFrame += 0.1f;
	}
}
