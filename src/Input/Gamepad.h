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
			A,
			B,
			X,
			Y,

			L1,
			R1,
			
			Back,
			Select = Back,
			Start,
			Guide,
			
			L3,
			R3,

			Up,
			Right,
			Down,
			Left,

			Last = Left
		};

		/**
		 * Checks if the given gamepad is connected.
		 * Gamepads aren't sorted so if a gamepad isn't connected, this doesn't mean that it's the last gamepad in the list.
		 */
		[[nodiscard]] static bool connected(const unsigned int& gamepad);

		/**
		 * Returns the name (if any) of the gamepad.
		 */
		[[nodiscard]] static String name(const unsigned int& gamepad);
		
		/**
		 * Was this button pressed down on the current frame?
		 */
		[[nodiscard]] static bool pressed(const GamepadButton& button, const unsigned int& gamepad = 0);

		/**
		 * Was this button held during the current frame?
		 */
		[[nodiscard]] static bool held(const GamepadButton& button, const unsigned int& gamepad = 0);

		/**
		 * Was this button released on the current frame?
		 */
		[[nodiscard]] static bool released(const GamepadButton& button, const unsigned int& gamepad = 0);

		/**
		 * The x and y value of the left axis, often referred to as the left thumbstick.
		 *
		 * Its values are between -1 and 1.
		 */
		[[nodiscard]] static Vector axisLeft(const unsigned int& gamepad = 0);

		/**
		 * The x and y value of the right axis, often referred to as the right thumbstick.
		 *
		 * Its values are between -1 and 1.
		 */
		[[nodiscard]] static Vector axisRight(const unsigned int& gamepad = 0);

		/**
		 * The value of the L2 axis, often referred to as the left trigger button.
		 *
		 * This value ranges from 0.0 when not pressed to 1.0 when pressed completely.
		 */
		[[nodiscard]] static float axisL2(const unsigned int& gamepad = 0);

		/**
		 * The value of the R2 axis, often referred to as the right trigger button.
		 *
		 * This value ranges from 0.0 when not pressed to 1.0 when pressed completely.
		 */
		[[nodiscard]] static float axisR2(const unsigned int& gamepad = 0);

		static constexpr unsigned int maximumGamepads = 15;
	private:
		bool _buttons[Last + 1] = {};
		bool _buttonsPressed[Last + 1] = {};
		bool _buttonsReleased[Last + 1] = {};

		Vector _left{};
		Vector _right{};

		float _l2 = 0;
		float _r2 = 0;

		bool _connected = false;

		String _name = "";
		
		static Gamepad gamepads[15];
		
		/**
		 * Clears frame-based data structures.
		 * Called every frame by the main Engine loop.
		 */
		static void reset();

		/**
		 * Clears every stat, used on things like window focus.
		 */
		static void clearAll();
		
		/**
		 * Input update sent by the window interface.
		 */
		static void buttonChanged(const unsigned int& gamepad, const GamepadButton& button, const bool& pressed);
	};
}