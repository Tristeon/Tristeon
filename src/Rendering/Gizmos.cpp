#include "Gizmos.h"

#include <glad/glad.h>
#include "Math/Math.h"
#include "Math/Vector2.h"
#include "Shader.h"

namespace Tristeon
{
	std::queue<Gizmos::Shape> Gizmos::shapes;
	
	void Gizmos::drawLine(Vector2 const& worldStart, Vector2 const& worldEnd, Colour const& colour)
	{
		shapes.push({ { worldStart, worldEnd }, colour });
	}

	void Gizmos::drawAABB(Vector2 const& worldMin, Vector2 const& worldMax, Colour const& colour)
	{
		Vector2 const bl = worldMin;
		Vector2 const tl = Vector2(worldMin.x, worldMax.y);
		Vector2 const br = Vector2(worldMax.x, worldMin.y);
		Vector2 const tr = worldMax;
		
		shapes.push(
			{
				{
					bl, tl,
					tl, tr,
					tr, br,
					br,	bl
				},
				colour
			});
	}

	void Gizmos::drawSquare(Vector2 const& worldPosition, Vector2 const& size, float rotationDegrees, Colour const& colour)
	{
		Vector2 const bl = Math::orbit(worldPosition, -size / 2.0f, rotationDegrees);
		Vector2 const tl = Math::orbit(worldPosition, Vector2(-size.x / 2.0f, size.y / 2.0f), rotationDegrees);
		Vector2 const br = Math::orbit(worldPosition, Vector2(size.x / 2.0f, -size.y / 2.0f), rotationDegrees);
		Vector2 const tr = Math::orbit(worldPosition, size / 2.0f, rotationDegrees);

		shapes.push(
			{
				{
					bl, tl,
					tl, tr,
					tr, br,
					br,	bl
				},
				colour
			});
	}

	void Gizmos::drawCircle(Vector2 const& worldPosition, float radius, Colour const& colour)
	{
		Shape shape;
		shape.colour = colour;
		
		for (int i = 0; i < 32; i++)
		{
			float const theta = Math::twoPI * i / 32.0f;

			float const x = radius * cosf(theta);
			float const y = radius * sinf(theta);

			shape.vertices.add({ x + worldPosition.x , y + worldPosition.y });

			if (i != 0)
				shape.vertices.add({ x + worldPosition.x , y + worldPosition.y });

			if (i == 31)
				shape.vertices.add({ radius + worldPosition.x, worldPosition.y });
		}

		shapes.push(shape);
	}

	void Gizmos::render()
	{
		static Shader shader = Shader("Internal/Shaders/Gizmo.vert", "Internal/Shaders/Gizmo.frag");
		shader.bind();
		
		while (!shapes.empty())
		{
			auto shape = shapes.front();
			shapes.pop();

			//create buffer
			unsigned int buffer = 0;
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, shape.vertices.size() * sizeof(Vector2), shape.vertices.ptr(), GL_STATIC_DRAW);

			//bind vertex attrib pointer
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(0);

			//draw
			shader.setUniformValue("colour", shape.colour.r, shape.colour.g, shape.colour.b, shape.colour.a);
			glLineWidth(2);
			glDrawArrays(GL_LINES, 0, shape.vertices.size());

			//cleanup
			glDisableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDeleteBuffers(1, &buffer);
		}
	}
}
