#include "TileLayer.h"

#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>

#include <Input/Keyboard.h>
#include <Scenes/Scene.h>
#include <FileTypes/TileSet.h>

#include <Engine.h>
#include <QOpenGLShaderProgram>

#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

namespace Tristeon
{
	REGISTER_LAYER_CPP(TileLayer);
	REGISTER_TYPE_CPP(TileLayer);

	TileLayer::TileLayer()
	{
		shader = std::make_unique<Shader>("Internal/Shaders/TileShader.vert", "Internal/Shaders/TileShader.frag");
		tileSet = std::make_unique<TileSet>();
		data = Unique<int[]>(new int[1]{ -1 });
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
		j["tileSetPath"] = tileSet->filePath;

		json d = json::array();
		for (unsigned int i = 0; i < w * h; i++)
			d.push_back(data[i]);
		j["data"] = d;
		return j;
	}

	void TileLayer::deserialize(json j)
	{
		Layer::deserialize(j);
		
		w = j["width"];
		h = j["height"];

		if (j.contains("tileSetPath") && j["tileSetPath"].is_string())
			tileSet = std::make_unique<TileSet>(j["tileSetPath"]);
		else
			tileSet = std::make_unique<TileSet>();
		
		data = std::make_unique<int[]>(w * h);
		for (unsigned int i = 0; i < w * h; i++)
			data[i] = j["data"][i];
		
		createTBO();
		createColliders();
	}

	int& TileLayer::operator[](Vector2Int const& coords)
	{
		if (coords.x < 0 || coords.y < 0)
			throw std::invalid_argument("Coords can't be less than 0");

		if (coords.x * coords.y > w * h || coords.x > w || coords.y > h)
			throw std::out_of_range("Out of range exception: coords exceed tile level");

		isDirty = true;

		return data[coords.y * (int)w + coords.x];
	}

	void TileLayer::tile(int const& x, int const& y, int const& value)
	{
		(*this)[{x, y}] = value;
	}

	void TileLayer::tile(Vector2Int const& coords, int const& value)
	{
		(*this)[coords] = value;
	}

	int TileLayer::tile(int const& x, int const& y)
	{
		return (*this)[{x, y}];
	}

	int TileLayer::tile(Vector2Int const& coords)
	{
		return (*this)[coords];
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

		//TileSet
		QOpenGLContext* context = GameView::context();
		QOpenGLFunctions* f = context->functions();
		QOpenGLShaderProgram* program = shader->getShaderProgram();

		//Upload texture & tileset info
		f->glActiveTexture(GL_TEXTURE0);
		tileSet->texture->bind();

		program->setUniformValue("tileSet.texture", 0);

		program->setUniformValue("tileSet.cols", tileSet->cols);
		program->setUniformValue("tileSet.rows", tileSet->rows);

		program->setUniformValue("tileSet.renderWidth", tileSet->tileRenderWidth);
		program->setUniformValue("tileSet.renderHeight", tileSet->tileRenderHeight);

		//Spacing
		program->setUniformValue("tileSet.spacingLeft", tileSet->spacingLeft);
		program->setUniformValue("tileSet.spacingRight", tileSet->spacingRight);
		program->setUniformValue("tileSet.spacingTop", tileSet->spacingTop);
		program->setUniformValue("tileSet.spacingBottom", tileSet->spacingBottom);
		program->setUniformValue("tileSet.horizontalSpacing", tileSet->horizontalSpacing);
		program->setUniformValue("tileSet.verticalSpacing", tileSet->verticalSpacing);

		//Bind level data
		f->glActiveTexture(GL_TEXTURE1);
		f->glBindTexture(GL_TEXTURE_BUFFER, tbo_tex);
		context->extraFunctions()->glTexBuffer(GL_TEXTURE_BUFFER, GL_R32I, tbo);
		program->setUniformValue("level.data", 1);

		program->setUniformValue("level.width", w);
		program->setUniformValue("level.height", h);

		//Draw
		f->glDrawArrays(GL_TRIANGLES, 0, 3);
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
		f->glBufferData(GL_TEXTURE_BUFFER, sizeof(int) * w * h, data.get(), GL_STATIC_DRAW);

		f->glGenTextures(1, &tbo_tex);
		f->glBindBuffer(GL_TEXTURE_BUFFER, 0);
	}

	void TileLayer::createColliders()
	{
		if (!tileSet->tileInfo)
			return;
		
		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				bool const colliderExists = fixtures.find(Vector2Int{ x, y }) != fixtures.end();

				int const index = y * w + x;
				if (data[index] == -1)
				{
					if (colliderExists)
					{
						PhysicsWorld::instance()->staticBody->DestroyFixture(fixtures[{ x, y }]);
						fixtures.erase(fixtures.find({ x, y }));
					}
					continue;
				}
				Tile const tile = tileSet->tileInfo[data[index]];

				//No collider exists but the tile wants a collider
				if (tile.hasCollider && !colliderExists)
				{
					Vector2 position = Vector2(x * tileSet->tileRenderWidth, y * tileSet->tileRenderHeight);
					Vector2 const meterSize = PhysicsWorld::pixelsToMeters({ tileSet->tileRenderWidth / 2.0f, tileSet->tileRenderHeight / 2.0f });
					Vector2 meterPosition = PhysicsWorld::pixelsToMeters(position);

					b2PolygonShape shape;
					shape.SetAsBox(meterSize.x, meterSize.y, meterPosition.convert<b2Vec2>(), 0);

					b2FixtureDef def;
					def.shape = &shape;
					def.density = tile.density;
					def.friction = tile.friction;
					def.restitution = tile.restitution;

					auto fixture = PhysicsWorld::instance()->staticBody->CreateFixture(&def);
					fixture->SetUserData(this);
					fixtures[{x, y}] = fixture;
				}
				//A collider exists but the tile doesn't want a collider
				else if (!tile.hasCollider && colliderExists)
				{
					PhysicsWorld::instance()->staticBody->DestroyFixture(fixtures[{ x, y }]);
					fixtures.erase(fixtures.find({ x, y }));
				}
			}
		}
	}
}