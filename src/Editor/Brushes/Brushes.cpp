#ifdef TRISTEON_EDITOR
#include "Brushes.h"
#include "Stamp.h"

namespace TristeonEditor
{
	Tristeon::Unique<AbstractBrush> Brushes::_current = std::make_unique<Stamp>();
	Tristeon::Tile Brushes::_tile { -1, -1 };

	void Brushes::selectTile(Tristeon::Tile const& tile)
	{
		_tile = tile;
	}

	Tristeon::Tile Brushes::selectedTile()
	{
		return _tile;
	}

	AbstractBrush* Brushes::current()
	{
		return _current.get();
	}
}
#endif