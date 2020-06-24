#pragma once
#include <Math/Vector2.h>

namespace Tristeon
{
	class Window;
	class Engine;

	/**
	 * Interface to interact with the OS Gamepad/Controller.
	 *
	 * To get gamepad input, use pressed(), held(), released() and more.
	 */
	class Gamepad
	{
		friend Window;
		friend Engine;

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
		static bool pressed(GamepadButton const& button);

		/**
		 * Was this button held during the current frame?
		 */
		static bool held(GamepadButton const& button);

		/**
		 * Was this button released on the current frame?
		 */
		static bool released(GamepadButton const& button);

		/**
		 * The x and y value of the left axis, often referred to as the left thumbstick.
		 *
		 * Its values are between -1 and 1.
		 */
		static Vector2 axisLeft();

		/**
		 * The x and y value of the right axis, often referred to as the right thumbstick.
		 *
		 * Its values are between -1 and 1.
		 */
		static Vector2 axisRight();

		/**
		 * The value of the L2 axis, often referred to as the left trigger button.
		 *
		 * This value ranges from 0.0 when not pressed to 1.0 when pressed completely.
		 */
		static float axisL2();

		/**
		 * The value of the R2 axis, often referred to as the right trigger button.
		 *
		 * This value ranges from 0.0 when not pressed to 1.0 when pressed completely.
		 */
		static float axisR2();

	private:
		static bool buttons[];
		static bool buttonsPressed[];
		static bool buttonsReleased[];

		static Vector2 left;
		static Vector2 right;

		static float l2;
		static float r2;

		/**
		 * Clears frame-based data structures.
		 * Called every frame by the main Engine loop.
		 */
		static void reset();
		/**
		 * Clears all input.
		 */
		static void clearAll();

		static void buttonChanged(GamepadButton const& button, bool const& pressed);
	};
}