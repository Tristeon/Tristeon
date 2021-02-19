#include "TileLayer.h"
#include <glad/glad.h>

#include <Engine.h>
#include <Scenes/Tiles/Tileset.h>
#include <Input/Keyboard.h>
#include <Scenes/Scene.h>

#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

#include <AssetManagement/Resources.h>

#include "Rendering/Grid.h"
#include "Rendering/Texture.h"

namespace Tristeon
{
	TileLayer::TileLayer()
	{
		_tiles = Unique<Tile[]>(new Tile[1]{ Tile { -1, -1 } });
	}

	TileLayer::~TileLayer()
	{
		glDeleteBuffers(1, &_tbo);

		for (auto const& pair: _fixtures)
			PhysicsWorld::instance()->_staticBody->DestroyFixture(pair.second);
	}

	json TileLayer::serialize()
	{
		auto j = Layer::serialize();
		j["typeID"] = Type<TileLayer>::fullName();
		j["columns"] = _columns;
		j["rows"] = _rows;

		auto paths = json::array_t();
		for (auto path : _tilesetPaths)
			paths.push_back(path);
		j["tilesets"] = paths;

		//Tiles use a custom serialization method instead of the standard json library to speed things up
		String tilesSerialized;
		for (auto i = 0; i < _columns * _rows; i++)
		{
			//They're stored as a comma separated string
			tilesSerialized += std::to_string(_tiles[i].index) + "," + std::to_string(_tiles[i].tilesetID);
			if (i < _columns * _rows - 1)
				tilesSerialized += ",";
		}
		j["tileData"] = tilesSerialized;
		return j;
	}

	void TileLayer::deserialize(json j)
	{
		Layer::deserialize(j);
		
		_columns = j.value("columns", 1u);
		_rows = j.value("rows", 1u);

		_tilesets.clear();
		if (j.contains("tilesets"))
		{
			for (auto& i : j["tilesets"])
			{
				auto* tileset = Resources::jsonLoad<Tileset>(i);
				if (tileset == nullptr)
					continue;
				_tilesetPaths.add(i.get<String>());
				_tilesets.add(tileset);
			}
		}

		//json reading can be rather slow, so tiles are read manually through a simple comma-separated string.
		_tiles = std::make_unique<Tile[]>(_columns * _rows);
		const auto tileString = j.value("tileData", "");
		const auto splitTileString = StringHelper::split(tileString, ',');

		assert(splitTileString.size() % 2 == 0);

		for (unsigned int i = 0; i < splitTileString.size(); i += 2)
		{
			_tiles[i / 2] = Tile{ std::stoi(splitTileString[i]), std::stoi(splitTileString[i + 1]) };
		}
		
		createTBO();
		createColliders();
	}

	void TileLayer::setTileByIndex(const unsigned int& column, const unsigned int& row, const Tile& value)
	{
		if (column * row > _columns * _rows || column > _columns || row > _rows)
			throw std::out_of_range("Out of range exception: coords exceed tile level");

		_tiles[row * (int)_columns + column] = value;
		_isDirty = true;
	}

	void TileLayer::setTileByIndex(const VectorI& index, const Tile& value)
	{
		if (index.x < 0 || index.y < 0)
			throw std::invalid_argument("Coords can't be less than 0");
		
		setTileByIndex(index.x, index.y, value);
	}

	void TileLayer::setTileByPosition(const Vector& position, const Tile& value)
	{
		setTileByPosition(position.x, position.y, value);
	}

	void TileLayer::setTileByPosition(const float& wx, const float& wy, const Tile& value)
	{
		auto const index = Grid::indexByPosition(wx, wy);
		setTileByIndex(index.x, index.y, value);
	}

	Tile TileLayer::tileByIndex(const unsigned int& column, const unsigned int& row) const
	{
		if (column * row > _columns * _rows || column > _columns || row > _rows)
			throw std::out_of_range("Out of range exception: coords exceed tile level");

		return _tiles[row * _columns + column];
	}

	Tile TileLayer::tileByIndex(const VectorI& index) const
	{
		if (index.x < 0 || index.y < 0)
			throw std::invalid_argument("Coords can't be less than 0");
		
		return tileByIndex(index.x, index.y);
	}

	Tile TileLayer::tileByPosition(const Vector& position) const
	{
		return tileByPosition(position.x, position.y);
	}

	Tile TileLayer::tileByPosition(const float& wx, const float& wy) const
	{
		auto const index = Grid::indexByPosition(wx, wy);
		return tileByIndex(index);
	}

	Tileset* TileLayer::tileset(const unsigned int& id)
	{
		for (auto* tileset : _tilesets)
		{
			if (tileset->id == id)
				return tileset;
		}
		return nullptr;
	}

	void TileLayer::addTileset(const String& tilesetPath)
	{
		auto* tileset = Resources::jsonLoad<Tileset>(tilesetPath);
		if (tileset == nullptr)
			return;
		
		for (auto* t : _tilesets)
		{
			if (t->id == tileset->id)
				return;
		}

		_tilesetPaths.add(tilesetPath);
		_tilesets.add(tileset);
	}

	bool TileLayer::checkBoundsByIndex(const unsigned& column, const unsigned& row) const
	{
		if (column * row > _columns * _rows || column > _columns || row > _rows)
			return false;

		return true;
	}

	bool TileLayer::checkBoundsByIndex(const VectorI& index) const
	{
		if (index.x < 0 || index.y < 0)
			return false;

		return checkBoundsByIndex((unsigned int)index.x, (unsigned int)index.y);
	}

	bool TileLayer::checkBoundsByPosition(const float& wx, const float& wy) const
	{
		if (wx < 0 || wy < 0)
			return false;

		if (wx > _columns * Grid::tileWidth() || wy > _rows * Grid::tileHeight())
			return false;

		return true;
	}

	bool TileLayer::checkBoundsByPosition(const Vector& position) const
	{
		return checkBoundsByPosition(position.x, position.y);
	}

	void TileLayer::render(const Framebuffer& framebuffer)
	{
		static auto shader = Shader("Internal/Shaders/FullscreenTriangle.vert", "Internal/Shaders/TileShader.frag");
		
		if (_tbo == 0 || _tboTex == 0)
			return;

		if (!shader.ready())
			return;
		
		shader.bind();

		if (_isDirty)
		{
			createTBO();
			createColliders();
			_isDirty = false;
		}
		
		//Shader
		//Bind level data
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_BUFFER, _tboTex);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_R32I, _tbo);

		shader.setUniformValue("level.data", 0);

		shader.setUniformValue("level.columns", _columns);
		shader.setUniformValue("level.rows", _rows);

		shader.setUniformValue("level.tileRenderWidth", Grid::tileWidth());
		shader.setUniformValue("level.tileRenderHeight", Grid::tileHeight());

		shader.setUniformValue("tileset.albedoMap", 1);
		shader.setUniformValue("tileset.normalMap", 2);
		shader.setUniformValue("tileset.lightMask", 3);
		
		for (auto* set : _tilesets)
		{
			if (!set->albedoMap)
				continue;
			
			glActiveTexture(GL_TEXTURE1);
			set->albedoMap->bind();

			shader.setUniformValue("tileset.normalMapEnabled", (bool)set->normalMap);
			if (set->normalMap)
			{
				glActiveTexture(GL_TEXTURE2);
				set->normalMap->bind();
			}

			shader.setUniformValue("tileset.lightMaskEnabled", (bool)set->lightMask);
			if (set->lightMask)
			{
				glActiveTexture(GL_TEXTURE3);
				set->lightMask->bind();
			}

			//Upload texture & tileset info
			shader.setUniformValue("tileset.cols", set->cols);
			shader.setUniformValue("tileset.rows", set->rows);

			shader.setUniformValue("tileset.id", set->id);

			//Spacing
			shader.setUniformValue("tileset.spacingLeft", set->spacingLeft);
			shader.setUniformValue("tileset.spacingRight", set->spacingRight);
			shader.setUniformValue("tileset.spacingTop", set->spacingTop);
			shader.setUniformValue("tileset.spacingBottom", set->spacingBottom);
			shader.setUniformValue("tileset.horizontalSpacing", set->horizontalSpacing);
			shader.setUniformValue("tileset.verticalSpacing", set->verticalSpacing);

			//Draw
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}

	void TileLayer::createTBO()
	{
		//TODO: Updating TBO could be optimized by updating data as opposed to recreating the TBO
		//TODO: Updating TBO could be optimized by using glBufferSubData()
		if (_tbo != 0)
			glDeleteBuffers(1, &_tbo);
		if (_tboTex != 0)
			glDeleteTextures(1, &_tboTex);
		_tbo = 0;
		_tboTex = 0;

		glGenBuffers(1, &_tbo);
		glBindBuffer(GL_TEXTURE_BUFFER, _tbo);
		glBufferData(GL_TEXTURE_BUFFER, sizeof(Tile) * _columns * _rows, _tiles.get(), GL_STATIC_DRAW);

		glGenTextures(1, &_tboTex);
		glBindBuffer(GL_TEXTURE_BUFFER, 0);
	}

	void TileLayer::createColliders()
	{
		for (auto x = 0; x < _columns; x++)
		{
			for (auto y = 0; y < _rows; y++)
			{
				auto const colliderExists = _fixtures.find(VectorI{ x, y }) != _fixtures.end();

				int const index = y * _columns + x;
				if (_tiles[index].index == -1)
				{
					if (colliderExists)
					{
						PhysicsWorld::instance()->_staticBody->DestroyFixture(_fixtures[{ x, y }]);
						_fixtures.erase(_fixtures.find({ x, y }));
					}
					continue;
				}

				auto const tile = _tiles[index];
				auto* set = tileset(tile.tilesetID);
				if (set == nullptr)
					continue;

				auto const settings = set->info(tile.index);

				//No collider exists but the tile wants a collider
				if (settings.hasCollider && !colliderExists)
				{
					auto position = Vector(x * Grid::tileWidth(), y * Grid::tileHeight());
					auto const meterSize = PhysicsWorld::pixelsToMeters({ (float)Grid::tileWidth() / 2.0f, (float)Grid::tileHeight() / 2.0f });
					const auto meterPosition = PhysicsWorld::pixelsToMeters(position);

					b2PolygonShape shape;
					shape.SetAsBox(meterSize.x, meterSize.y, b2Vec2(meterPosition.x, meterPosition.y), 0);

					b2FixtureDef def;
					def.shape = &shape;
					def.density = settings.density;
					def.friction = settings.friction;
					def.restitution = settings.restitution;

					auto* fixture = PhysicsWorld::instance()->_staticBody->CreateFixture(&def);
					fixture->GetUserData().pointer = (uintptr_t)this;
					_fixtures[{x, y}] = fixture;
				}
				//A collider exists but the tile doesn't want a collider
				else if (!settings.hasCollider && colliderExists)
				{
					PhysicsWorld::instance()->_staticBody->DestroyFixture(_fixtures[{ x, y }]);
					_fixtures.erase(_fixtures.find({ x, y }));
				}
			}
		}
	}
}