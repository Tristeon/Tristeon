#pragma once

namespace Tristeon
{
	/**
	 * Describes a TileLayer tile.
	 * Instances of this struct can be obtained through the TileLayer.
	 */
	struct Tile final
	{
		/**
		 * The tile's index on its Tileset.
		 */
		int index = -1;
		/**
		 * The tile's Tileset.
		 */
		int tilesetID = -1;
	};
}