#include "TileLayer.h"

#include <iostream>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include "Input/Keyboard.h"

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
		tileSet = std::make_unique<TileSet>("Project/TilesetTest.jpg", 3, 5, tileInfo);
	}

	void TileLayer::render()
	{
		if (Keyboard::pressed(Key_R))
			shader.reload();

		if (!shader.isReady())
			return;

		//TileSet
		QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
		f->glActiveTexture(0);
		tileSet->texture->bind();

		auto program = shader.getShaderProgram();
		program->setUniformValue("tileSetCols", tileSet->width);
		program->setUniformValue("tileSetRows", tileSet->height);

		//Camera
		program->setUniformValue("cameraPos", 0.0f, 0.0f);
		program->setUniformValue("cameraPixelsX", 1920);
		program->setUniformValue("cameraPixelsY", 1080);

		//Draw
		shader.bind();
		f->glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
