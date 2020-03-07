#include "TileLayer.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include <Input/Keyboard.h>
#include <Scenes/Scene.h>

#include "Engine.h"

namespace Tristeon
{
	REGISTER_TYPE_CPP(TileLayer)
	
	TileLayer::TileLayer()
	{
		shader = std::make_unique<Shader>("Internal/Shaders/TileShader.vert", "Internal/Shaders/TileShader.frag");
		tileSet = std::make_unique<TileSet>();
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
		QOpenGLFunctions* f = Engine::instance()->gameView()->context()->functions();
		f->glActiveTexture(0);
		tileSet->texture->bind();

		QOpenGLShaderProgram* program = shader->getShaderProgram();
		program->setUniformValue("tileSetCols", tileSet->width);
		program->setUniformValue("tileSetRows", tileSet->height);

		//Camera
		program->setUniformValue("camera.posX", scene->getCamera()->position.x);
		program->setUniformValue("camera.posY", scene->getCamera()->position.y);
		program->setUniformValue("camera.pixelsX", (int)scene->getCamera()->size.x);
		program->setUniformValue("camera.pixelsY", (int)scene->getCamera()->size.y);
		program->setUniformValue("camera.zoom", scene->getCamera()->zoom);

		//Draw
		f->glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
