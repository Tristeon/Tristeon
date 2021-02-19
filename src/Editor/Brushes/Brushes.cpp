#ifdef TRISTEON_EDITOR
#include "Brushes.h"
#include "Stamp.h"

namespace TristeonEditor
{
	Tristeon::Unique<AbstractBrush> Brushes::_current = std::make_unique<Stamp>();
	Tristeon::Tile Brushes::_tile { -1, -1 };
	Tristeon::String Brushes::_tilesetPath;
	
	void Brushes::selectTile(Tristeon::Tile const& tile, const Tristeon::String& tilesetPath)
	{
		_tile = tile;
		_tilesetPath = tilesetPath;
	}

	Tristeon::Tile Brushes::selectedTile()
	{
		return _tile;
	}

	Tristeon::String Brushes::selectedTilesetPath()
	{
		return _tilesetPath;
	}

	AbstractBrush* Brushes::current()
	{
		return _current.get();
	}
}
#endif