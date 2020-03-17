#pragma once
#include <memory>
#include <Rendering/Texture.h>
#include <Serialization/Serializable.h>
#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	struct Tile
	{
		//TODO: Tile info
	};

	/**
	 * A TileSet is an image that gets interpreted as if it were constructed out of separated tiles, 
	 * by dividing the texture into equally sized tiles using columns and rows.
	 *
	 * These tiles are simply split up in rows and columns by splitting up the width and the height of the texture.
	 * The current implementation only allows tiles of the same size, split up equally over the texture, but this may change in the future.
	 */
	class TileSet : public Serializable
	{
		REGISTER_TYPE_H(TileSet)
	public:
		/**
		 * Creates an empty tileset with a white texture.
		 */
		explicit TileSet();

		/**
		 * Creates a tileset with the given values, this is shorthand for simply assigning the values after creation.
		 */
		explicit TileSet(std::string const& imagePath, 
			uint const& cols, 
			uint const& rows, 
			Tile tileInfo[], 
			int const& tileRenderWidth = -1, 
			int const& tileRenderHeight = -1,

			uint const& spacingLeft = 0,
			uint const& spacingRight = 0,
			uint const& spacingTop = 0,
			uint const& spacingBottom = 0,
			uint const& horizontalSpacing = 0, 
			uint const& verticalSpacing = 0
		);

		json serialize() override;
		void deserialize(json j) override;

		/**
		 * The amount of columns the tileset has.
		 * Can otherwise be described as how many tiles the tileset is split up in horizontally.
		 */
		unsigned int cols = 1;

		/**
		 * The amount of rows the tileset has.
		 * Can otherwise be described as how many tiles the tileset is split up in vertically.
		 */
		unsigned int rows = 1;

		/**
		 * The width in pixels that the tiles are to be rendered at on screen.
		 */
		unsigned int tileRenderWidth = 0;

		/**
		 * The height in pixels that the tiles are to be rendered at on screen.
		 */
		unsigned int tileRenderHeight = 0;

		/**
		 * The spacing/cutoff in pixels on the left of the texture.
		 */
		unsigned int spacingLeft = 0;
		
		/**
		 * The spacing/cutoff in pixels on the right of the texture.
		 */
		unsigned int spacingRight = 0;

		/**
		 * The spacing/cutoff in pixels on the top of the texture.
		 */
		unsigned int spacingTop = 0;

		/**
		 * The spacing/cutoff in pixels on the bottom of the texture.
		 */
		unsigned int spacingBottom = 0;

		/**
		 * The horizontal spacing in pixels between tiles.
		 */
		unsigned int horizontalSpacing = 0;

		/**
		 * The vertical spacing in pixels between tiles.
		 */
		unsigned int verticalSpacing = 0;

		/**
		 * Tile specific information to help inform the engine about collisions or other tile-based behaviour. 
		 */
		std::unique_ptr<Tile[]> tileInfo = nullptr;

		/**
		 * The texture used by the tileset.
		 */
		std::unique_ptr<Texture> texture = nullptr;
	};
}
