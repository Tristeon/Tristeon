#include "Gamepad.h"

namespace Tristeon
{
	bool Gamepad::buttons[Last];
	bool Gamepad::buttonsPressed[Last];
	bool Gamepad::buttonsReleased[Last];

	Vector2 Gamepad::left = Vector2::zero();
	Vector2 Gamepad::right = Vector2::zero();

	float Gamepad::l2 = 0;
	float Gamepad::r2 = 0;
	
	bool Gamepad::pressed(GamepadButton const& button)
	{
		return buttonsPressed[button];
	}

	bool Gamepad::held(GamepadButton const& button)
	{
		return buttons[button];
	}

	bool Gamepad::released(GamepadButton const& button)
	{
		return buttonsReleased[button];
	}

	Vector2 Gamepad::axisLeft()
	{
		return left;
	}

	Vector2 Gamepad::axisRight()
	{
		return right;
	}

	float Gamepad::axisL2()
	{
		return l2;
	}

	float Gamepad::axisR2()
	{
		return r2;
	}

	void Gamepad::reset()
	{
		std::fill(std::begin(buttonsPressed), std::end(buttonsPressed), false);
		std::fill(std::begin(buttonsReleased), std::end(buttonsReleased), false);
	}

	void Gamepad::buttonChanged(GamepadButton const& button, bool const& pressed)
	{
		if (pressed)
		{
			buttons[button] = true;
			buttonsPressed[button] = true;
			buttonsReleased[button] = false;
		}
		else
		{
			buttons[button] = false;
			buttonsPressed[button] = false;
			buttonsReleased[button] = false;
		}
	}
}
