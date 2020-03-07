#include "TileSet.h"

namespace Tristeon
{
	REGISTER_TYPE_CPP(TileSet)
	
	TileSet::TileSet()
	{
		texture = std::make_unique<Texture>();
		//Empty
	}

	TileSet::TileSet(std::string const& imagePath, unsigned int const& width, unsigned int const& height, Tile tileInfo[]) : width(width), height(height)
	{
		this->tileInfo = std::unique_ptr<Tile[]>(new Tile[width * height]);
		if (tileInfo != nullptr)
			memcpy(this->tileInfo.get(), tileInfo, width * height * sizeof(Tile));

		texture = std::make_unique<Texture>(imagePath);
	}

	json TileSet::serialize()
	{
		json j;
		j["typeID"] = TRISTEON_TYPENAME(TileSet);
		j["width"] = width;
		j["height"] = height;
		j["texturePath"] = texture->getPath();
		//TODO: Serialize tileset tile info
		return j;
	}

	void TileSet::deserialize(json j)
	{
		width = j["width"];
		height = j["height"];

		std::string texturePath = j["texturePath"];
		texture = std::make_unique<Texture>(texturePath);

		//TODO: Deserialize tileset tile info
	}
}
