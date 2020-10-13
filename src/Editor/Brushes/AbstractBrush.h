#pragma once
#ifdef TRISTEON_EDITOR
#include <Scenes/Layers/TileLayer.h>

namespace TristeonEditor
{
	class AbstractBrush
	{
	public:
		virtual void draw(Tristeon::TileLayer* layer, Tristeon::VectorI const& index) = 0;
		virtual void erase(Tristeon::TileLayer* layer, Tristeon::VectorI const& index) = 0;
	};
}
#endif