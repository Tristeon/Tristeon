#pragma once
#include "Sprite.h"

namespace Tristeon
{
	class AnimationSprite : public Sprite
	{
	public:
		int frame = 0;
		int rows = 1;
		int cols = 1;

		SpriteRenderMode getRenderMode() override;
	protected:
		void render(QOpenGLShaderProgram* program) override;
		void update() override;

		float currentFrame = 0;
	};
}
