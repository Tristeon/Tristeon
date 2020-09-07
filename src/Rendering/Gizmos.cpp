#include "Gizmos.h"

#include <glad/glad.h>
#include "Math/Math.h"
#include "Math/Vector2.h"
#include "Shader.h"

namespace Tristeon
{
	Vector<Gizmos::Shape> Gizmos::_shapes;
	
	void Gizmos::drawLine(const Vector2& worldStart, const Vector2& worldEnd, const Colour& colour)
	{
		_shapes.add({ { worldStart, worldEnd }, colour });
	}

	void Gizmos::drawAABB(const Vector2& worldMin, const Vector2& worldMax, const Colour& colour)
	{
		auto const bl = worldMin;
		auto const tl = Vector2(worldMin.x, worldMax.y);
		auto const br = Vector2(worldMax.x, worldMin.y);
		auto const tr = worldMax;
		
		_shapes.add(
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

	void Gizmos::drawSquare(const Vector2& worldPosition, const Vector2& size, const float& rotationDegrees, const Colour& colour)
	{
		auto const bl = Math::orbit(worldPosition, -size / 2.0f, rotationDegrees);
		auto const tl = Math::orbit(worldPosition, Vector2(-size.x / 2.0f, size.y / 2.0f), rotationDegrees);
		auto const br = Math::orbit(worldPosition, Vector2(size.x / 2.0f, -size.y / 2.0f), rotationDegrees);
		auto const tr = Math::orbit(worldPosition, size / 2.0f, rotationDegrees);

		_shapes.add(
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

	void Gizmos::drawCircle(const Vector2& worldPosition, const float& radius, const Colour& colour)
	{
		Shape shape;
		shape.colour = colour;
		
		for (auto i = 0; i < 32; i++)
		{
			auto const theta = Math::TAU * i / 32.0f;

			auto const x = radius * cosf(theta);
			auto const y = radius * sinf(theta);

			shape.vertices.add({ x + worldPosition.x , y + worldPosition.y });

			if (i != 0)
				shape.vertices.add({ x + worldPosition.x , y + worldPosition.y });

			if (i == 31)
				shape.vertices.add({ radius + worldPosition.x, worldPosition.y });
		}

		_shapes.add(shape);
	}

	void Gizmos::render()
	{
		static Shader shader = Shader("Internal/Shaders/Gizmo.vert", "Internal/Shaders/Gizmo.frag");
		shader.bind();
		
		for (auto& shape : _shapes)
		{
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

	void Gizmos::clear()
	{
		_shapes.clear();
	}
}
