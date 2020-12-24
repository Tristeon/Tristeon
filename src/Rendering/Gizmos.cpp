#include "Gizmos.h"

#include <glad/glad.h>
#include "Math/Math.h"
#include "Math/Vector.h"
#include "Shader.h"

namespace Tristeon
{
	List<Gizmos::Shape> Gizmos::_shapes;
	
	void Gizmos::drawLine(const Vector& worldStart, const Vector& worldEnd, const Colour& colour)
	{
		_shapes.add({ { worldStart, worldEnd }, colour });
	}

	void Gizmos::drawAABB(const Vector& worldMin, const Vector& worldMax, const Colour& colour)
	{
		auto const bl = worldMin;
		auto const tl = Vector(worldMin.x, worldMax.y);
		auto const br = Vector(worldMax.x, worldMin.y);
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

	void Gizmos::drawSquare(const Vector& worldPosition, const Vector& size, const float& rotationDegrees, const Colour& colour)
	{
		auto const bl = Math::orbit(worldPosition, -size / 2.0f, rotationDegrees);
		auto const tl = Math::orbit(worldPosition, Vector(-size.x / 2.0f, size.y / 2.0f), rotationDegrees);
		auto const br = Math::orbit(worldPosition, Vector(size.x / 2.0f, -size.y / 2.0f), rotationDegrees);
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

	void Gizmos::drawCircle(const Vector& worldPosition, const float& radius, const Colour& colour)
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
		static Shader shader = Shader("Internal/Shaders/WorldVertex.vert", "Internal/Shaders/Colour.frag");
		shader.bind();
		
		for (auto& shape : _shapes)
		{
			//create buffer
			unsigned int buffer = 0;
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, shape.vertices.size() * sizeof(Vector), shape.vertices.ptr(), GL_STATIC_DRAW);

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
