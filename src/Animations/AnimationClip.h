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
			uint left = 0;

			/**
			 * The spacing/cutoff in pixels on the right of the texture.
			 */
			uint right = 0;

			/**
			 * The spacing/cutoff in pixels on the top of the texture.
			 */
			uint top = 0;

			/**
			 * The spacing/cutoff in pixels on the bottom of the texture.
			 */
			uint bottom = 0;

			/**
			 * The horizontal spacing in pixels between frames.
			 */
			uint horizontalFrame = 0;

			/**
			 * The vertical spacing in pixels between frames.
			 */
			uint verticalFrame = 0;
		};

		/**
		 * The amount of columns the animationclip has.
		 * Can otherwise be described as how many frames the animationclip is split up in horizontally.
		 */
		uint cols = 0;

		/**
		 * The amount of rows the animationclip has.
		 * Can otherwise be described as how many frames the animationclip is split up in vertically.
		 */
		uint rows = 0;

		/**
		 * The first frame of the animationclip.
		 * Sometimes animationclips have more frames in one texture than the ones needed for the animation,
		 * so startIndex and endIndex allow you to define where the animation starts and ends.
		 */
		uint startIndex = 0;
		/**
		 * The last frame of the animationclip.
		 * Sometimes animationclips have more frames in one texture than the ones needed for the animation,
		 * so startIndex and endIndex allow you to define where the animation starts and ends.
		 */
		uint endIndex = 0;

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

		/**
		 * Creates an animationclip with the given values, this is shorthand for simply assigning the values after creation.
		 */
		AnimationClip(std::string const& texturePath, uint const& cols, uint const& rows, bool const& loops, int const& startIndex = -1, int const& endIndex = -1, Spacing const& spacing = { 0, 0, 0, 0, 0, 0 }, float const& playbackRate = 1);

		json serialize() override;
		void deserialize(json j) override;
	};
}