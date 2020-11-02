#include "Mouse.h"

#include <Math/Vector.h>

namespace Tristeon
{
	bool Mouse::_buttons[Last];
	bool Mouse::_buttonsPressed[Last];
	bool Mouse::_buttonsReleased[Last];
	
	VectorU Mouse::_mousePos{};
	VectorI Mouse::_mouseDelta{};
	VectorI Mouse::_scrollDelta{};
	
	bool Mouse::pressed(const MouseButton& button)
	{
		return _buttonsPressed[button];
	}

	bool Mouse::held(const MouseButton& button)
	{
		return _buttons[button];
	}

	bool Mouse::released(const MouseButton& button)
	{
		return _buttonsReleased[button];
	}

	VectorU Mouse::position()
	{
		return _mousePos;
	}

	VectorI Mouse::deltaPos()
	{
		return _mouseDelta;
	}

	VectorI Mouse::deltaScroll()
	{
		return _scrollDelta;
	}

	void Mouse::onPress(const MouseButton& button)
	{
		_buttonsPressed[button] = true;
		_buttons[button] = true;
		_buttonsReleased[button] = false;
	}

	void Mouse::onRelease(const MouseButton& button)
	{
		_buttonsPressed[button] = false;
		_buttons[button] = false;
		_buttonsReleased[button] = true;
	}

	void Mouse::onMove(const VectorU& pos)
	{
		_mouseDelta += (VectorI)pos - (VectorI)_mousePos;
		_mousePos = pos;
	}

	void Mouse::onScroll(const VectorI& change)
	{
		_scrollDelta += change;
	}

	void Mouse::reset()
	{
		std::fill(std::begin(_buttonsPressed), std::end(_buttonsPressed), false);
		std::fill(std::begin(_buttonsReleased), std::end(_buttonsReleased), false);

		_mouseDelta = VectorI::zero();
		_scrollDelta = VectorI::zero();
	}

	void Mouse::clearAll()
	{
		std::fill(std::begin(_buttonsPressed), std::end(_buttonsPressed), false);
		std::fill(std::begin(_buttons), std::end(_buttons), false);
		std::fill(std::begin(_buttonsReleased), std::end(_buttonsReleased), false);

		_mouseDelta = VectorI::zero();
		_scrollDelta = VectorI::zero();
	}
}