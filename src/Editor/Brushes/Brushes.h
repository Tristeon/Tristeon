#pragma once
#ifdef TRISTEON_EDITOR
#include "AbstractBrush.h"

namespace TristeonEditor
{
	class Brushes
	{
	public:
		static void selectTile(Tristeon::Tile const& tile, Tristeon::TileSet* tileset);
		static Tristeon::Tile selectedTile();
		static Tristeon::TileSet* selectedTileSet();

		static AbstractBrush* current();
	private:
		static Tristeon::Unique<AbstractBrush> _current;
		static Tristeon::Tile _tile;
		static Tristeon::TileSet* _tileSet;
	};
}
#endif