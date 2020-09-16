#include "Gamepad.h"

namespace Tristeon
{
	bool Gamepad::_buttons[Last];
	bool Gamepad::_buttonsPressed[Last];
	bool Gamepad::_buttonsReleased[Last];

	Vector2 Gamepad::_left{};
	Vector2 Gamepad::_right{};

	float Gamepad::_l2 = 0;
	float Gamepad::_r2 = 0;
	
	bool Gamepad::pressed(const GamepadButton& button)
	{
		return _buttonsPressed[button];
	}

	bool Gamepad::held(const GamepadButton& button)
	{
		return _buttons[button];
	}

	bool Gamepad::released(const GamepadButton& button)
	{
		return _buttonsReleased[button];
	}

	Vector2 Gamepad::axisLeft()
	{
		return _left;
	}

	Vector2 Gamepad::axisRight()
	{
		return _right;
	}

	float Gamepad::axisL2()
	{
		return _l2;
	}

	float Gamepad::axisR2()
	{
		return _r2;
	}

	void Gamepad::reset()
	{
		std::fill(std::begin(_buttonsPressed), std::end(_buttonsPressed), false);
		std::fill(std::begin(_buttonsReleased), std::end(_buttonsReleased), false);
	}

	void Gamepad::clearAll()
	{
		std::fill(std::begin(_buttonsPressed), std::end(_buttonsPressed), false);
		std::fill(std::begin(_buttons), std::end(_buttons), false);
		std::fill(std::begin(_buttonsReleased), std::end(_buttonsReleased), false);
	}

	void Gamepad::buttonChanged(GamepadButton const& button, bool const& pressed)
	{
		if (pressed)
		{
			_buttons[button] = true;
			_buttonsPressed[button] = true;
			_buttonsReleased[button] = false;
		}
		else
		{
			_buttons[button] = false;
			_buttonsPressed[button] = false;
			_buttonsReleased[button] = false;
		}
	}
}
