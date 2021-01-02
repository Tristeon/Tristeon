#include "Grid.h"

#include "Project.h"
#include "Shader.h"

#include <glad/glad.h>
#include "Editor/GameView.h" 

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

	VectorI Grid::tileSize()
	{
		return VectorI((int)tileWidth(), (int)tileHeight());
	}

	Vector Grid::snap(const Vector& position)
	{
		return positionByIndex(indexByPosition(position));
	}

	VectorI Grid::indexByPosition(const Vector& position)
	{
		return indexByPosition(position.x, position.y);
	}

	VectorI Grid::indexByPosition(const float& wx, const float& wy)
	{
		Vector result{ (wx + (float)tileWidth() / 2.0f) / (float)tileWidth(), (wy + (float)tileHeight() / 2.0f) / (float)tileHeight() };
		return static_cast<VectorI>(result.floor());
	}

	Vector Grid::positionByIndex(const VectorI& index)
	{
		return positionByIndex(index.x, index.y);
	}

	Vector Grid::positionByIndex(const int& ix, const int& iy)
	{
		return {(float)ix * (float)tileWidth(), (float)iy * (float)tileHeight()};
	}

	void Grid::render()
	{
		static Shader shader = Shader("Internal/Shaders/FullscreenTriangle.vert", "Internal/Shaders/Grid.frag");

		if (!shader.ready())
			return;

		shader.bind();

		//Pass grid properties
		shader.setUniformValue("grid.tileWidth", tileWidth());
		shader.setUniformValue("grid.tileHeight", tileHeight());
		
		//Draw
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}