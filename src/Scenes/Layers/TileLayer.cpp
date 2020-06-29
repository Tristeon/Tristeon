#include "TileLayer.h"
#include <GL/glew.h>

#include <Engine.h>
#include <FileTypes/TileSet.h>
#include <Input/Keyboard.h>
#include <Scenes/Scene.h>

#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

#include <Resources.h>

#include "Rendering/Grid.h"

namespace Tristeon
{
	REGISTER_LAYER_CPP(TileLayer);
	REGISTER_TYPE_CPP(TileLayer);

	TileLayer::TileLayer()
	{
		shader = std::make_unique<Shader>("Internal/Shaders/TileShader.vert", "Internal/Shaders/TileShader.frag");
		tiles = Unique<Tile[]>(new Tile[1]{ Tile { -1, -1 } });
	}

	TileLayer::~TileLayer()
	{
		glDeleteBuffers(1, &tbo);

		for (auto const& pair: fixtures)
			PhysicsWorld::instance()->staticBody->DestroyFixture(pair.second);
	}

	json TileLayer::serialize()
	{
		json j = Layer::serialize();
		j["typeID"] = TRISTEON_TYPENAME(TileLayer);
		j["width"] = _width;
		j["height"] = _height;

		json sets = json::array();
		for (auto* tileset : tilesets)
			sets.push_back(tileset->filePath);
		j["tileSets"] = sets;

		json t = json::array();
		for (unsigned int i = 0; i < _width * _height; i++)
			t.push_back(tiles[i]);
		j["tiles"] = t;
		return j;
	}

	void TileLayer::deserialize(json j)
	{
		Layer::deserialize(j);
		
		_width = j.value("width", 1);
		_height = j.value("height", 1);

		tilesets.clear();
		if (j.contains("tileSets"))
		{
			for (auto& i : j["tileSets"])
			{
				TileSet* tileset = Resources::jsonLoad<TileSet>(i);
				if (tileset == nullptr)
					continue;
				tileset->filePath = i.get<std::string>();
				tilesets.push_back(tileset);
			}
		}
		
		tiles = std::make_unique<Tile[]>(_width * _height);
		if (j.contains("tiles"))
		{
			for (unsigned int i = 0; i < _width * _height && i < j["tiles"].size(); i++)
				tiles[i] = j["tiles"].at(i);
		}
		createTBO();
		createColliders();
	}

	void TileLayer::setTileByIndex(int const& ix, int const& iy, Tile const& value)
	{
		if (ix < 0 || iy < 0)
			throw std::invalid_argument("Coords can't be less than 0");

		if (ix * iy > _width * _height || ix > _width || iy > _height)
			throw std::out_of_range("Out of range exception: coords exceed tile level");

		tiles[iy * (int)_width + ix] = value;
		isDirty = true;
	}

	void TileLayer::setTileByIndex(Vector2Int const& index, Tile const& value)
	{
		setTileByIndex(index.x, index.y, value);
	}

	void TileLayer::setTileByPosition(Vector2 const& position, Tile const& value)
	{
		setTileByPosition(position.x, position.y, value);
	}

	void TileLayer::setTileByPosition(float const& wx, float const& wy, Tile const& value)
	{
		Vector2Int const index = Grid::indexByPosition(wx, wy);
		setTileByIndex(index.x, index.y, value);
	}

	Tile TileLayer::tileByIndex(int const& ix, int const& iy) const
	{
		if (ix < 0 || iy < 0)
			throw std::invalid_argument("Coords can't be less than 0");

		if (ix * iy > _width * _height || ix > _width || iy > _height)
			throw std::out_of_range("Out of range exception: coords exceed tile level");

		return tiles[iy * (int)_width + ix];
	}

	Tile TileLayer::tileByIndex(Vector2Int const& index) const
	{
		return tileByIndex(index.x, index.y);
	}

	Tile TileLayer::tileByPosition(Vector2 const& position) const
	{
		return tileByPosition(position.x, position.y);
	}

	Tile TileLayer::tileByPosition(float const& wx, float const& wy) const
	{
		Vector2Int const index = Grid::indexByPosition(wx, wy);
		return tileByIndex(index);
	}

	TileSet* TileLayer::tileset(int const& id)
	{
		for (TileSet* tileset : tilesets)
		{
			if (tileset->id == id)
				return tileset;
		}
		return nullptr;
	}

	void TileLayer::addTileSet(TileSet* tileset)
	{
		if (tileset == nullptr)
			return;
		
		for (auto* t : tilesets)
		{
			if (t->id == tileset->id)
				return;
		}

		tilesets.add(tileset);
	}

	bool TileLayer::checkBoundsByIndex(Vector2Int const& index) const
	{
		if (index.x < 0 || index.y < 0)
			return false;
		
		if (index.x * index.y > _width * _height || index.x > _width || index.y > _height)
			return false;

		return true;
	}

	bool TileLayer::checkBoundsByPosition(Vector2 const& position) const
	{
		if (position.x < 0 || position.y < 0)
			return false;

		if (position.x > _width * Grid::tileWidth() || position.y > _height * Grid::tileHeight())
			return false;

		return true;
	}

	void TileLayer::render(Renderer * renderer, Scene * scene)
	{
		if (tbo == 0 || tbo_tex == 0)
			return;

		if (!shader->isReady())
			return;
		
		shader->bind();

		if (isDirty)
		{
			createTBO();
			createColliders();
			isDirty = false;
		}
		
		//Shader
		//Bind level data
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_BUFFER, tbo_tex);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_R32I, tbo);

		shader->setUniformValue("level.data", 1);

		shader->setUniformValue("level.width", _width);
		shader->setUniformValue("level.height", _height);

		shader->setUniformValue("level.tileRenderWidth", Grid::tileWidth());
		shader->setUniformValue("level.tileRenderHeight", Grid::tileHeight());

		glActiveTexture(GL_TEXTURE0);
		for (TileSet* tileSet : tilesets)
		{
			tileSet->texture->bind();

			//Upload texture & tileset info
			shader->setUniformValue("tileSet.texture", 0);

			shader->setUniformValue("tileSet.cols", tileSet->cols);
			shader->setUniformValue("tileSet.rows", tileSet->rows);

			shader->setUniformValue("tileSet.id", tileSet->id);

			//Spacing
			shader->setUniformValue("tileSet.spacingLeft", tileSet->spacingLeft);
			shader->setUniformValue("tileSet.spacingRight", tileSet->spacingRight);
			shader->setUniformValue("tileSet.spacingTop", tileSet->spacingTop);
			shader->setUniformValue("tileSet.spacingBottom", tileSet->spacingBottom);
			shader->setUniformValue("tileSet.horizontalSpacing", tileSet->horizontalSpacing);
			shader->setUniformValue("tileSet.verticalSpacing", tileSet->verticalSpacing);

			//Draw
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}

	void TileLayer::createTBO()
	{
		//TODO: Updating TBO could be optimized by updating data as opposed to recreating the TBO
		//TODO: Updating TBO could be optimized by using glBufferSubData()
		if (tbo != 0)
			glDeleteBuffers(1, &tbo);
		if (tbo_tex != 0)
			glDeleteTextures(1, &tbo_tex);
		tbo = 0;
		tbo_tex = 0;

		glGenBuffers(1, &tbo);
		glBindBuffer(GL_TEXTURE_BUFFER, tbo);
		glBufferData(GL_TEXTURE_BUFFER, sizeof(Tile) * _width * _height, tiles.get(), GL_STATIC_DRAW);

		glGenTextures(1, &tbo_tex);
		glBindBuffer(GL_TEXTURE_BUFFER, 0);
	}

	void TileLayer::createColliders()
	{
		for (int x = 0; x < _width; x++)
		{
			for (int y = 0; y < _height; y++)
			{
				bool const colliderExists = fixtures.find(Vector2Int{ x, y }) != fixtures.end();

				int const index = y * _width + x;
				if (tiles[index].index == -1)
				{
					if (colliderExists)
					{
						PhysicsWorld::instance()->staticBody->DestroyFixture(fixtures[{ x, y }]);
						fixtures.erase(fixtures.find({ x, y }));
					}
					continue;
				}

				Tile const tile = tiles[index];
				TileSet* set = tileset(tile.tileSetID);
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

					auto fixture = PhysicsWorld::instance()->staticBody->CreateFixture(&def);
					fixture->SetUserData(this);
					fixtures[{x, y}] = fixture;
				}
				//A collider exists but the tile doesn't want a collider
				else if (!settings.hasCollider && colliderExists)
				{
					PhysicsWorld::instance()->staticBody->DestroyFixture(fixtures[{ x, y }]);
					fixtures.erase(fixtures.find({ x, y }));
				}
			}
		}
	}
}
