#pragma once
#ifdef TRISTEON_EDITOR
#include "AbstractBrush.h"

namespace TristeonEditor
{
	class Brushes
	{
	public:
		static void selectTile(Tristeon::Tile const& tile, const Tristeon::String& tilesetPath);
		static Tristeon::Tile selectedTile();
		static Tristeon::String selectedTilesetPath();

		static AbstractBrush* current();
	private:
		static Tristeon::Unique<AbstractBrush> _current;
		static Tristeon::Tile _tile;
		static Tristeon::String _tilesetPath;
	};
}
#endif