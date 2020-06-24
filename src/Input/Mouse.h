#pragma once
#include <Math/Vector2Int.h>
#include <QMouseEvent>

namespace Tristeon
{
	class Window;
	class Engine;

	/**
	 * Interface to interact with the OS Mouse.
	 *
	 * To get mouse input, use pressed(), held(), released() and more.
	 * For mouse movement, use position().
	 */
	class Mouse final
	{
		friend Window;
		friend Engine;

	public:
		//Slightly adjusted Qt::MouseButton, copied into the Tristeon namespace to keep the API consistent
		enum MouseButton
		{
			Left = 1,
			Right = 2,
			Mid = 3,
			Middle = Mid,
			Back = 4,
			Extra1 = Back,
			Forward = 5,
			Extra2 = Forward,
			Task = 6,
			Extra3 = 7,
			Extra4 = 8,
			Extra5 = 9,
			Extra6 = 10,
			Extra7 = 11,
			Extra8 = 12,
			Extra9 = 13,
			Extra10 = 14,
			Extra11 = 15,
			Extra12 = 16,
			Extra13 = 17,
			Extra14 = 18,
			Extra15 = 19,
			Extra16 = 20,
			Extra17 = 21,
			Extra18 = 22,
			Extra19 = 23,
			Extra20 = 24,
			Extra21 = 25,
			Extra22 = 26,
			Extra23 = 27,
			Extra24 = 28,
			Last = Extra24,
		};

		/**
		 * Was this button pressed down on the current frame?
		 */
		static bool pressed(MouseButton const& button);

		/**
		 * Was this button held during the current frame?
		 */
		static bool held(MouseButton const& button);

		/**
		 * Was this button released on the current frame?
		 */
		static bool released(MouseButton const& button);

		/**
		 * Did a double click event occur with this mouse button?
		 */
		static bool doubleClicked(MouseButton const& button);

		/**
		 * The position of the mouse, local to the Window.
		 */
		static Vector2Int position();

		/**
		 * The amount of pixels the mouse moved in the current frame.
		 */
		static Vector2Int deltaPos();

		/**
		 * The amount in degrees at which the scroll wheel scrolled in the current frame.
		 *
		 * Some mice allow the user to tilt the wheel to perform horizontal scrolling, and some touchpads support a horizontal scrolling gesture hence why this returns a Vector2Int.
		 * Most mouse types work in steps of 15 degrees, in which case the delta value is a multiple of 120; i.e., 120 units * 1/8 = 15 degrees.
		 */
		static Vector2Int deltaScroll();

	private:
		static void onPress(QMouseEvent const& event);
		static void onRelease(QMouseEvent const& event);
		static void onDoubleClick(QMouseEvent const& event);
		static void onMove(QMouseEvent const& event);
		static void onScroll(QWheelEvent const& event);

		/**
		 * Clears frame-based data structures.
		 * Called every frame by the main Engine loop.
		 */
		static void reset();
		/**
		 * Clears all input.
		 */
		static void clearAll();

		static bool buttons[];
		static bool buttonsPressed[];
		static bool buttonsReleased[];
		static bool buttonsDoubleClicked[];

		static Vector2Int mousePos;
		static Vector2Int mouseDelta;
		static Vector2Int scrollDelta;
	};
}
