#pragma once
#include "Layer.h"

#include <FileTypes/TileSet.h>

namespace Tristeon
{
	class TiledLayer : public Layer
	{
	public:
		virtual void render() override;
	private:
		TileSet* set = nullptr;
		int data[];
	};
}