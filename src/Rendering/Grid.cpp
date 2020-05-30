#include "Grid.h"

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include "GameView.h"
#include "Project.h"
#include "Shader.h"

namespace Tristeon
{
	unsigned int Grid::tileWidth()
	{
		return Project::Graphics::tileWidth();
	}

	unsigned int Grid::tileHeight()
	{
		return Project::Graphics::tileHeight();
	}

	Vector2 Grid::snap(Vector2 const& position)
	{
		return positionByIndex(indexByPosition(position));
	}

	Vector2Int Grid::indexByPosition(Vector2 const& position)
	{
		return indexByPosition(position.x, position.y);
	}

	Vector2Int Grid::indexByPosition(float const& wx, float const& wy)
	{
		return (Vector2Int)Vector2::floor({ (wx + (float)tileWidth() / 2.0f) / (float)tileWidth(), (wy + (float)tileHeight() / 2.0f) / (float)tileHeight() });
	}

	Vector2 Grid::positionByIndex(Vector2Int const& index)
	{
		return positionByIndex(index.x, index.y);
	}

	Vector2 Grid::positionByIndex(int const& ix, int const& iy)
	{
		return {(float)ix * tileWidth(), (float)iy * tileHeight()};
	}

	void Grid::render()
	{
		static Shader shader = Shader("Internal/Shaders/TileShader.vert", "Internal/Shaders/Grid.frag");

		if (!shader.isReady())
			return;

		shader.bind();

		//Shader
		QOpenGLContext* context = GameView::context();
		QOpenGLFunctions* f = context->functions();
		QOpenGLShaderProgram* program = shader.getShaderProgram();

		//Pass grid properties
		program->setUniformValue("grid.tileWidth", tileWidth());
		program->setUniformValue("grid.tileHeight", tileHeight());
		
		//Draw
		f->glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
