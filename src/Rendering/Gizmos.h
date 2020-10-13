#pragma once
#include <Utils/Colour.h>
#include <Standard/List.h>

#include <Math/Vector.h>

namespace Tristeon
{
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
		static void drawLine(const Vector& worldStart, const Vector& worldEnd, const Colour& colour = Colour(1, 1, 1, 1));
		/**
		 * Draw an axis aligned bounding box, from world min, to world max.
		 */
		static void drawAABB(const Vector& worldMin, const Vector& worldMax, const Colour& colour = Colour(1, 1, 1, 1));
		/**
		 * Draw a square at the given position with a size and rotation.
		 */
		static void drawSquare(const Vector& worldPosition, const Vector& size, const float& rotationDegrees, const Colour& colour = Colour(1, 1, 1, 1));
		/**
		 * Draw a circle at the given position with a given radius.
		 */
		static void drawCircle(const Vector& worldPosition, const float& radius, const Colour& colour = Colour(1, 1, 1, 1));

	private:
		/**
		 * A simple shape with vertices and a colour.
		 *
		 * When drawn, the vertices simply connect as lines.
		 */
		struct Shape
		{
			List<Vector> vertices;
			Colour colour;
		};
		
		static List<Shape> _shapes;

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
