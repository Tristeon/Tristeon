#ifdef TRISTEON_EDITOR
#include "Brushes.h"
#include "Stamp.h"

namespace TristeonEditor
{
	Tristeon::Unique<AbstractBrush> Brushes::_current = std::make_unique<Stamp>();
	Tristeon::Tile Brushes::_tile { -1, -1 };
	Tristeon::Tileset* Brushes::_tileset = nullptr;
	
	void Brushes::selectTile(Tristeon::Tile const& tile, Tristeon::Tileset* tileset)
	{
		_tile = tile;
		_tileset = tileset;
	}

	Tristeon::Tile Brushes::selectedTile()
	{
		return _tile;
	}

	Tristeon::Tileset* Brushes::selectedTileset()
	{
		return _tileset;
	}

	AbstractBrush* Brushes::current()
	{
		return _current.get();
	}
}
#endif