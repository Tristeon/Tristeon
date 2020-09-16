#include "Mouse.h"

#include <Math/Math.h>
#include <Math/Vector2Int.h>

namespace Tristeon
{
	bool Mouse::_buttons[Last];
	bool Mouse::_buttonsPressed[Last];
	bool Mouse::_buttonsReleased[Last];
	
	Vector2Int Mouse::_mousePos{};
	Vector2Int Mouse::_mouseDelta{};
	Vector2Int Mouse::_scrollDelta{};
	
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

	Vector2Int Mouse::position()
	{
		return _mousePos;
	}

	Vector2Int Mouse::deltaPos()
	{
		return _mouseDelta;
	}

	Vector2Int Mouse::deltaScroll()
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

	void Mouse::onMove(const Vector2Int& pos)
	{
		_mouseDelta += pos - _mousePos;
		_mousePos = pos;
	}

	void Mouse::onScroll(const Vector2Int& change)
	{
		_scrollDelta += change;
	}

	void Mouse::reset()
	{
		std::fill(std::begin(_buttonsPressed), std::end(_buttonsPressed), false);
		std::fill(std::begin(_buttonsReleased), std::end(_buttonsReleased), false);

		_mouseDelta = Vector2Int::zero();
		_scrollDelta = Vector2Int::zero();
	}

	void Mouse::clearAll()
	{
		std::fill(std::begin(_buttonsPressed), std::end(_buttonsPressed), false);
		std::fill(std::begin(_buttons), std::end(_buttons), false);
		std::fill(std::begin(_buttonsReleased), std::end(_buttonsReleased), false);

		_mouseDelta = Vector2Int::zero();
		_scrollDelta = Vector2Int::zero();
	}
}