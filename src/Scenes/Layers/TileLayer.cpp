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
		json j = Layer::serialize();
		j["typeID"] = TRISTEON_TYPENAME(TileLayer);
		j["columns"] = _columns;
		j["rows"] = _rows;

		json sets = json::array();
		for (auto* tileset : _tilesets)
			sets.push_back(tileset->filePath);
		j["tilesets"] = sets;

		//Tiles use a custom serialization method instead of the standard json library to speed things up
		String tilesSerialized;
		for (int i = 0; i < _columns * _rows; i++)
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
				Tileset* tileset = Resources::jsonLoad<Tileset>(i);
				if (tileset == nullptr)
					continue;
				tileset->filePath = i.get<String>();
				_tilesets.add(tileset);
			}
		}

		//json reading can be rather slow, so tiles are read manually through a simple comma-separated string.
		_tiles = std::make_unique<Tile[]>(_columns * _rows);
		const String tileString = j.value("tileData", "");
		const Vector<String> splitTileString = StringHelper::split(tileString, ',');

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

	void TileLayer::setTileByIndex(const Vector2Int& index, const Tile& value)
	{
		if (index.x < 0 || index.y < 0)
			throw std::invalid_argument("Coords can't be less than 0");
		
		setTileByIndex(index.x, index.y, value);
	}

	void TileLayer::setTileByPosition(const Vector2& position, const Tile& value)
	{
		setTileByPosition(position.x, position.y, value);
	}

	void TileLayer::setTileByPosition(const float& wx, const float& wy, const Tile& value)
	{
		Vector2Int const index = Grid::indexByPosition(wx, wy);
		setTileByIndex(index.x, index.y, value);
	}

	Tile TileLayer::tileByIndex(const unsigned int& column, const unsigned int& row) const
	{
		if (column * row > _columns * _rows || column > _columns || row > _rows)
			throw std::out_of_range("Out of range exception: coords exceed tile level");

		return _tiles[row * _columns + column];
	}

	Tile TileLayer::tileByIndex(const Vector2Int& index) const
	{
		if (index.x < 0 || index.y < 0)
			throw std::invalid_argument("Coords can't be less than 0");
		
		return tileByIndex(index.x, index.y);
	}

	Tile TileLayer::tileByPosition(const Vector2& position) const
	{
		return tileByPosition(position.x, position.y);
	}

	Tile TileLayer::tileByPosition(const float& wx, const float& wy) const
	{
		Vector2Int const index = Grid::indexByPosition(wx, wy);
		return tileByIndex(index);
	}

	Tileset* TileLayer::tileset(const unsigned int& id)
	{
		for (Tileset* tileset : _tilesets)
		{
			if (tileset->id == id)
				return tileset;
		}
		return nullptr;
	}

	void TileLayer::addTileset(Tileset* tileset)
	{
		if (tileset == nullptr)
			return;
		
		for (auto* t : _tilesets)
		{
			if (t->id == tileset->id)
				return;
		}

		_tilesets.add(tileset);
	}

	bool TileLayer::checkBoundsByIndex(const unsigned& column, const unsigned& row) const
	{
		if (column * row > _columns * _rows || column > _columns || row > _rows)
			return false;

		return true;
	}

	bool TileLayer::checkBoundsByIndex(const Vector2Int& index) const
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

	bool TileLayer::checkBoundsByPosition(const Vector2& position) const
	{
		return checkBoundsByPosition(position.x, position.y);
	}

	void TileLayer::render(const Framebuffer& framebuffer)
	{
		static Shader shader = Shader("Internal/Shaders/TileShader.vert", "Internal/Shaders/TileShader.frag");
		
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
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_BUFFER, _tboTex);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_R32I, _tbo);

		shader.setUniformValue("level.data", 1);

		shader.setUniformValue("level.columns", _columns);
		shader.setUniformValue("level.rows", _rows);

		shader.setUniformValue("level.tileRenderWidth", Grid::tileWidth());
		shader.setUniformValue("level.tileRenderHeight", Grid::tileHeight());

		glActiveTexture(GL_TEXTURE0);
		for (Tileset* set : _tilesets)
		{
			set->texture->bind();

			//Upload texture & tileset info
			shader.setUniformValue("tileset.texture", 0);

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
				bool const colliderExists = _fixtures.find(Vector2Int{ x, y }) != _fixtures.end();

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

				Tile const tile = _tiles[index];
				Tileset* set = tileset(tile.tilesetID);
				if (set == nullptr)
					continue;
				
				TileInfo const settings = set->info(tile.index);

				//No collider exists but the tile wants a collider
				if (settings.hasCollider && !colliderExists)
				{
					Vector2 position = Vector2(x * Grid::tileWidth(), y * Grid::tileHeight());
					Vector2 const meterSize = PhysicsWorld::pixelsToMeters({ (float)Grid::tileWidth() / 2.0f, (float)Grid::tileHeight() / 2.0f });
					Vector2 meterPosition = PhysicsWorld::pixelsToMeters(position);

					b2PolygonShape shape;
					shape.SetAsBox(meterSize.x, meterSize.y, meterPosition.convert<b2Vec2>(), 0);

					b2FixtureDef def;
					def.shape = &shape;
					def.density = settings.density;
					def.friction = settings.friction;
					def.restitution = settings.restitution;

					auto fixture = PhysicsWorld::instance()->_staticBody->CreateFixture(&def);
					fixture->SetUserData(this);
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