#include "Mouse.h"

#include "Math/Math.h"
#include "Math/Vector2Int.h"

namespace Tristeon
{
	bool Mouse::buttons[MaxMouseButton];
	bool Mouse::buttonsPressed[MaxMouseButton];
	bool Mouse::buttonsReleased[MaxMouseButton];
	bool Mouse::buttonsDoubleClicked[MaxMouseButton];
	Vector2Int Mouse::mousePos = Vector2Int::zero();
	
	bool Mouse::pressed(MouseButton button)
	{
		return buttonsPressed[button];
	}

	bool Mouse::held(MouseButton button)
	{
		return buttons[button];
	}

	bool Mouse::released(MouseButton button)
	{
		return buttonsReleased[button];
	}

	bool Mouse::doubleClicked(MouseButton button)
	{
		return buttonsDoubleClicked[button];
	}

	Vector2 Mouse::position()
	{
		return mousePos;
	}

	void Mouse::onPress(QMouseEvent event)
	{
		int const index = Math::maskToIndex(event.button());
		buttonsPressed[index] = true;
		buttons[index] = true;
		buttonsReleased[index] = false;
	}

	void Mouse::onRelease(QMouseEvent event)
	{
		int const index = Math::maskToIndex(event.button());
		buttonsPressed[index] = false;
		buttons[index] = false;
		buttonsReleased[index] = true;
	}

	void Mouse::onDoubleClick(QMouseEvent event)
	{
		int const index = Math::maskToIndex(event.button());
		buttonsDoubleClicked[index] = true;
	}

	void Mouse::onMove(QMouseEvent event)
	{
		mousePos = Vector2Int(event.pos().x(), event.pos().y());
	}

	void Mouse::reset()
	{
		std::fill(std::begin(buttonsPressed), std::end(buttonsPressed), false);
		std::fill(std::begin(buttonsReleased), std::end(buttonsReleased), false);
		std::fill(std::begin(buttonsDoubleClicked), std::end(buttonsDoubleClicked), false);
	}
}
