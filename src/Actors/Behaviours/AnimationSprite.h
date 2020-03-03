#pragma once
#include "Sprite.h"

namespace Tristeon
{
	class AnimationClip;

	class AnimationSprite : public Sprite
	{
	public:
		SpriteRenderMode getRenderMode() override;

		void setAnimationClip(AnimationClip* clip);
	protected:
		void render(QOpenGLShaderProgram* program) override;
		void update() override;

		float currentFrame = 0;
		AnimationClip* clip = nullptr;
	};
}
