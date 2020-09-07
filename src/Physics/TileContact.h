#pragma once
#include <Math/Vector2Int.h>
#include <Scenes/Layers/TileLayer.h>

namespace Tristeon
{
	/**
	 * Data structure containing information about a collision/contact between a collider and a tile.
	 */
	struct TileContact
	{
		/**
		 * The layer of the collided tile.
		 */
		TileLayer* layer = nullptr;
		/**
		 * The world index of the tile.
		 */
		Vector2Int tileIndex = {};
		/**
		 * The tile's tile-set and tile-set index.
		 */
		Tile tile = { -1, -1 };
		/**
		 * The tile's additional data/info.
		 */
		TileInfo tileInfo = {};
		/**
		 * The normal of the collision.
		 */
		Vector2 normal{};
	};
}