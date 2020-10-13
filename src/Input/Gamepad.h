#pragma once
#include <Math/Vector.h>

#ifdef TRISTEON_EDITOR
namespace TristeonEditor { class EditorWindow; }
#endif

namespace Tristeon
{
	/**
	 * Interface to interact with the OS Gamepad/Controller.
	 *
	 * To get gamepad input, use pressed(), held(), released() and more.
	 */
	class Gamepad
	{
		friend class Window;
		friend class Engine;

#ifdef TRISTEON_EDITOR
		friend TristeonEditor::EditorWindow;
#else
		friend class GameWindow;
#endif
		
	public:
		enum GamepadButton
		{
			L1,
			L3,

			R1,
			R3,

			A,
			B,
			X,
			Y,

			Center,
			Guide,
			Select,
			Start,

			Left,
			Right,
			Up,
			Down,

			Last = Down
		};
		
		/**
		 * Was this button pressed down on the current frame?
		 */
		[[nodiscard]] static bool pressed(const GamepadButton& button);

		/**
		 * Was this button held during the current frame?
		 */
		[[nodiscard]] static bool held(const GamepadButton& button);

		/**
		 * Was this button released on the current frame?
		 */
		[[nodiscard]] static bool released(const GamepadButton& button);

		/**
		 * The x and y value of the left axis, often referred to as the left thumbstick.
		 *
		 * Its values are between -1 and 1.
		 */
		[[nodiscard]] static Vector axisLeft();

		/**
		 * The x and y value of the right axis, often referred to as the right thumbstick.
		 *
		 * Its values are between -1 and 1.
		 */
		[[nodiscard]] static Vector axisRight();

		/**
		 * The value of the L2 axis, often referred to as the left trigger button.
		 *
		 * This value ranges from 0.0 when not pressed to 1.0 when pressed completely.
		 */
		[[nodiscard]] static float axisL2();

		/**
		 * The value of the R2 axis, often referred to as the right trigger button.
		 *
		 * This value ranges from 0.0 when not pressed to 1.0 when pressed completely.
		 */
		[[nodiscard]] static float axisR2();

	private:
		static bool _buttons[];
		static bool _buttonsPressed[];
		static bool _buttonsReleased[];

		static Vector _left;
		static Vector _right;

		static float _l2;
		static float _r2;

		/**
		 * Clears frame-based data structures.
		 * Called every frame by the main Engine loop.
		 */
		static void reset();
		/**
		 * Clears all input.
		 */
		static void clearAll();

		/**
		 * Input update sent by the window interface.
		 */
		static void buttonChanged(const GamepadButton& button, const bool& pressed);
	};
}