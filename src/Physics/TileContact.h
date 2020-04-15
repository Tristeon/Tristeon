#pragma once
#include <Math/Vector2Int.h>
#include <Scenes/Layers/TileLayer.h>

namespace Tristeon
{
	struct TileContact
	{
		TileLayer* layer = nullptr;
		Vector2Int tileIndex = {};
		int tile = -1;
		Tile tileInfo = {};
	};
}