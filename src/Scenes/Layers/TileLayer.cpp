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
		j["width"] = w;
		j["height"] = h;

		j["tileRenderWidth"] = tileRenderWidth;
		j["tileRenderHeight"] = tileRenderHeight;

		json sets = json::array();
		for (auto tileset : tilesets)
			sets.push_back(tileset->filePath);
		j["tileSets"] = sets;

		json t = json::array();
		for (unsigned int i = 0; i < w * h; i++)
			t.push_back(tiles[i]);
		j["tiles"] = t;
		return j;
	}

	void TileLayer::deserialize(json j)
	{
		Layer::deserialize(j);
		
		w = j["width"];
		h = j["height"];

		tileRenderWidth = j["tileRenderWidth"];
		tileRenderHeight = j["tileRenderHeight"];

		if (j.contains("tileSets"))
		{
			for (size_t i = 0; i < j["tileSets"].size(); i++)
			{
				TileSet* tileset = Resources::load<TileSet>(j["tileSets"][i]);
				tileset->filePath = j["tileSets"][i].get<std::string>();
				tilesets.push_back(tileset);
			}
		}
		
		tiles = std::make_unique<Tile[]>(w * h);
		if (j.contains("tiles"))
		{
			for (unsigned int i = 0; i < w * h && i < j["tiles"].size(); i++)
				tiles[i] = j["tiles"][i];
		}
		createTBO();
		createColliders();
	}

	void TileLayer::tile(int const& x, int const& y, Tile const& value)
	{
		if (x < 0 || y < 0)
			throw std::invalid_argument("Coords can't be less than 0");

		if (x * y > w * h || x > w || y > h)
			throw std::out_of_range("Out of range exception: coords exceed tile level");

		tiles[y * (int)w + x] = value;
		isDirty = true;
	}

	void TileLayer::tile(Vector2Int const& coords, Tile const& value)
	{
		tile(coords.x, coords.y, value);
	}

	Tile TileLayer::tile(int const& x, int const& y) const
	{
		if (x < 0 || y < 0)
			throw std::invalid_argument("Coords can't be less than 0");

		if (x * y > w * h || x > w || y > h)
			throw std::out_of_range("Out of range exception: coords exceed tile level");

		return tiles[y * (int)w + x];
	}

	Tile TileLayer::tile(Vector2Int const& coords)
	{
		return tile(coords.x, coords.y);
	}

	TileSet* TileLayer::tileset(int id)
	{
		for (TileSet* tileset : tilesets)
		{
			if (tileset->id == id)
				return tileset;
		}
		return nullptr;
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
			createTBO(); //TODO: Reloading data is faster than recreating the object
			createColliders();
			isDirty = false;
		}

		//TODO: Camera movement should really be in a behaviour
		if (Keyboard::held(Keyboard::Left))
			scene->getCamera()->position.x -= 1;
		if (Keyboard::held(Keyboard::Right))
			scene->getCamera()->position.x += 1;

		if (Keyboard::held(Keyboard::Up))
			scene->getCamera()->position.y += 1;
		if (Keyboard::held(Keyboard::Down))
			scene->getCamera()->position.y -= 1;

		if (Keyboard::held(Keyboard::Minus))
			scene->getCamera()->zoom -= 0.01f;
		if (Keyboard::held(Keyboard::Equal))
			scene->getCamera()->zoom += 0.01f;

		//Shader
		QOpenGLContext* context = GameView::context();
		QOpenGLFunctions* f = context->functions();
		QOpenGLShaderProgram* program = shader->getShaderProgram();

		//Bind level data
		f->glActiveTexture(GL_TEXTURE1);
		f->glBindTexture(GL_TEXTURE_BUFFER, tbo_tex);
		context->extraFunctions()->glTexBuffer(GL_TEXTURE_BUFFER, GL_R32I, tbo);
		program->setUniformValue("level.data", 1);

		program->setUniformValue("level.width", w);
		program->setUniformValue("level.height", h);

		program->setUniformValue("level.tileRenderWidth", tileRenderWidth);
		program->setUniformValue("level.tileRenderHeight", tileRenderHeight);

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
		QOpenGLFunctions* f = GameView::context()->functions();

		if (tbo != 0)
			f->glDeleteBuffers(1, &tbo);
		if (tbo_tex != 0)
			f->glDeleteTextures(1, &tbo_tex);

		f->glGenBuffers(1, &tbo);
		f->glBindBuffer(GL_TEXTURE_BUFFER, tbo);
		f->glBufferData(GL_TEXTURE_BUFFER, sizeof(Tile) * w * h, tiles.get(), GL_STATIC_DRAW);

		f->glGenTextures(1, &tbo_tex);
		f->glBindBuffer(GL_TEXTURE_BUFFER, 0);
	}

	void TileLayer::createColliders()
	{
		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				bool const colliderExists = fixtures.find(Vector2Int{ x, y }) != fixtures.end();

				int const index = y * w + x;
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
				
				TileSettings const settings = set->info(tile.index);

				//No collider exists but the tile wants a collider
				if (settings.hasCollider && !colliderExists)
				{
					Vector2 position = Vector2(x * tileRenderWidth, y * tileRenderHeight);
					Vector2 const meterSize = PhysicsWorld::pixelsToMeters({ tileRenderWidth / 2.0f, tileRenderHeight / 2.0f });
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