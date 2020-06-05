#pragma once
#include <queue>
#include <Utils/Vector.h>
#include "Utils/Colour.h"

namespace Tristeon
{
	struct Vector2;

	/**
	 * Draws 2D shapes in world space, on top of all other graphics.
	 */
	class Gizmos
	{
		friend class Renderer;
	public:
		/**
		 * Draw a line from world start to world end.
		 */
		static void drawLine(Vector2 const& worldStart, Vector2 const& worldEnd, Colour const& colour = Colour(1, 1, 1, 1));
		/**
		 * Draw an axis aligned bounding box, from world min, to world max.
		 */
		static void drawAABB(Vector2 const& worldMin, Vector2 const& worldMax, Colour const& colour = Colour(1, 1, 1, 1));
		/**
		 * Draw a square at the given position with a size and rotation.
		 */
		static void drawSquare(Vector2 const& worldPosition, Vector2 const& size, float rotationDegrees, Colour const& colour = Colour(1, 1, 1, 1));
		/**
		 * Draw a circle at the given position with a given radius.
		 */
		static void drawCircle(Vector2 const& worldPosition, float radius, Colour const& colour = Colour(1, 1, 1, 1));

	private:
		struct Shape
		{
			Vector<Vector2> vertices;
			Colour colour;
		};
		
		static std::queue<Shape> shapes;
		static void render();
	};
}
