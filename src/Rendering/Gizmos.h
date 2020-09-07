#pragma once
#include <Utils/Vector.h>
#include "Utils/Colour.h"

namespace Tristeon
{
	struct Vector2;

	/**
	 * Draws 2D shapes in world space.
	 *
	 * Gizmos are drawn at the end of the frame, overlaying all other graphics.
	 * The gizmo drawlist is cleared after every frame, meaning that the gizmo calls need to be made every frame for graphics to show up consistently.
	 *
	 * All current gizmo shapes are GL_LINES only.
	 */
	class Gizmos
	{
		friend class Renderer;
	public:
		/**
		 * Draw a line from world start to world end.
		 */
		static void drawLine(const Vector2& worldStart, const Vector2& worldEnd, const Colour& colour = Colour(1, 1, 1, 1));
		/**
		 * Draw an axis aligned bounding box, from world min, to world max.
		 */
		static void drawAABB(const Vector2& worldMin, const Vector2& worldMax, const Colour& colour = Colour(1, 1, 1, 1));
		/**
		 * Draw a square at the given position with a size and rotation.
		 */
		static void drawSquare(const Vector2& worldPosition, const Vector2& size, const float& rotationDegrees, const Colour& colour = Colour(1, 1, 1, 1));
		/**
		 * Draw a circle at the given position with a given radius.
		 */
		static void drawCircle(const Vector2& worldPosition, const float& radius, const Colour& colour = Colour(1, 1, 1, 1));

	private:
		/**
		 * A simple shape with vertices and a colour.
		 *
		 * When drawn, the vertices simply connect as lines.
		 */
		struct Shape
		{
			Vector<Vector2> vertices;
			Colour colour;
		};
		
		static Vector<Shape> _shapes;

		/**
		 * Draws the gizmos onto the current camera.
		 */
		static void render();
		/**
		 * Clears the gizmo draw list, called every frame.
		 */
		static void clear();
	};
}
