#ifdef TRISTEON_EDITOR
#include "Stamp.h"
#include "Brushes.h"
#include "Input/Mouse.h"

namespace TristeonEditor
{
	void Stamp::draw(Tristeon::TileLayer* layer, Tristeon::Vector2Int const& index)
	{
		//if (Brushes::selectedTile().tileSetID != -1)
		//	layer->tile(index, Brushes::selectedTile());
		
		layer->setTileByIndex(index, Tristeon::Tile{ 13, 1242115487 });
	}

	void Stamp::erase(Tristeon::TileLayer* layer, Tristeon::Vector2Int const& index)
	{
		layer->setTileByIndex(index, Tristeon::Tile{});
	}
}
#endif