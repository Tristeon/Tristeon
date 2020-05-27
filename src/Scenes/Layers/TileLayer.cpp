#include "TileLayer.h"

#include <Engine.h>
#include <FileTypes/TileSet.h>
#include <Input/Keyboard.h>
#include <Scenes/Scene.h>

#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>

#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

#include <Resources.h>

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
		QOpenGLFunctions* f = GameView::context()->functions();
		f->glDeleteBuffers(1, &tbo);

		for (auto const& pair: fixtures)
			PhysicsWorld::instance()->staticBody->DestroyFixture(pair.second);
	}

	json TileLayer::serialize()
	{
		json j = Layer::serialize();
		j["typeID"] = TRISTEON_TYPENAME(TileLayer);
		j["width"] = _width;
		j["height"] = _height;

		j["tileRenderWidth"] = _tileRenderWidth;
		j["tileRenderHeight"] = _tileRenderHeight;

		json sets = json::array();
		for (auto tileset : tilesets)
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
		
		_width = j["width"];
		_height = j["height"];

		_tileRenderWidth = j["tileRenderWidth"];
		_tileRenderHeight = j["tileRenderHeight"];

		tilesets.clear();
		if (j.contains("tileSets"))
		{
			for (size_t i = 0; i < j["tileSets"].size(); i++)
			{
				TileSet* tileset = Resources::jsonLoad<TileSet>(j["tileSets"][i]);
				tileset->filePath = j["tileSets"][i].get<std::string>();
				tilesets.push_back(tileset);
			}
		}
		
		tiles = std::make_unique<Tile[]>(_width * _height);
		if (j.contains("tiles"))
		{
			for (unsigned int i = 0; i < _width * _height && i < j["tiles"].size(); i++)
				tiles[i] = j["tiles"][i];
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
		Vector2Int const index = indexByPosition(wx, wy);
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
		Vector2Int const index = indexByPosition(wx, wy);
		return tileByIndex(index);
	}

	Vector2Int TileLayer::indexByPosition(Vector2 const& position) const
	{
		return indexByPosition(position.x, position.y);
	}

	Vector2Int TileLayer::indexByPosition(float const& wx, float const& wy) const
	{
		return (Vector2Int)Vector2::floor({ (wx + _tileRenderWidth / 2.0f) / _tileRenderWidth, (wy + _tileRenderHeight / 2.0f) / _tileRenderHeight });
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

		if (position.x > _width * _tileRenderWidth || position.y > _height * _tileRenderHeight)
			return false;

		return true;
	}

	void TileLayer::render(Renderer * renderer, Scene * scene)
	{
		if (tbo == 0 || tbo_tex == 0)
			return;

		if (Keyboard::pressed(Keyboard::R))
			shader->reload();

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
		QOpenGLContext* context = GameView::context();
		QOpenGLFunctions* f = context->functions();
		QOpenGLShaderProgram* program = shader->getShaderProgram();

		//Bind level data
		f->glActiveTexture(GL_TEXTURE1);
		f->glBindTexture(GL_TEXTURE_BUFFER, tbo_tex);
		context->extraFunctions()->glTexBuffer(GL_TEXTURE_BUFFER, GL_R32I, tbo);
		program->setUniformValue("level.data", 1);

		program->setUniformValue("level.width", _width);
		program->setUniformValue("level.height", _height);

		program->setUniformValue("level.tileRenderWidth", _tileRenderWidth);
		program->setUniformValue("level.tileRenderHeight", _tileRenderHeight);

		f->glActiveTexture(GL_TEXTURE0);
		for (TileSet* tileSet : tilesets)
		{
			tileSet->texture->bind();

			//Upload texture & tileset info
			program->setUniformValue("tileSet.texture", 0);

			program->setUniformValue("tileSet.cols", tileSet->cols);
			program->setUniformValue("tileSet.rows", tileSet->rows);

			program->setUniformValue("tileSet.id", tileSet->id);

			//Spacing
			program->setUniformValue("tileSet.spacingLeft", tileSet->spacingLeft);
			program->setUniformValue("tileSet.spacingRight", tileSet->spacingRight);
			program->setUniformValue("tileSet.spacingTop", tileSet->spacingTop);
			program->setUniformValue("tileSet.spacingBottom", tileSet->spacingBottom);
			program->setUniformValue("tileSet.horizontalSpacing", tileSet->horizontalSpacing);
			program->setUniformValue("tileSet.verticalSpacing", tileSet->verticalSpacing);

			//Draw
			f->glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}

	void TileLayer::createTBO()
	{
		//TODO: Updating TBO could be optimized by updating data as opposed to recreating the TBO
		//TODO: Updating TBO could be optimized by using glBufferSubData()
		QOpenGLFunctions* f = GameView::context()->functions();

		if (tbo != 0)
			f->glDeleteBuffers(1, &tbo);
		if (tbo_tex != 0)
			f->glDeleteTextures(1, &tbo_tex);
		tbo = 0;
		tbo_tex = 0;

		f->glGenBuffers(1, &tbo);
		f->glBindBuffer(GL_TEXTURE_BUFFER, tbo);
		f->glBufferData(GL_TEXTURE_BUFFER, sizeof(Tile) * _width * _height, tiles.get(), GL_STATIC_DRAW);

		f->glGenTextures(1, &tbo_tex);
		f->glBindBuffer(GL_TEXTURE_BUFFER, 0);
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
					Vector2 position = Vector2(x * _tileRenderWidth, y * _tileRenderHeight);
					Vector2 const meterSize = PhysicsWorld::pixelsToMeters({ _tileRenderWidth / 2.0f, _tileRenderHeight / 2.0f });
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