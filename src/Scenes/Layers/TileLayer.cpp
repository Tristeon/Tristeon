#include "TileLayer.h"

#include <iostream>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include <Input/Keyboard.h>
#include <Scenes/Scene.h>

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
		
		shader = Shader("Internal/Shaders/TileShader.vert", "Internal/Shaders/TileShader.frag");

		Tile tileInfo[256] = { {} };
		tileSet = std::make_unique<TileSet>("Project/TilesetTest.png", 3, 5, tileInfo);
	}

	void TileLayer::render(Scene* scene)
	{
		if (Keyboard::pressed(Key_R))
			shader.reload();

		if (!shader.isReady())
			return;

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
		QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
		f->glActiveTexture(0);
		tileSet->texture->bind();

		auto program = shader.getShaderProgram();
		program->setUniformValue("tileSetCols", tileSet->width);
		program->setUniformValue("tileSetRows", tileSet->height);

		//Camera
		program->setUniformValue("cameraPosX", scene->getCamera()->position.x);
		program->setUniformValue("cameraPosY", scene->getCamera()->position.y);
		//TODO: Zoom might not be very intuitive because it zooms into the bottom left as opposed to the center
		program->setUniformValue("cameraPixelsX", (int)(scene->getCamera()->size.x * (1.0f / scene->getCamera()->zoom)));
		program->setUniformValue("cameraPixelsY", (int)(scene->getCamera()->size.y * (1.0f / scene->getCamera()->zoom)));

		//Transparency
		f->glEnable(GL_BLEND);
		f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		//Draw
		shader.bind();
		f->glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
