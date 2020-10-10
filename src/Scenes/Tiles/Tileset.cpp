#include "Tileset.h"
#include "AssetManagement/Resources.h"
#include <Rendering/Texture.h>

namespace Tristeon
{
	void to_json(json& j, const TileInfo& p)
	{
		j["hasCollider"] = p.hasCollider;
		j["density"] = p.density;
		j["friction"] = p.friction;
		j["restitution"] = p.restitution;
	}

	void from_json(const json& j, TileInfo& p)
	{
		p.hasCollider = j.value("hasCollider", false);
		p.density = j.value("density", 1.0f);
		p.friction = j.value("friction", 0.0f);
		p.restitution = j.value("restitution", 0.0f);
	}

	Tileset::Tileset()
	{
		texture = Resources::assetLoad<Texture>(Texture::defaultPath);
		texturePath = Texture::defaultPath;
		
		this->tileInfo = std::make_unique<TileInfo[]>(cols * rows);
		for (size_t i = 0; i < cols * rows; i++)
			tileInfo[i] = TileInfo{};
	}

	json Tileset::serialize()
	{
		json j = Serializable::serialize();
		j["typeID"] = Type<Tileset>::fullName();

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

	void Tileset::deserialize(json j)
	{
		Serializable::deserialize(j);
		
		cols = j.value("width", 1u);
		rows = j.value("height", 1u);

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

	TileInfo Tileset::info(const int& index) const
	{
		if (index < 0 || index > cols * rows)
			return {};
		
		return tileInfo[index];
	}

	Vector2Int Tileset::tileSize() const
	{
		Vector2Int img = texture->size();
		img.x -= spacingLeft + spacingRight;
		img.y -= spacingTop + spacingBottom;

		img.x -= horizontalSpacing * (cols - 1);
		img.y -= verticalSpacing * (rows - 1);

		return { img.x / (int)cols, img.y / (int)rows };
	}

	Vector2 Tileset::tileSizeNormalized() const
	{
		Vector2Int const img = texture->size();
		Vector2Int const size = tileSize();
		return { size.x / (float)img.x, size.y / (float)img.y };
	}

	Vector2Int Tileset::tileMin(const int& x, const int& y) const
	{
		Vector2Int result{ (int)spacingLeft, (int)spacingTop };

		Vector2Int const size = tileSize();
		
		result.x += x * (size.x + horizontalSpacing);
		result.y += y * (size.y + verticalSpacing);

		return result;
	}

	Vector2Int Tileset::tileMin(const int& index) const
	{
		return tileMin(tileCoords(index));
	}
	
	Vector2Int Tileset::tileMin(const Vector2Int& coords) const
	{
		return tileMin(coords.x, coords.y);
	}

	Vector2 Tileset::tileMinNormalized(const int& x, const int& y) const
	{
		Vector2Int const min = tileMin(x, y);
		Vector2Int const img = texture->size();
		return { min.x / (float)img.x, min.y / (float)img.y };
	}

	Vector2 Tileset::tileMinNormalized(const int& index) const
	{
		return tileMinNormalized(tileCoords(index));
	}

	Vector2 Tileset::tileMinNormalized(const Vector2Int& coords) const
	{
		return tileMinNormalized(coords.x, coords.y);
	}

	Vector2Int Tileset::tileMax(const int& x, const int& y) const
	{
		Vector2Int const min = tileMin(x, y);
		Vector2Int const size = tileSize();
		return min + size;
	}

	Vector2Int Tileset::tileMax(const int& index) const
	{
		return tileMax(tileCoords(index));
	}

	Vector2Int Tileset::tileMax(const Vector2Int& coords) const
	{
		return tileMax(coords.x, coords.y);
	}

	Vector2 Tileset::tileMaxNormalized(const int& x, const int& y) const
	{
		Vector2Int const max = tileMax(x, y);
		Vector2Int const img = texture->size();
		return { max.x / (float)img.x, max.y / (float)img.y };
	}

	Vector2 Tileset::tileMaxNormalized(const int& index) const
	{
		return tileMaxNormalized(tileCoords(index));
	}

	Vector2 Tileset::tileMaxNormalized(const Vector2Int& coords) const
	{
		return tileMaxNormalized(coords.x, coords.y);
	}

	int Tileset::tile(const int& x, const int& y) const
	{
		assert(x >= 0);
		assert(y >= 0);

		return y * cols + x;
	}

	int Tileset::tile(const Vector2Int& coords) const
	{
		return tile(coords.x, coords.y);
	}

	Vector2Int Tileset::tileCoords(const int& index) const
	{
		return { (int)index % (int)cols, (int)(index / (float)cols) };
	}

	TileInfo Tileset::info(const int& x, const int& y) const
	{
		return tileInfo[tile(x, y)];
	}

	TileInfo Tileset::info(const Vector2Int& coords) const
	{
		return tileInfo[tile(coords.x, coords.y)];
	}
}
