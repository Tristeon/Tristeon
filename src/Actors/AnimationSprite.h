#pragma once
#include "Sprite.h"
#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	class AnimationClip;

	class AnimationSprite : public Sprite
	{
		REGISTER_TYPE_H(AnimationSprite)
	public:
		json serialize() override;
		void deserialize(json j) override;

		void setAnimationClip(AnimationClip* clip);
	protected:
		virtual void render(QOpenGLShaderProgram* program) override;
		virtual void update() override;
		virtual Shader* getShader() override;

		float currentFrame = 0;
		AnimationClip* clip = nullptr;
	};
}
