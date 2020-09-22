#pragma once
#include "Layer.h"
#include <Scenes/Tiles/Tileset.h>
#include <Scenes/Tiles/Tile.h>

#include <Rendering/Shader.h>

#include <box2d/b2_fixture.h>

namespace Tristeon
{
	/**
	 * The TileLayer renders tiles on a 2D grid using a given tileset.
	 *
	 * Tiles are rendered on the GPU using a full-screen shader, so performance cost should be minimal.
	 * Tile-specific behaviour (such as collisions, animations) can be defined through the Tile class in Tileset.
	 *
	 * This class can be used as an interface to change tile values within the level.
	 * Tiles are represented by a single index, read on the tileset texture from top left to bottom right.
	 * To convert said index to tileset x,y coordinates, you can use the tileset()'s interface.
	 */
	class TileLayer : public Layer
	{
		friend class SceneManager;
		friend class CollisionListener;

	public:
		TileLayer();
		virtual ~TileLayer();

		json serialize() override;
		void deserialize(json j) override;

#pragma region Core
		/**
		 * The number of columns in the TileLayer's tile-map.
		 */
		[[nodiscard]] unsigned int columns() const { return _columns; }
		/**
		 * The number of rows in the TileLayer's tile-map.
		 */
		[[nodiscard]] unsigned int rows() const { return _rows; }
#pragma endregion

#pragma region Tilesets
		/**
		 * Gets the tileset with the given ID.
		 */
		[[nodiscard]] Tileset* tileset(const unsigned int& id);

		/**
		 * Adds a tileset if it hasn't already been added.
		 */
		void addTileset(Tileset* tileset);
#pragma endregion

#pragma region Tile Modification
		/**
		 * Sets the tile at x, y to the given value.
		 *
		 * \exception out_of_range Throws if x is more than columns() or y is more than rows()
		 */
		void setTileByIndex(const unsigned int& column, const unsigned int& row, const Tile& value);

		/**
		 * Sets the tile at index.x, index.y to the given value.
		 *
		 * \exception invalid_argument Throws if index.x or index.y is less than 0
		 * \exception out_of_range Throws if index.x is more than columns() or index.y is more than rows()
		 */
		void setTileByIndex(const Vector2Int& index, const Tile& value);

		/**
		 * Sets the tile at the given world position to the given value.
		 *
		 * \exception invalid_argument Throws if position is under 0
		 * \exception out_of_range Throws if the position is outside of the tilemap
		 */
		void setTileByPosition(const Vector2& position, const Tile& value);

		/**
		 * Sets the tile at the given world position to the given value.
		 *
		 * \exception invalid_argument Throws if position is under 0
		 * \exception out_of_range Throws if the position is outside of the tilemap
		 */
		void setTileByPosition(const float& wx, const float& wy, const Tile& value);

		/**
		 * Gets the tile at x, y.
		 * 
		 * \exception out_of_range Throws if x is more than columns() or y is more than rows()
		 */
		[[nodiscard]] Tile tileByIndex(const unsigned int& column, const unsigned int& row) const;

		/**
		 * Gets the tile at index.x, index.y.
		 *
		 * \exception invalid_argument Throws if index.x or index.y is less than 0
		 * \exception out_of_range Throws if index.x is more than columns() or index.y is more than rows()
		 */
		[[nodiscard]] Tile tileByIndex(const Vector2Int& index) const;

		/**
		 * Gets the tile at the given world position.
		 *
		 * \exception invalid_argument Throws if position is under 0
		 * \exception out_of_range Throws if the position is outside of the tilemap
		 */
		[[nodiscard]] Tile tileByPosition(const Vector2& position) const;

		/**
		 * Gets the tile at the given world position.
		 *
		 * \exception invalid_argument Throws if position is under 0
		 * \exception out_of_range Throws if the position is outside of the tilemap
		 */
		[[nodiscard]] Tile tileByPosition(const float& wx, const float& wy) const;

		/**
		 * Returns true if column is less than columns() and row is less than rows().
		 */
		[[nodiscard]] bool checkBoundsByIndex(const unsigned int& column, const unsigned int& row) const;

		/**
		 * Returns true if the given index is within the bounds of the TileLayer
		 */
		[[nodiscard]] bool checkBoundsByIndex(const Vector2Int& index) const;

		/**
		 * Returns true if the world position is within the bounds of the TileLayer.
		 */
		[[nodiscard]] bool checkBoundsByPosition(const float& wx, const float& wy) const;
		
		/**
		 * Returns true if the world position is within the bounds of the TileLayer
		 */
		[[nodiscard]] bool checkBoundsByPosition(const Vector2& position) const;
#pragma endregion
		
	protected:
		void render(const Framebuffer& framebuffer) override;

		Unique<Tile[]> _tiles = nullptr;
		Vector<Tileset*> _tilesets;
		unsigned int _columns = 0, _rows = 0;

		/**
		 * If the TileLayer is dirty, it re-uploads the tile data to the GPU and rebuilds colliders where needed.
		 */
		bool _isDirty = false;

		/**
		 * Creates a texture buffer object.
		 *
		 * A texture buffer object is a 1D buffer, used to send big arrays of data to the GPU.
		 * This TBO is used to send the Level data to the tile shader.
		 */
		void createTBO();
		unsigned int _tbo = 0;
		unsigned int _tboTex = 0;
		
		/**
		 * Creates box colliders for all tiles that have the hasCollider flag set to true in their Tileset.
		 * Currently doesn't support other shapes.
		 */
		void createColliders();
		std::map<Vector2Int, b2Fixture*> _fixtures{};
	};

	REGISTER_TYPE(TileLayer);
}