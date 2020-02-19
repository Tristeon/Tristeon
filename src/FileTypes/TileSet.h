#pragma once
#include <memory>
#include "Rendering/Texture.h"

namespace Tristeon
{
	struct Tile
	{
		
	};
	
	class TileSet
	{
	public:
		explicit TileSet();
		explicit TileSet(std::string const& imagePath, unsigned int const& width, unsigned int const& height, Tile tileInfo[]);
		
		unsigned int width = 0;
		unsigned int height = 0;
		std::unique_ptr<Tile[]> tileInfo = nullptr;
		std::unique_ptr<Texture> texture;
	};
}
