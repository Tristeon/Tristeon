#include "Window.h"

#include <QApplication>

#include "Input/Mouse.h"
#include <QMouseEvent>

#include "Input/Keyboard.h"

namespace Tristeon
{
	Window* Window::instance = nullptr;

	Window::Window()
	{
		resize(1600, 960);
		instance = this;
	}

	Window::~Window()
	{
		instance = nullptr;
	}

	Window* Window::main()
	{
		return instance;
	}

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

	void Window::focusInEvent(QFocusEvent* event)
	{
		
	}

	void Window::focusOutEvent(QFocusEvent* event)
	{
		
	}

	void Window::moveEvent(QMoveEvent* event)
	{
		
	}

	void Window::resizeEvent(QResizeEvent* event)
	{
		
	}

	void Window::closeEvent(QCloseEvent* event)
	{
		QApplication::quit();
		exit(0);
	}
}
