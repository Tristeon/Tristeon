#include "Mouse.h"

#include <Math/Math.h>
#include <Math/Vector2Int.h>

namespace Tristeon
{
	bool Mouse::buttons[Last];
	bool Mouse::buttonsPressed[Last];
	bool Mouse::buttonsReleased[Last];
	
	Vector2Int Mouse::mousePos = Vector2Int::zero();
	Vector2Int Mouse::mouseDelta = Vector2Int::zero();
	Vector2Int Mouse::scrollDelta = Vector2Int::zero();
	
	bool Mouse::pressed(MouseButton const& button)
	{
		return buttonsPressed[button];
	}

	bool Mouse::held(MouseButton const& button)
	{
		return buttons[button];
	}

	bool Mouse::released(MouseButton const& button)
	{
		return buttonsReleased[button];
	}

	Vector2Int Mouse::position()
	{
		return mousePos;
	}

	Vector2Int Mouse::deltaPos()
	{
		return mouseDelta;
	}

	Vector2Int Mouse::deltaScroll()
	{
		return scrollDelta;
	}

	void Mouse::onPress(MouseButton const& button)
	{
		buttonsPressed[button] = true;
		buttons[button] = true;
		buttonsReleased[button] = false;
	}

	void Mouse::onRelease(MouseButton const& button)
	{
		buttonsPressed[button] = false;
		buttons[button] = false;
		buttonsReleased[button] = true;
	}

	void Mouse::onMove(Vector2Int const& pos)
	{
		//Vector2Int newPos = Vector2Int(event.pos().x(), Window::height() - event.pos().y());
		//if (!Window::isFullscreen())
		//	newPos.y += QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight);

		mouseDelta += pos - mousePos;
		mousePos = pos;
	}

	void Mouse::onScroll(Vector2Int const& change)
	{
		scrollDelta += change;
	}

	void Mouse::reset()
	{
		std::fill(std::begin(buttonsPressed), std::end(buttonsPressed), false);
		std::fill(std::begin(buttonsReleased), std::end(buttonsReleased), false);

		mouseDelta = Vector2Int::zero();
		scrollDelta = Vector2Int::zero();
	}

	void Mouse::clearAll()
	{
		std::fill(std::begin(buttonsPressed), std::end(buttonsPressed), false);
		std::fill(std::begin(buttons), std::end(buttons), false);
		std::fill(std::begin(buttonsReleased), std::end(buttonsReleased), false);

		mouseDelta = Vector2Int::zero();
		scrollDelta = Vector2Int::zero();
	}
}
