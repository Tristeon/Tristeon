#ifdef TRISTEON_EDITOR
#include "Stamp.h"
#include "Input/Mouse.h"

namespace TristeonEditor
{
	void Stamp::draw(Tristeon::TileLayer* layer, Tristeon::Vector2Int index)
	{
		layer->tile(index, Tristeon::Tile{ 16, 1242115487 });
	}
}
#endif