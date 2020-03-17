#pragma once
#include <Math/Vector2Int.h>
#include <QMouseEvent>

namespace Tristeon
{
	//Slightly adjusted Qt::MouseButton, copied into the Tristeon namespace to keep the API consistent
	enum MouseButton {
		LeftButton = 1,
		RightButton = 2,
		MidButton = 3,
		MiddleButton = MidButton,
		BackButton = 4,
		XButton1 = BackButton,
		ExtraButton1 = XButton1,
		ForwardButton = 5,
		XButton2 = ForwardButton,
		ExtraButton2 = ForwardButton,
		TaskButton = 6,
		ExtraButton3 = 7,
		ExtraButton4 = 8,
		ExtraButton5 = 9,
		ExtraButton6 = 10,
		ExtraButton7 = 11,
		ExtraButton8 = 12,
		ExtraButton9 = 13,
		ExtraButton10 = 14,
		ExtraButton11 = 15,
		ExtraButton12 = 16,
		ExtraButton13 = 17,
		ExtraButton14 = 18,
		ExtraButton15 = 19,
		ExtraButton16 = 20,
		ExtraButton17 = 21,
		ExtraButton18 = 22,
		ExtraButton19 = 23,
		ExtraButton20 = 24,
		ExtraButton21 = 25,
		ExtraButton22 = 26,
		ExtraButton23 = 27,
		ExtraButton24 = 28,
		MaxMouseButton = ExtraButton24,
	};
	
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
		static Vector2 position();
	private:
		static void onPress(QMouseEvent const& event);
		static void onRelease(QMouseEvent const& event);
		static void onDoubleClick(QMouseEvent const& event);
		static void onMove(QMouseEvent const& event);

		/**
		 * Clears frame-based data structures.
		 * Called every frame by the main Engine loop.
		 */
		static void reset();
		
		static bool buttons[];
		static bool buttonsPressed[];
		static bool buttonsReleased[];
		static bool buttonsDoubleClicked[];

		static Vector2Int mousePos;
	};
}
