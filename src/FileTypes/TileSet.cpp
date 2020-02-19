#include "TileSet.h"

namespace Tristeon
{
	TileSet::TileSet()
	{
		
	}

	TileSet::TileSet(std::string const& imagePath, unsigned int const& width, unsigned int const& height, Tile tileInfo[]) : width(width), height(height)
	{
		this->tileInfo = std::unique_ptr<Tile[]>(new Tile[width * height]);
		memcpy(this->tileInfo.get(), tileInfo, width * height * sizeof(Tile));

		texture = std::make_unique<Texture>(imagePath);
	}
}
