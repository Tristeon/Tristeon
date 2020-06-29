#pragma once
#include "Rendering/Texture.h"

#include <Serialization/Serializable.h>
#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	/**
	 * An animation clip describes an animation for an animationsprite as a 2D texture split up into separate frames
	 * by dividing the texture into equally sized images using columns and rows.
	 *
	 * Unlike TileSet, the AnimationClip doesn't own its texture but stores a filepath to it instead.
	 * This is because AnimationSprite is designed to own and load in the texture.
	 */
	class AnimationClip : public Serializable
	{
		REGISTER_TYPE_H(AnimationClip)
		
	public:
		struct Spacing
		{
			/**
			 * The spacing/cutoff in pixels on the left of the texture.
			 */
			unsigned int left = 0;

			/**
			 * The spacing/cutoff in pixels on the right of the texture.
			 */
			unsigned int right = 0;

			/**
			 * The spacing/cutoff in pixels on the top of the texture.
			 */
			unsigned int top = 0;

			/**
			 * The spacing/cutoff in pixels on the bottom of the texture.
			 */
			unsigned int bottom = 0;

			/**
			 * The horizontal spacing in pixels between frames.
			 */
			unsigned int horizontalFrame = 0;

			/**
			 * The vertical spacing in pixels between frames.
			 */
			unsigned int verticalFrame = 0;
		};

		/**
		 * The amount of columns the animationclip has.
		 * Can otherwise be described as how many frames the animationclip is split up in horizontally.
		 */
		unsigned int cols = 0;

		/**
		 * The amount of rows the animationclip has.
		 * Can otherwise be described as how many frames the animationclip is split up in vertically.
		 */
		unsigned int rows = 0;

		/**
		 * The first frame of the animationclip.
		 * Sometimes animationclips have more frames in one texture than the ones needed for the animation,
		 * so startIndex and endIndex allow you to define where the animation starts and ends.
		 */
		unsigned int startIndex = 0;
		/**
		 * The last frame of the animationclip.
		 * Sometimes animationclips have more frames in one texture than the ones needed for the animation,
		 * so startIndex and endIndex allow you to define where the animation starts and ends.
		 */
		unsigned int endIndex = 0;

		/**
		 * The AnimationClip spacing, in case animation frames have extra space between each other.
		 */
		Spacing spacing;

		/**
		 * If the animation continuously loops (starts back over once it ends) or not.
		 *
		 * If false, the AnimationSprite will freeze on the last frame of this animationclip until a new clip is assigned.
		 */
		bool loops = false;

		/**
		 * The filepath for the Texture of this animationclip.
		 */
		std::string texturePath = "";

		/**
		 * A modifier for the playback rate of the animationclip.
		 * This modifier works as a multiplier in where 1 is the default speed and > 1 speeds up and < 1 slows down.
		 *
		 * TODO: AnimationClip::playbackRate doesn't support negative (backwards) playback rates yet.
		 */
		float playbackRate = 1;

		/**
		 * Creates an empty animationclip.
		 */
		AnimationClip() = default;

		json serialize() override;
		void deserialize(json j) override;
	};
}