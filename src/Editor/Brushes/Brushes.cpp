#ifdef TRISTEON_EDITOR
#include "Brushes.h"
#include "Stamp.h"

namespace TristeonEditor
{
	Tristeon::Unique<AbstractBrush> Brushes::_current = std::make_unique<Stamp>();
	Tristeon::Tile Brushes::_tile { -1, -1 };
	Tristeon::TileSet* Brushes::_tileSet = nullptr;
	
	void Brushes::selectTile(Tristeon::Tile const& tile, Tristeon::TileSet* tileset)
	{
		_tile = tile;
		_tileSet = tileset;
	}

	Tristeon::Tile Brushes::selectedTile()
	{
		return _tile;
	}

	Tristeon::TileSet* Brushes::selectedTileSet()
	{
		return _tileSet;
	}

	AbstractBrush* Brushes::current()
	{
		return _current.get();
	}
}
#endif