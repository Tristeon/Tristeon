#include "TileLayer.h"

#include <iostream>

#include "Rendering/GLContext.h"

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
		tileSet = std::make_unique<TileSet>("Project/Tiny Platformer/Tileset(16x16)/Tileset.png", 16, 16, tileInfo);
	}

	void TileLayer::render()
	{
		if (!shader.isReady())
			return;

		//TileSet
		GLContext::getInstance()->glActiveTexture(0);
		tileSet->texture->bind();

		shader.getShaderProgram()->setUniformValue("tileSetWidth", tileSet->width);
		shader.getShaderProgram()->setUniformValue("tileSetWidth", tileSet->height);

		//Camera
		shader.getShaderProgram()->setUniformValue("cameraPos", 0.0f, 0.0f);
		shader.getShaderProgram()->setUniformValue("cameraWidth", 10.0f);
		shader.getShaderProgram()->setUniformValue("cameraHeight", 10.0f);

		//Draw
		shader.bind();
		GLContext::getInstance()->glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
