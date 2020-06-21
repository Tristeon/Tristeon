#pragma once
#include <json.h>

namespace Tristeon
{
	struct Tile
	{
		int index = -1;
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
