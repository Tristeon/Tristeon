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
		albedoMap = Resources::assetLoad<Texture>(Texture::defaultPath);
		albedoPath = Texture::defaultPath;
		
		this->tileInfo = std::make_unique<TileInfo[]>(cols * rows);
		for (size_t i = 0; i < cols * rows; i++)
			tileInfo[i] = TileInfo{};
	}

	json Tileset::serialize()
	{
		auto j = Serializable::serialize();
		j["typeID"] = Type<Tileset>::fullName();

		j["id"] = id;
		
		j["width"] = cols;
		j["height"] = rows;
		j["albedoPath"] = albedoPath;
		j["normalPath"] = normalPath;
		
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

		albedoPath = j.value("albedoPath", "");
		albedoMap = Resources::assetLoad<Texture>(albedoPath);

		normalPath = j.value("normalPath", "");
		normalMap = Resources::assetLoad<Texture>(normalPath);

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

	VectorI Tileset::tileSize() const
	{
		auto img = albedoMap->size();
		img.x -= spacingLeft + spacingRight;
		img.y -= spacingTop + spacingBottom;

		img.x -= horizontalSpacing * (cols - 1);
		img.y -= verticalSpacing * (rows - 1);

		return { img.x / (int)cols, img.y / (int)rows };
	}

	Vector Tileset::tileSizeNormalized() const
	{
		if (!albedoMap)
			return Vector{};
		auto const img = albedoMap->size();
		auto const size = tileSize();
		return { size.x / (float)img.x, size.y / (float)img.y };
	}

	VectorI Tileset::tileMin(const int& x, const int& y) const
	{
		VectorI result{ (int)spacingLeft, (int)spacingTop };

		auto const size = tileSize();
		
		result.x += x * (size.x + horizontalSpacing);
		result.y += y * (size.y + verticalSpacing);

		return result;
	}

	VectorI Tileset::tileMin(const int& index) const
	{
		return tileMin(tileCoords(index));
	}
	
	VectorI Tileset::tileMin(const VectorI& coords) const
	{
		return tileMin(coords.x, coords.y);
	}

	Vector Tileset::tileMinNormalized(const int& x, const int& y) const
	{
		auto const min = tileMin(x, y);
		auto const img = albedoMap->size();
		return { min.x / (float)img.x, min.y / (float)img.y };
	}

	Vector Tileset::tileMinNormalized(const int& index) const
	{
		return tileMinNormalized(tileCoords(index));
	}

	Vector Tileset::tileMinNormalized(const VectorI& coords) const
	{
		return tileMinNormalized(coords.x, coords.y);
	}

	VectorI Tileset::tileMax(const int& x, const int& y) const
	{
		auto const min = tileMin(x, y);
		auto const size = tileSize();
		return min + size;
	}

	VectorI Tileset::tileMax(const int& index) const
	{
		return tileMax(tileCoords(index));
	}

	VectorI Tileset::tileMax(const VectorI& coords) const
	{
		return tileMax(coords.x, coords.y);
	}

	Vector Tileset::tileMaxNormalized(const int& x, const int& y) const
	{
		auto const max = tileMax(x, y);
		auto const img = albedoMap->size();
		return { max.x / (float)img.x, max.y / (float)img.y };
	}

	Vector Tileset::tileMaxNormalized(const int& index) const
	{
		return tileMaxNormalized(tileCoords(index));
	}

	Vector Tileset::tileMaxNormalized(const VectorI& coords) const
	{
		return tileMaxNormalized(coords.x, coords.y);
	}

	int Tileset::tile(const int& x, const int& y) const
	{
		assert(x >= 0);
		assert(y >= 0);

		return y * cols + x;
	}

	int Tileset::tile(const VectorI& coords) const
	{
		return tile(coords.x, coords.y);
	}

	VectorI Tileset::tileCoords(const int& index) const
	{
		return { (int)index % (int)cols, (int)(index / (float)cols) };
	}

	TileInfo Tileset::info(const int& x, const int& y) const
	{
		return tileInfo[tile(x, y)];
	}

	TileInfo Tileset::info(const VectorI& coords) const
	{
		return tileInfo[tile(coords.x, coords.y)];
	}
}
