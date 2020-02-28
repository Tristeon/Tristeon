#include "TileLayer.h"

#include <iostream>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include <Input/Keyboard.h>
#include <Scenes/Scene.h>

#include "Engine.h"

namespace Tristeon
{
	TileLayer::TileLayer()
	{
		width = 5;
		height = 5;
		
		data = std::unique_ptr<int[]>(new int[25]
			{
				1, 2, 3, 4, 5,
				5, 4, 3, 2, 1,
				1, 1, 5, 4, 4,
				3, 3, 1, 2, 2
			});
		
		shader = std::make_unique<Shader>("Internal/Shaders/TileShader.vert", "Internal/Shaders/TileShader.frag");
		
		Tile tileInfo[256] = { {} };
		tileSet = std::make_unique<TileSet>("Project/TilesetTest.png", 3, 5, tileInfo);
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
