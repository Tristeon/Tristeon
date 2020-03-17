#pragma once
#include "Sprite.h"
#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	class AnimationClip;

	/**
	 * An AnimationSprite is a sprite that uses AnimationClips to divide the Sprite's Texture up in separate "frames", which it then iterates over to create an animation.
	 *
	 * The animation behaviour is mostly defined in the AnimationClip itself, the AnimationSprite then uses that data to play the animation.
	 */
	class AnimationSprite : public Sprite
	{
		REGISTER_TYPE_H(AnimationSprite)
	public:
		json serialize() override;
		void deserialize(json j) override;

		/**
		 * Sets the current AnimationClip. Resets the frame counter to 0.
		 */
		void setAnimationClip(AnimationClip* clip);
	protected:
		virtual void render(QOpenGLShaderProgram* program) override;
		virtual void update() override;
		virtual Shader* getShader() override;

		float currentFrame = 0;
		AnimationClip* clip = nullptr;
	};
}
