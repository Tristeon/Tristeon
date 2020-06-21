#include "TileSet.h"


#include "Resources.h"
#include "Serialization/JsonSerializer.h"

namespace Tristeon
{
	REGISTER_TYPE_CPP(TileSet)
	
	TileSet::TileSet()
	{
		texture = Resources::assetLoad<Texture>(Texture::defaultPath);
		texturePath = Texture::defaultPath;
		
		this->tileInfo = std::make_unique<TileInfo[]>(cols * rows);
		for (size_t i = 0; i < cols * rows; i++)
			tileInfo[i] = TileInfo{};
	}

	json TileSet::serialize()
	{
		json j;
		j["typeID"] = TRISTEON_TYPENAME(TileSet);

		j["id"] = id;
		
		j["width"] = cols;
		j["height"] = rows;
		j["texturePath"] = texturePath;
		
		j["spacingLeft"] = spacingLeft;
		j["spacingRight"] = spacingRight;
		j["spacingTop"] = spacingTop;
		j["spacingBottom"] = spacingBottom;
		
		j["horizontalSpacing"] = horizontalSpacing;
		j["verticalSpacing"] = verticalSpacing;

		auto info = json::array_t();
		for (size_t i = 0; i < cols * rows; i++)
		{
			info.emplace_back(TileInfo{ false, 1, 0, 0 });
			if (tileInfo)
				info[i] = tileInfo[i];
		}
		j["tileInfo"] = info;
		return j;
	}

	void TileSet::deserialize(json j)
	{
		cols = j.value("width", 1);
		rows = j.value("height", 1);

		id = j.value("id", 0);

		texturePath = j.value("texturePath", "");
		texture = Resources::assetLoad<Texture>(texturePath);

		spacingLeft = j.value("spacingLeft", 0);
		spacingRight = j.value("spacingRight", 0);
		spacingTop = j.value("spacingTop", 0);
		spacingBottom = j.value("spacingBottom", 0);

		horizontalSpacing = j.value("horizontalSpacing", 0);
		verticalSpacing = j.value("verticalSpacing", 0);

		this->tileInfo = std::make_unique<TileInfo[]>(cols * rows);
		for (size_t i = 0; i < cols * rows; i++)
			tileInfo[i] = TileInfo{};
		
		if (j.contains("tileInfo") && j["tileInfo"].is_array())
		{
			for (size_t i = 0; i < cols * rows && i < j["tileInfo"].size(); i++)
			{
				tileInfo[i] = j["tileInfo"][i];
			}
		}
	}

	TileInfo TileSet::info(int const& index) const
	{
		if (index < 0 || index > cols * rows)
			return {};
		
		return tileInfo[index];
	}

	Vector2Int TileSet::tileSize() const
	{
		Vector2Int img = texture->size();
		img.x -= spacingLeft + spacingRight;
		img.y -= spacingTop + spacingBottom;

		img.x -= horizontalSpacing * (cols - 1);
		img.y -= verticalSpacing * (rows - 1);

		return { (int)img.x / (int)cols, (int)img.y / (int)rows };
	}

	Vector2 TileSet::tileSizeNormalized() const
	{
		Vector2Int const img = texture->size();
		Vector2Int const size = tileSize();
		return { size.x / (float)img.x, size.y / (float)img.y };
	}

	Vector2Int TileSet::tileMin(int const& x, int const& y) const
	{
		Vector2Int result;
		result.x += spacingLeft;
		result.y += spacingTop;

		Vector2Int const size = tileSize();
		
		result.x += x * (size.x + horizontalSpacing);
		result.y += y * (size.y + verticalSpacing);

		return result;
	}

	Vector2Int TileSet::tileMin(int const& index) const
	{
		return tileMin(tileCoords(index));
	}
	
	Vector2Int TileSet::tileMin(Vector2Int const& coords) const
	{
		return tileMin(coords.x, coords.y);
	}

	Vector2 TileSet::tileMinNormalized(int const& x, int const& y) const
	{
		Vector2Int const min = tileMin(x, y);
		Vector2Int const img = texture->size();
		return { min.x / (float)img.x, min.y / (float)img.y };
	}

	Vector2 TileSet::tileMinNormalized(int const& index) const
	{
		return tileMinNormalized(tileCoords(index));
	}

	Vector2 TileSet::tileMinNormalized(Vector2Int const& coords) const
	{
		return tileMinNormalized(coords.x, coords.y);
	}

	Vector2Int TileSet::tileMax(int const& x, int const& y) const
	{
		Vector2Int const min = tileMin(x, y);
		Vector2Int const size = tileSize();
		return min + size;
	}

	Vector2Int TileSet::tileMax(int const& index) const
	{
		return tileMax(tileCoords(index));
	}

	Vector2Int TileSet::tileMax(Vector2Int const& coords) const
	{
		return tileMax(coords.x, coords.y);
	}

	Vector2 TileSet::tileMaxNormalized(int const& x, int const& y) const
	{
		Vector2Int const max = tileMax(x, y);
		Vector2Int const img = texture->size();
		return { max.x / (float)img.x, max.y / (float)img.y };
	}

	Vector2 TileSet::tileMaxNormalized(int const& index) const
	{
		return tileMaxNormalized(tileCoords(index));
	}

	Vector2 TileSet::tileMaxNormalized(Vector2Int const& coords) const
	{
		return tileMaxNormalized(coords.x, coords.y);
	}

	int TileSet::tile(int const& x, int const& y) const
	{
		assert(x >= 0);
		assert(y >= 0);

		return y * cols + x;
	}

	int TileSet::tile(Vector2Int const& coords) const
	{
		return tile(coords.x, coords.y);
	}

	Vector2Int TileSet::tileCoords(int const& index) const
	{
		return { (int)index % (int)cols, (int)(index / (float)cols) };
	}

	TileInfo TileSet::info(int const& x, int const& y) const
	{
		return tileInfo[tile(x, y)];
	}

	TileInfo TileSet::info(Vector2Int const& coords) const
	{
		return tileInfo[tile(coords.x, coords.y)];
	}
}
