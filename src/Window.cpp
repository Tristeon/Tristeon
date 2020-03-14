#include "Window.h"

#include <QApplication>

#include "Input/Mouse.h"
#include <QMouseEvent>
#include <QSurfaceFormat>


#include "Input/Keyboard.h"
#include "Scenes/SceneManager.h"

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

	void Window::closeEvent(QCloseEvent* event)
	{
		SceneManager::reset();
		QApplication::quit();
		exit(0);
	}
}
