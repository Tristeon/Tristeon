#pragma once
#include "Sprite.h"
#include <Callbacks/IUpdate.h>

namespace Tristeon
{
	class AnimationClip;

	/**
	 * An AnimationSprite is a Sprite that uses AnimationClips to divide the Sprite's Texture up in separate "frames", which it then iterates over to create an animation.
	 *
	 * The animation behaviour is defined in the AnimationClip itself, the AnimationSprite then uses that data to play the animation.
	 */
	class AnimationSprite : public Sprite, public IUpdate
	{
	public:
		AnimationSprite() = default;
		virtual ~AnimationSprite() = default;

		DELETE_COPY(AnimationSprite);
		DEFAULT_MOVE(AnimationSprite);

		json serialize() override;
		void deserialize(json j) override;

		/**
		 * Sets if the animation is paused.
		 */
		void setPaused(bool const& value);
		
		/**
		 * Sets the current AnimationClip. Resets the frame counter to 0.
		 */
		void setAnimationClip(String const& clipPath);

		/**
		 * Sets the frame of the animation. Clamped to animationclip start and end.
		 */
		void setFrame(unsigned int const& frame);
	protected:
		void render() override;
		void update() override;
		Shader* getShader() override;

		float _currentFrame = 0;
		AnimationClip* _clip = nullptr;
		String _clipPath;
		
		bool _paused = false;
	};

	REGISTER_TYPE(AnimationSprite);
}