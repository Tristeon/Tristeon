#include "Mouse.h"

#include <Math/Math.h>
#include <Math/Vector2Int.h>

#include <QApplication>
#include <QCommonStyle>

#include "Window.h"

namespace Tristeon
{
	bool Mouse::buttons[Last];
	bool Mouse::buttonsPressed[Last];
	bool Mouse::buttonsReleased[Last];
	bool Mouse::buttonsDoubleClicked[Last];
	
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

	bool Mouse::doubleClicked(MouseButton const& button)
	{
		return buttonsDoubleClicked[button];
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

	void Mouse::onPress(QMouseEvent const& event)
	{
		int const index = Math::maskToIndex(event.button());
		buttonsPressed[index] = true;
		buttons[index] = true;
		buttonsReleased[index] = false;
	}

	void Mouse::onRelease(QMouseEvent const& event)
	{
		int const index = Math::maskToIndex(event.button());
		buttonsPressed[index] = false;
		buttons[index] = false;
		buttonsReleased[index] = true;
	}

	void Mouse::onDoubleClick(QMouseEvent const& event)
	{
		int const index = Math::maskToIndex(event.button());
		buttonsDoubleClicked[index] = true;
	}

	void Mouse::onMove(QMouseEvent const& event)
	{
		Vector2Int newPos = Vector2Int(event.pos().x(), Window::height() - event.pos().y());
		if (!Window::fullScreen())
			newPos.y += QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight);
		mouseDelta += newPos - mousePos;
		mousePos = newPos;
	}

	void Mouse::onScroll(QWheelEvent const& event)
	{
		scrollDelta += Vector2Int(event.angleDelta().x(), event.angleDelta().y());
	}

	void Mouse::reset()
	{
		std::fill(std::begin(buttonsPressed), std::end(buttonsPressed), false);
		std::fill(std::begin(buttonsReleased), std::end(buttonsReleased), false);
		std::fill(std::begin(buttonsDoubleClicked), std::end(buttonsDoubleClicked), false);

		mouseDelta = Vector2Int::zero();
		scrollDelta = Vector2Int::zero();
	}

	void Mouse::clearAll()
	{
		std::fill(std::begin(buttonsPressed), std::end(buttonsPressed), false);
		std::fill(std::begin(buttons), std::end(buttons), false);
		std::fill(std::begin(buttonsReleased), std::end(buttonsReleased), false);
		std::fill(std::begin(buttonsDoubleClicked), std::end(buttonsDoubleClicked), false);

		mouseDelta = Vector2Int::zero();
		scrollDelta = Vector2Int::zero();
	}
}
