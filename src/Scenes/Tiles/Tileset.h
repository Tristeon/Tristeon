#pragma once
#include <Serialization/Serializable.h>
#include <Serialization/TypeRegister.h>
#include <Math/Vector.h>

namespace Tristeon
{
	class Texture;

	/**
	 * A structure with basic information about a tile.
	 * It describes the tile's collider and physical properties.
	 */
	struct TileInfo
	{
		bool hasCollider = false;
		float density = 1.0f;
		float friction = 0.0f;
		float restitution = 0.0f;
	};
	void to_json(json& j, const TileInfo& p);
	void from_json(const json& j, TileInfo& p);

	/**
	 * A Tileset is an image that gets interpreted as if it were constructed out of separated tiles, 
	 * by dividing the texture into equally sized tiles using columns and rows.
	 *
	 * These tiles are simply split up in rows and columns by splitting up the width and the height of the texture.
	 * The current implementation only allows tiles of the same size, split up equally over the texture, but this may change in the future.
	 */
	class Tileset : public Serializable
	{
	public:
		/**
		 * Creates an empty tileset with a white texture.
		 */
		explicit Tileset();

		json serialize() override;
		void deserialize(json j) override;

		/**
		 * Gets the Tile data struct of the tile at {x, y}
		 */
		[[nodiscard]] TileInfo info(const int& x, const int& y) const;

		/**
		 * Gets the Tile data struct of the tile at {x, y}
		 */
		[[nodiscard]] TileInfo info(const VectorI& coords) const;
		
		/**
		 * Gets the Tile data struct of the tile at the given index.
		 */
		[[nodiscard]] TileInfo info(const int& index) const;

		/**
		 * Gets the size of a tile in pixels.
		 */
		[[nodiscard]] VectorI tileSize() const;

		/**
		 * Gets the size of the tiles in 0..1 range
		 */
		[[nodiscard]] Vector tileSizeNormalized() const;

		/**
		 * Gets the top-left corner of the tile on the image in pixels.
		 */
		[[nodiscard]] VectorI tileMin(const int& index) const;
		/**
		 * Gets the top-left corner of the tile on the image in pixels.
		 */
		[[nodiscard]] VectorI tileMin(const int& x, const int& y) const;
		/**
		 * Gets the top-left corner of the tile on the image in pixels.
		 */
		[[nodiscard]] VectorI tileMin(const VectorI& coords) const;

		/**
		 * Gets the bottom-left corner of the tile on the image in normalized 0..1 range.
		 */
		[[nodiscard]] Vector tileMinNormalized(const int& x, const int& y) const;
		/**
		 * Gets the bottom-left corner of the tile on the image in normalized 0..1 range.
		 */
		[[nodiscard]] Vector tileMinNormalized(const int& index) const;
		/**
		 * Gets the bottom-left corner of the tile on the image in normalized 0..1 range.
		 */
		[[nodiscard]] Vector tileMinNormalized(const VectorI& coords) const;
		
		/**
		 * Gets the bottom-right corner of the tile on the image in pixels.
		 */
		[[nodiscard]] VectorI tileMax(const int& index) const;
		/**
		 * Gets the bottom-right corner of the tile on the image in pixels.
		 */
		[[nodiscard]] VectorI tileMax(const int& x, const int& y) const;
		/**
		 * Gets the bottom-right corner of the tile on the image in pixels.
		 */
		[[nodiscard]] VectorI tileMax(const VectorI& coords) const;

		/**
		 * Gets the bottom-right corner of the tile on the image in normalized 0..1 range.
		 */
		[[nodiscard]] Vector tileMaxNormalized(const int& x, const int& y) const;
		/**
		 * Gets the bottom-right corner of the tile on the image in normalized 0..1 range.
		 */
		[[nodiscard]] Vector tileMaxNormalized(const int& index) const;
		/**
		 * Gets the bottom-right corner of the tile on the image in normalized 0..1 range.
		 */
		[[nodiscard]] Vector tileMaxNormalized(const VectorI& coords) const;
		
		/**
		 * Converts x,y coordinates to the tile index.
		 */
		[[nodiscard]] int tile(const int& x, const int& y) const;

		/**
		 * Converts x,y coordinates to the tile index.
		 */
		[[nodiscard]] int tile(const VectorI& coords) const;
		
		/**
		 * Converts a tile index to x,y coordinates.
		 */
		[[nodiscard]] VectorI tileCoords(const int& index) const;
		
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
		Unique<TileInfo[]> tileInfo = nullptr;

		/**
		 * The texture used by the tileset.
		 */
		Texture* albedoMap = nullptr;
		/**
		 * The filepath of the texture
		 */
		String albedoPath;

		/**
		 * The normal map texture used by the tileset
		 */
		Texture* normalMap = nullptr;
		/**
		 * The filepath of the normal map
		 */
		String normalPath;
		float normalMapStrength = 1.0f;

		/**
		 * The light mask used by the tileset
		 */
		Texture* lightMask;
		/**
		 * The filepath of the light mask
		 */
		String lightMaskPath;
		
		/**
		 * The filepath of the tileset (not the texture)
		 */
		String filePath;

		//TODO: Replace Tileset::id with instanceID
		unsigned int id = 0;
	};

	REGISTER_TYPE(Tileset);
}