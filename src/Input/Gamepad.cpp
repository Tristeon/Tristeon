#include "Gamepad.h"
#include "magic_enum.hpp"

namespace Tristeon
{
	Gamepad Gamepad::gamepads[15];

	bool Gamepad::connected(const unsigned& gamepad)
	{
		if (gamepad >= 15)
			return false;
		return gamepads[gamepad]._connected;
	}

	String Gamepad::name(const unsigned& gamepad)
	{
		return gamepads[gamepad]._name;
	}

	bool Gamepad::pressed(const GamepadButton& button, const unsigned& gamepad)
	{
		if (gamepad >= 15)
			return false;
		return gamepads[gamepad]._buttonsPressed[button];
	}

	bool Gamepad::held(const GamepadButton& button, const unsigned& gamepad)
	{
		if (gamepad >= 15)
			return false;
		return gamepads[gamepad]._buttons[button];
	}

	bool Gamepad::released(const GamepadButton& button, const unsigned& gamepad)
	{
		if (gamepad >= 15)
			return false;
		return gamepads[gamepad]._buttonsReleased[button];
	}

	Vector Gamepad::axisLeft(const unsigned& gamepad)
	{
		if (gamepad >= 15)
			return {};
		return gamepads[gamepad]._left;
	}

	Vector Gamepad::axisRight(const unsigned& gamepad)
	{
		if (gamepad >= 15)
			return {};
		return gamepads[gamepad]._right;
	}

	float Gamepad::axisL2(const unsigned& gamepad)
	{
		if (gamepad >= 15)
			return 0;
		return gamepads[gamepad]._l2;
	}

	float Gamepad::axisR2(const unsigned& gamepad)
	{
		if (gamepad >= 15)
			return 0;
		return gamepads[gamepad]._r2;
	}

	void Gamepad::reset()
	{
		for (auto& gamepad : gamepads)
		{
			std::fill(std::begin(gamepad._buttonsPressed), std::end(gamepad._buttonsPressed), false);
			std::fill(std::begin(gamepad._buttonsReleased), std::end(gamepad._buttonsReleased), false);
		}
	}

	void Gamepad::clearAll()
	{
		for (auto i = 0; i < maximumGamepads; i++)
			clearGamepad(i);
	}

	void Gamepad::clearGamepad(const unsigned& gamepad)
	{
		std::fill(std::begin(gamepads[gamepad]._buttonsPressed), std::end(gamepads[gamepad]._buttonsPressed), false);
		std::fill(std::begin(gamepads[gamepad]._buttons), std::end(gamepads[gamepad]._buttons), false);
		std::fill(std::begin(gamepads[gamepad]._buttonsReleased), std::end(gamepads[gamepad]._buttonsReleased), false);

		gamepads[gamepad]._l2 = {};
		gamepads[gamepad]._r2 = {};
		gamepads[gamepad]._left = {};
		gamepads[gamepad]._right = {};
	}

	void Gamepad::buttonChanged(const unsigned int& gamepad, GamepadButton const& button, bool const& pressed)
	{
		if (pressed)
		{
			gamepads[gamepad]._buttons[button] = true;
			gamepads[gamepad]._buttonsPressed[button] = true;
			gamepads[gamepad]._buttonsReleased[button] = false;
		}
		else
		{
			gamepads[gamepad]._buttons[button] = false;
			gamepads[gamepad]._buttonsPressed[button] = false;
			gamepads[gamepad]._buttonsReleased[button] = false;
		}
	}
}