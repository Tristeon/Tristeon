#pragma once
#ifdef TRISTEON_EDITOR
#include "AbstractBrush.h"

namespace TristeonEditor
{
	class Brushes
	{
	public:
		static void selectTile(Tristeon::Tile const& tile);
		static Tristeon::Tile selectedTile();

		static AbstractBrush* current();
	private:
		static Tristeon::Unique<AbstractBrush> _current;
		static Tristeon::Tile _tile;
	};
}
#endif