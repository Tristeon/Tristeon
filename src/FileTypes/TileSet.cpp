#include "TileSet.h"

namespace Tristeon
{
	REGISTER_TYPE_CPP(TileSet)
	
	TileSet::TileSet()
	{
		texture = std::make_unique<Texture>();
		//Empty
	}

	TileSet::TileSet(std::string const& imagePath,
		uint const& cols,
		uint const& rows,
		Tile tileInfo[],
		int const& tileRenderWidth, 
		int const& tileRenderHeight,

		uint const& spacingLeft,
		uint const& spacingRight,
		uint const& spacingTop,
		uint const& spacingBottom,
		uint const& horizontalSpacing,
		uint const& verticalSpacing) : cols(cols), rows(rows), spacingLeft(spacingLeft), spacingRight(spacingRight), spacingTop(spacingTop), spacingBottom(spacingBottom), horizontalSpacing(horizontalSpacing), verticalSpacing(verticalSpacing)
	{
		this->tileInfo = std::make_unique<Tile[]>(cols * rows);
		if (tileInfo != nullptr)
			memcpy(this->tileInfo.get(), tileInfo, int(cols * rows) * sizeof(Tile));

		texture = std::make_unique<Texture>(imagePath);

		this->tileRenderWidth = tileRenderWidth == -1 ? texture->width() / cols : tileRenderWidth;
		this->tileRenderHeight = tileRenderHeight == -1 ? texture->height() / rows : tileRenderHeight;
	}

	json TileSet::serialize()
	{
		json j;
		j["typeID"] = TRISTEON_TYPENAME(TileSet);

		j["width"] = cols;
		j["height"] = rows;
		j["texturePath"] = texture->getPath();
		
		j["tileRenderWidth"] = tileRenderWidth;
		j["tileRenderHeight"] = tileRenderHeight;

		j["spacingLeft"] = spacingLeft;
		j["spacingRight"] = spacingRight;
		j["spacingTop"] = spacingTop;
		j["spacingBottom"] = spacingBottom;
		
		j["horizontalSpacing"] = horizontalSpacing;
		j["verticalSpacing"] = verticalSpacing;
		//TODO: Serialize tileset tile info
		return j;
	}

	void TileSet::deserialize(json j)
	{
		cols = j["width"];
		rows = j["height"];

		std::string texturePath = j["texturePath"];
		texture = std::make_unique<Texture>(texturePath);

		tileRenderWidth = j["tileRenderWidth"];
		tileRenderHeight = j["tileRenderHeight"];

		spacingLeft = j["spacingLeft"];
		spacingRight = j["spacingRight"];
		spacingTop = j["spacingTop"];
		spacingBottom = j["spacingBottom"];
		
		horizontalSpacing = j["horizontalSpacing"];
		verticalSpacing = j["verticalSpacing"];

		//TODO: Deserialize tileset tile info
	}
}
