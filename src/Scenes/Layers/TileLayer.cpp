#include "TileLayer.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

#include <Input/Keyboard.h>
#include <Scenes/Scene.h>

#include "Engine.h"

namespace Tristeon
{
	REGISTER_TYPE_CPP(TileLayer);

	TileLayer::TileLayer()
	{
		shader = std::make_unique<Shader>("Internal/Shaders/TileShader.vert", "Internal/Shaders/TileShader.frag");
		tileSet = std::make_unique<TileSet>();

		int data[60] = {
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			4, 4, -1, -1, -1, -1, -1, -1, -1, -1,
			9, 10, -1, -1, -1, -1, 4, 4, 14, 14,
			-1, -1, -1, -1, -1, 4, 10, 9, 4, 14,
			0, 1, 2, 0, 8, 3, 6, 3, 6, 8,
			3, 6, 3, 6, 3, 6, 3, 6, 3, 6
		};
		
		QOpenGLFunctions* f = Engine::instance()->gameView()->context()->functions();
		f->glGenBuffers(1, &tbo);
		f->glBindBuffer(GL_TEXTURE_BUFFER, tbo);
		f->glBufferData(GL_TEXTURE_BUFFER, sizeof(int) * 60, data, GL_STATIC_DRAW);

		f->glGenTextures(1, &tbo_tex);
		f->glBindBuffer(GL_TEXTURE_BUFFER, 0);
	}

	TileLayer::~TileLayer()
	{
		QOpenGLFunctions* f = Engine::instance()->gameView()->context()->functions();
		f->glDeleteBuffers(1, &tbo);
	}

	json TileLayer::serialize()
	{
		json j;
		j["typeID"] = TRISTEON_TYPENAME(TileLayer);
		j["width"] = width;
		j["height"] = height;
		j["tileSet"] = tileSet->serialize();
		//TODO: Serialize TileLayer level data
		return j;
	}

	void TileLayer::deserialize(json j)
	{
		width = j["width"];
		height = j["height"];
		tileSet->deserialize(j["tileSet"]);
		//TODO: Deserialize TileLayer level data
	}

	void TileLayer::render(Renderer* renderer, Scene* scene)
	{
		if (Keyboard::pressed(Key_R))
			shader->reload();

		if (!shader->isReady())
			return;
		shader->bind();

		if (Keyboard::held(Key_Left))
			scene->getCamera()->position.x -= 1;
		if (Keyboard::held(Key_Right))
			scene->getCamera()->position.x += 1;

		if (Keyboard::held(Key_Up))
			scene->getCamera()->position.y += 1;
		if (Keyboard::held(Key_Down))
			scene->getCamera()->position.y -= 1;

		if (Keyboard::held(Key_Minus))
			scene->getCamera()->zoom -= 0.01f;
		if (Keyboard::held(Key_Equal))
			scene->getCamera()->zoom += 0.01f;

		//TileSet
		QOpenGLContext* context = Engine::instance()->gameView()->context();
		QOpenGLFunctions* f = context->functions();
		QOpenGLShaderProgram* program = shader->getShaderProgram();

		//Upload texture & tileset info
		f->glActiveTexture(GL_TEXTURE0);
		tileSet->texture->bind();
		program->setUniformValue("tileSet", 0);

		program->setUniformValue("tileSetCols", tileSet->width);
		program->setUniformValue("tileSetRows", tileSet->height);

		//Camera
		program->setUniformValue("camera.posX", scene->getCamera()->position.x);
		program->setUniformValue("camera.posY", scene->getCamera()->position.y);
		program->setUniformValue("camera.pixelsX", (int)scene->getCamera()->size.x);
		program->setUniformValue("camera.pixelsY", (int)scene->getCamera()->size.y);
		program->setUniformValue("camera.zoom", scene->getCamera()->zoom);

		//Bind level data
		f->glActiveTexture(GL_TEXTURE1);
		f->glBindTexture(GL_TEXTURE_BUFFER, tbo_tex);
		context->extraFunctions()->glTexBuffer(GL_TEXTURE_BUFFER, GL_R32I, tbo);
		program->setUniformValue("levelData", 1);
		
		//Draw
		f->glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
