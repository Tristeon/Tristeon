#pragma once
#include <memory>
#include <Rendering/Texture.h>
#include <Serialization/Serializable.h>
#include <Serialization/TypeRegister.h>
#include <Math/Vector2Int.h>

namespace Tristeon
{
	struct TileInfo
	{
		bool hasCollider = false;
		float density = 1.0f;
		float friction = 0.0f;
		float restitution = 0.0f;
	};

	inline void to_json(json& j, const TileInfo& p) {
		j["hasCollider"] = p.hasCollider;
		j["density"] = p.density;
		j["friction"] = p.friction;
		j["restitution"] = p.restitution;
	}

	inline void from_json(const json& j, TileInfo& p) {
		p.hasCollider = j["hasCollider"];
		p.density = j["density"];
		p.friction = j["friction"];
		p.restitution = j["restitution"];
	}

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
		 * Creates a tileset from a given tileset file.
		 */
		explicit TileSet(String const& path);

		/**
		 * Creates a tileset with the given values, this is shorthand for simply assigning the values after creation.
		 */
		explicit TileSet(String const& imagePath, 
			uint const& cols, 
			uint const& rows, 
			TileInfo tileInfo[], 

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
		 * Gets the Tile data struct of the tile at {x, y}
		 */
		TileInfo info(int const& x, int const& y) const;

		/**
		 * Gets the Tile data struct of the tile at {x, y}
		 */
		TileInfo info(Vector2Int const& coords) const;
		
		/**
		 * Gets the Tile data struct of the tile at the given index.
		 */
		TileInfo info(int const& index) const;

		/**
		 * Gets the size of a tile in pixels.
		 */
		Vector2Int tileSize() const;

		/**
		 * Gets the size of the tiles in 0..1 range
		 */
		Vector2 tileSizeNormalized() const;

		/**
		 * Gets the top-left corner of the tile on the image in pixels.
		 */
		Vector2Int tileMin(int const& index) const;
		/**
		 * Gets the top-left corner of the tile on the image in pixels.
		 */
		Vector2Int tileMin(int const& x, int const& y) const;
		/**
		 * Gets the top-left corner of the tile on the image in pixels.
		 */
		Vector2Int tileMin(Vector2Int const& coords) const;

		/**
		 * Gets the bottom-left corner of the tile on the image in normalized 0..1 range.
		 */
		Vector2 tileMinNormalized(int const& x, int const& y) const;
		/**
		 * Gets the bottom-left corner of the tile on the image in normalized 0..1 range.
		 */
		Vector2 tileMinNormalized(int const& index) const;
		/**
		 * Gets the bottom-left corner of the tile on the image in normalized 0..1 range.
		 */
		Vector2 tileMinNormalized(Vector2Int const& coords) const;
		
		/**
		 * Gets the bottom-right corner of the tile on the image in pixels.
		 */
		Vector2Int tileMax(int const& index) const;
		/**
		 * Gets the bottom-right corner of the tile on the image in pixels.
		 */
		Vector2Int tileMax(int const& x, int const& y) const;
		/**
		 * Gets the bottom-right corner of the tile on the image in pixels.
		 */
		Vector2Int tileMax(Vector2Int const& coords) const;

		/**
		 * Gets the bottom-right corner of the tile on the image in normalized 0..1 range.
		 */
		Vector2 tileMaxNormalized(int const& x, int const& y) const;
		/**
		 * Gets the bottom-right corner of the tile on the image in normalized 0..1 range.
		 */
		Vector2 tileMaxNormalized(int const& index) const;
		/**
		 * Gets the bottom-right corner of the tile on the image in normalized 0..1 range.
		 */
		Vector2 tileMaxNormalized(Vector2Int const& coords) const;
		
		/**
		 * Converts x,y coordinates to the tile index.
		 */
		int tile(int const& x, int const& y) const;

		/**
		 * Converts x,y coordinates to the tile index.
		 */
		int tile(Vector2Int const& coords) const;
		
		/**
		 * Converts a tile index to x,y coordinates.
		 */
		Vector2Int tileCoords(int const& index) const;
		
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
		Unique<Texture> texture = nullptr;

		/**
		 * The filepath of the tileset (not the texture)
		 */
		String filePath = "";

		int id = 0;
	};
}
