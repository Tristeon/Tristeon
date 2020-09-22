#ifdef TRISTEON_EDITOR
#include "Stamp.h"
#include "Brushes.h"
#include "Input/Mouse.h"

namespace TristeonEditor
{
	void Stamp::draw(Tristeon::TileLayer* layer, Tristeon::Vector2Int const& index)
	{
		if (Brushes::selectedTile().tilesetID != -1)
			layer->setTileByIndex(index, Brushes::selectedTile());
	}

	void Stamp::erase(Tristeon::TileLayer* layer, Tristeon::Vector2Int const& index)
	{
		layer->setTileByIndex(index, Tristeon::Tile{});
	}
}
#endif