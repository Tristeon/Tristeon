#pragma once
#ifdef TRISTEON_EDITOR
#include "AbstractBrush.h"

namespace TristeonEditor
{
	class Stamp : public AbstractBrush
	{
	public:
		void draw(Tristeon::TileLayer* layer, Tristeon::Vector2Int const& index) override;
		void erase(Tristeon::TileLayer* layer, Tristeon::Vector2Int const& index) override;
	};
}
#endif