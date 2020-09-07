#pragma once
#include <Math/Vector2Int.h>

#ifdef TRISTEON_EDITOR
namespace TristeonEditor { class EditorWindow; }
#endif

namespace Tristeon
{
	/**
	 * Interface to interact with the OS Mouse.
	 *
	 * To get mouse input, use pressed(), held(), released() and more.
	 * For mouse movement, use position().
	 */
	class Mouse final
	{
		friend class Window;
		friend class Engine;

#ifdef TRISTEON_EDITOR
		friend TristeonEditor::EditorWindow;
#else
		friend class GameWindow;
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
		[[nodiscard]] static bool pressed(const MouseButton& button);

		/**
		 * Was this button held during the current frame?
		 */
		[[nodiscard]] static bool held(const MouseButton& button);

		/**
		 * Was this button released on the current frame?
		 */
		[[nodiscard]] static bool released(const MouseButton& button);

		/**
		 * The position of the mouse, local to the Window.
		 */
		[[nodiscard]] static Vector2Int position();

		/**
		 * The amount of pixels the mouse moved in the current frame.
		 */
		[[nodiscard]] static Vector2Int deltaPos();

		/**
		 * The amount in degrees at which the scroll wheel scrolled in the current frame.
		 *
		 * Some mice allow the user to tilt the wheel to perform horizontal scrolling, and some touchpads support a horizontal scrolling gesture hence why this returns a Vector2Int.
		 * Most mouse types work in steps of 15 degrees, in which case the delta value is a multiple of 120; i.e., 120 units * 1/8 = 15 degrees.
		 */
		[[nodiscard]] static Vector2Int deltaScroll();

	private:
		static void onPress(const MouseButton& button);
		static void onRelease(const MouseButton& button);
		static void onMove(const Vector2Int& pos);
		static void onScroll(const Vector2Int& change);

		/**
		 * Clears frame-based data structures.
		 * Called every frame by the main Engine loop.
		 */
		static void reset();
		/**
		 * Clears all input.
		 */
		static void clearAll();

		static bool _buttons[];
		static bool _buttonsPressed[];
		static bool _buttonsReleased[];

		static Vector2Int _mousePos;
		static Vector2Int _mouseDelta;
		static Vector2Int _scrollDelta;
	};
}