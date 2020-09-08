#pragma once
#include <json.h>

namespace Tristeon
{
	/**
	 * Describes a TileLayer tile.
	 * Instances of this struct can be obtained through the TileLayer.
	 */
	struct Tile
	{
		/**
		 * The tile's index on its TileSet.
		 */
		int index = -1;
		/**
		 * The tile's TileSet.
		 */
		int tileSetID = -1;
	};

	inline void to_json(nlohmann::json& j, const Tile& p) {
		j["index"] = p.index;
		j["tileSetID"] = p.tileSetID;
	}

	inline void from_json(const nlohmann::json& j, Tile& p) {
		p.index = j.value("index", -1);
		p.tileSetID = j.value("tileSetID", -1);
	}
}