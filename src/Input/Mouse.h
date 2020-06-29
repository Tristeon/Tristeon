#pragma once
#include <Math/Vector2Int.h>

#ifdef TRISTEON_EDITOR
namespace TristeonEditor { class EditorWindow; }
#endif

namespace Tristeon
{
	class Window;
	class Engine;
#ifndef TRISTEON_EDITOR
	class GameWindow;
#endif
	
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

#ifdef TRISTEON_EDITOR
		friend TristeonEditor::EditorWindow;
#else
		friend GameWindow;
#endif
		
	public:
		enum MouseButton
		{
			Left = 1,
			Right = 2,
			Middle = 3,
			Back = 4,
			Extra1 = Back,
			Forward = 5,
			Extra2 = Forward,
			Task = 6,
			Extra3 = 7,
			Extra4 = 8,
			Last = Extra4
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
		static void onPress(MouseButton const& button);
		static void onRelease(MouseButton const& button);
		static void onMove(Vector2Int const& pos);
		static void onScroll(Vector2Int const& change);

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

		static Vector2Int mousePos;
		static Vector2Int mouseDelta;
		static Vector2Int scrollDelta;
	};
}
