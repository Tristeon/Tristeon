#include "Window.h"

#include <Input/Keyboard.h>
#include <Input/Mouse.h>
#include <Input/Gamepad.h>
#include <iostream>

#include <QApplication>
#include <QMouseEvent>
#include <QtGamepad/QtGamepad>

#include <Scenes/SceneManager.h>

namespace Tristeon
{
	void Window::pollEvents()
	{
		//Mouse handling
		while (!mousePressEvents.empty())
		{
			Mouse::onPress(mousePressEvents.front());
			mousePressEvents.pop();
		}

		while (!mouseReleaseEvents.empty())
		{
			Mouse::onRelease(mouseReleaseEvents.front());
			mouseReleaseEvents.pop();
		}

		while (!mouseDoubleClickEvents.empty())
		{
			Mouse::onDoubleClick(mouseDoubleClickEvents.front());
			mouseDoubleClickEvents.pop();
		}

		while (!mouseMoveEvents.empty())
		{
			Mouse::onMove(mouseMoveEvents.front());
			mouseMoveEvents.pop();
		}

		while (!mouseWheelEvents.empty())
		{
			Mouse::onScroll(mouseWheelEvents.front());
			mouseWheelEvents.pop();
		}

		//Keyboard handling
		while (!keyPressEvents.empty())
		{
			Keyboard::onPress(keyPressEvents.front());
			keyPressEvents.pop();
		}

		while (!keyReleaseEvents.empty())
		{
			Keyboard::onRelease(keyReleaseEvents.front());
			keyReleaseEvents.pop();
		}
	}

	void Window::connectGamepads()
	{
		QLoggingCategory::setFilterRules(QStringLiteral("qt.gamepad.debug=true"));

		connect(QGamepadManager::instance(), &QGamepadManager::connectedGamepadsChanged, this, &Window::gamepadsChanged);
	}

	Window::Window()
	{
		connectGamepads();
	}

	void Window::mousePressEvent(QMouseEvent* event)
	{
		mousePressEvents.push(*event);
	}

	void Window::mouseReleaseEvent(QMouseEvent* event)
	{
		mouseReleaseEvents.push(*event);
	}

	void Window::mouseDoubleClickEvent(QMouseEvent* event)
	{
		mouseDoubleClickEvents.push(*event);
	}

	void Window::mouseMoveEvent(QMouseEvent* event)
	{
		mouseMoveEvents.push(*event);
	}

	void Window::wheelEvent(QWheelEvent* event)
	{
		mouseWheelEvents.push(*event);
	}

	void Window::keyPressEvent(QKeyEvent* event)
	{
		if (event->isAutoRepeat())
			return;
		keyPressEvents.push(*event);
	}

	void Window::keyReleaseEvent(QKeyEvent* event)
	{
		if (event->isAutoRepeat())
			return;
		
		keyReleaseEvents.push(*event);
	}

	void Window::closeEvent(QCloseEvent* event)
	{
		SceneManager::reset();
		QApplication::quit();
		exit(0);
	}

	void Window::gamepadsChanged()
	{
		auto gamepads = QGamepadManager::instance()->connectedGamepads();
		if (gamepads.isEmpty())
		{
			std::cout << "Gamepad disconnected: " << activeGamepad->name().toStdString() << std::endl;
			
			activeGamepad->deleteLater();
			activeGamepad = nullptr;

			return;
		}

		//Keep existing
		if (activeGamepad != nullptr && activeGamepad->deviceId() == gamepads[0])
			return;

		if (activeGamepad != nullptr)
		{
			activeGamepad->deleteLater();
			std::cout << "Deselected Gamepad: " << activeGamepad->name().toStdString() << std::endl;
		}

		activeGamepad = new QGamepad(gamepads[0]);
		std::cout << "Selected Gamepad: " << activeGamepad->name().toStdString() << std::endl;
		
		connect(activeGamepad, &QGamepad::axisLeftXChanged, this, [](double const& value) { Gamepad::left.x = value; });
		connect(activeGamepad, &QGamepad::axisLeftYChanged, this, [](double const& value) { Gamepad::left.y = -value; });

		connect(activeGamepad, &QGamepad::axisRightXChanged, this, [](double const& value) { Gamepad::right.x = value; });
		connect(activeGamepad, &QGamepad::axisRightYChanged, this, [](double const& value) { Gamepad::right.y = -value; });

		connect(activeGamepad, &QGamepad::buttonAChanged, this, [](bool const& pressed) { Gamepad::buttonChanged(Gamepad::A, pressed); });
		connect(activeGamepad, &QGamepad::buttonBChanged, this, [](bool const& pressed) { Gamepad::buttonChanged(Gamepad::B, pressed); });
		connect(activeGamepad, &QGamepad::buttonXChanged, this, [](bool const& pressed) { Gamepad::buttonChanged(Gamepad::X, pressed); });
		connect(activeGamepad, &QGamepad::buttonYChanged, this, [](bool const& pressed) { Gamepad::buttonChanged(Gamepad::Y, pressed); });

		connect(activeGamepad, &QGamepad::buttonL1Changed, this, [](bool const& pressed) { Gamepad::buttonChanged(Gamepad::L1, pressed); });
		connect(activeGamepad, &QGamepad::buttonL2Changed, this, [](float const& value) { Gamepad::l2 = value; });
		connect(activeGamepad, &QGamepad::buttonL3Changed, this, [](bool const& pressed) { Gamepad::buttonChanged(Gamepad::L3, pressed); });

		connect(activeGamepad, &QGamepad::buttonR1Changed, this, [](bool const& pressed) { Gamepad::buttonChanged(Gamepad::R1, pressed); });
		connect(activeGamepad, &QGamepad::buttonR2Changed, this, [](float const& value) { Gamepad::r2 = value; });
		connect(activeGamepad, &QGamepad::buttonR3Changed, this, [](bool const& pressed) { Gamepad::buttonChanged(Gamepad::R3, pressed); });

		connect(activeGamepad, &QGamepad::buttonSelectChanged, this, [](bool const& pressed) { Gamepad::buttonChanged(Gamepad::Select, pressed); });
		connect(activeGamepad, &QGamepad::buttonStartChanged, this, [](bool const& pressed) { Gamepad::buttonChanged(Gamepad::Start, pressed); });
		connect(activeGamepad, &QGamepad::buttonGuideChanged, this, [](bool const& pressed) { Gamepad::buttonChanged(Gamepad::Guide, pressed); });
	}
}