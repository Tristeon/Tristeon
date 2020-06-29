#ifdef TRISTEON_EDITOR
#include "EditorWindow.h"

#include <QApplication>
#include <QGamepad>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <qstyle.h>
#include <qloggingcategory.h>

#include <Utils/Colour.h>
#include <Math/Math.h>

#include <Input/Gamepad.h>
#include <Input/Keyboard.h>
#include <Input/Mouse.h>

#include <Editor/GameView.h>

#include <Scenes/Camera.h>
#include <Scenes/SceneManager.h>

using namespace Tristeon;

namespace TristeonEditor
{
	EditorWindow::EditorWindow()
	{
		setMouseTracking(true);
		setWindowIcon(QIcon("Internal/Icons/logo.png"));
		connectGamepads();

		oldMousePos = QCursor::pos();
	}

	void EditorWindow::_pollEvents()
	{
		QApplication::processEvents();
		
		//Mouse handling
		while (!mousePressEvents.empty())
		{
			Mouse::onPress(static_cast<Mouse::MouseButton>(Math::maskToIndex(mousePressEvents.front().button())));
			mousePressEvents.pop();
		}

		while (!mouseReleaseEvents.empty())
		{
			Mouse::onRelease(static_cast<Mouse::MouseButton>(Math::maskToIndex(mouseReleaseEvents.front().button())));
			mouseReleaseEvents.pop();
		}

		if (oldMousePos != QCursor::pos())
		{
			mouseMoveEvents.push(QMouseEvent(QEvent::MouseMove, QCursor::pos(), Qt::NoButton, Qt::NoButton,
				Qt::KeyboardModifier::NoModifier));
			oldMousePos = QCursor::pos();
		}
		while (!mouseMoveEvents.empty())
		{
			Vector2Int newPos = Vector2Int(mouseMoveEvents.front().pos().x(), Window::height() - mouseMoveEvents.front().pos().y());
			if (!Window::isFullscreen())
				newPos.y += QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight);
						
			Mouse::onMove(newPos);
			mouseMoveEvents.pop();
		}

		while (!mouseWheelEvents.empty())
		{
			Mouse::onScroll(Vector2Int(mouseWheelEvents.front().angleDelta().x(), mouseWheelEvents.front().angleDelta().y()));
			mouseWheelEvents.pop();
		}

		//Keyboard handling
		while (!keyPressEvents.empty())
		{
			Keyboard::onPress(static_cast<Keyboard::Key>(keyPressEvents.front().key()));
			keyPressEvents.pop();
		}

		while (!keyReleaseEvents.empty())
		{
			Keyboard::onRelease(static_cast<Keyboard::Key>(keyReleaseEvents.front().key()));
			keyReleaseEvents.pop();
		}
	}

	unsigned EditorWindow::_windowWidth()
	{
		return QMainWindow::width();
	}

	unsigned EditorWindow::_windowHeight()
	{
		return QMainWindow::height();
	}

	unsigned EditorWindow::_gameWidth()
	{
		return GameView::instance()->width();
	}

	unsigned EditorWindow::_gameHeight()
	{
		return GameView::instance()->height();
	}

	bool EditorWindow::_isFullscreen()
	{
		return QMainWindow::isFullScreen();
	}

	void EditorWindow::_setFullscreen(bool const& value)
	{
		if (value)
			QMainWindow::showFullScreen();
		else
			QMainWindow::showNormal();
	}

	void EditorWindow::_close()
	{
		QMainWindow::close();
		closing = true;
	}

	bool EditorWindow::_closingDown()
	{
		return closing;
	}

	Vector2 EditorWindow::_screenToWorld(Vector2Int const& screenPoint)
	{
		//Convert into Qt coords
		Vector2Int point = screenPoint;
		if (!isFullscreen())
			point.y -= QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight);
		point = Vector2Int(screenPoint.x, Window::height() - point.y);

		//Convert into local coords
		QPoint local = GameView::instance()->mapFromGlobal(QPoint(point.x, point.y));
		//Flip Y and remove area below gameview because apparently mapFromGlobal() doesnt handle that space?
		local.setY(Window::height() - (Window::height() - GameView::instance()->rect().bottom()) - local.y());

		//Scale with gameview screen 
		Vector2 const screenSize = Vector2(Window::width(), Window::height());
		Vector2 const viewSize = Vector2(GameView::instance()->width(), GameView::instance()->height());

		//Adjust for center
		Vector2 const halfSize = Vector2(GameView::instance()->width() / 2.0f, GameView::instance()->height() / 2.0f);

		Vector2 result = Vector2(local.x(), local.y()) - halfSize;
		//Adjust for camera
		result *= screenSize / viewSize;
		result *= 1.0f / Camera::main()->zoom;
		result += Camera::main()->position;
		return result;
	}

	Vector2Int EditorWindow::_worldToScreen(Vector2 const& worldPoint)
	{
		Vector2 const screenSize = Vector2(Window::width(), Window::height());
		Vector2 const viewSize = Vector2(GameView::instance()->width(), GameView::instance()->height());
		Vector2 const halfSize = Vector2(GameView::instance()->width() / 2.0f, GameView::instance()->height() / 2.0f);

		Vector2 point = worldPoint;
		point -= Camera::main()->position;
		point /= (1.0f / Camera::main()->zoom);
		point /= (screenSize / viewSize);

		point += halfSize;

		point.y = Window::height() - (Window::height() - GameView::instance()->rect().bottom()) - point.y;
		const QPoint global = GameView::instance()->mapToGlobal(QPoint(point.x, point.y));

		Vector2Int result = Vector2Int(global.x(), Window::height() - global.y());
		if (!isFullscreen())
			result.y += QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight);
		return result;
	}

	void EditorWindow::connectGamepads()
	{
		QLoggingCategory::setFilterRules(QStringLiteral("qt.gamepad.debug=true"));

		connect(QGamepadManager::instance(), &QGamepadManager::connectedGamepadsChanged, this,
			&EditorWindow::gamepadsChanged);
	}
	
	void EditorWindow::gamepadsChanged()
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
		connect(activeGamepad, &QGamepad::axisLeftYChanged, this,
			[](double const& value) { Gamepad::left.y = -value; });

		connect(activeGamepad, &QGamepad::axisRightXChanged, this,
			[](double const& value) { Gamepad::right.x = value; });
		connect(activeGamepad, &QGamepad::axisRightYChanged, this,
			[](double const& value) { Gamepad::right.y = -value; });

		connect(activeGamepad, &QGamepad::buttonAChanged, this, [](bool const& pressed)
			{
				Gamepad::buttonChanged(Gamepad::A, pressed);
			});
		connect(activeGamepad, &QGamepad::buttonBChanged, this, [](bool const& pressed)
			{
				Gamepad::buttonChanged(Gamepad::B, pressed);
			});
		connect(activeGamepad, &QGamepad::buttonXChanged, this, [](bool const& pressed)
			{
				Gamepad::buttonChanged(Gamepad::X, pressed);
			});
		connect(activeGamepad, &QGamepad::buttonYChanged, this, [](bool const& pressed)
			{
				Gamepad::buttonChanged(Gamepad::Y, pressed);
			});

		connect(activeGamepad, &QGamepad::buttonL1Changed, this, [](bool const& pressed)
			{
				Gamepad::buttonChanged(Gamepad::L1, pressed);
			});
		connect(activeGamepad, &QGamepad::buttonL2Changed, this, [](float const& value) { Gamepad::l2 = value; });
		connect(activeGamepad, &QGamepad::buttonL3Changed, this, [](bool const& pressed)
			{
				Gamepad::buttonChanged(Gamepad::L3, pressed);
			});

		connect(activeGamepad, &QGamepad::buttonR1Changed, this, [](bool const& pressed)
			{
				Gamepad::buttonChanged(Gamepad::R1, pressed);
			});
		connect(activeGamepad, &QGamepad::buttonR2Changed, this, [](float const& value) { Gamepad::r2 = value; });
		connect(activeGamepad, &QGamepad::buttonR3Changed, this, [](bool const& pressed)
			{
				Gamepad::buttonChanged(Gamepad::R3, pressed);
			});

		connect(activeGamepad, &QGamepad::buttonSelectChanged, this, [](bool const& pressed)
			{
				Gamepad::buttonChanged(Gamepad::Select, pressed);
			});
		connect(activeGamepad, &QGamepad::buttonStartChanged, this, [](bool const& pressed)
			{
				Gamepad::buttonChanged(Gamepad::Start, pressed);
			});
		connect(activeGamepad, &QGamepad::buttonGuideChanged, this, [](bool const& pressed)
			{
				Gamepad::buttonChanged(Gamepad::Guide, pressed);
			});
	}

	bool EditorWindow::event(QEvent* e)
	{
		switch (e->type())
		{
		case QEvent::WindowActivate:
			Keyboard::clearAll();
			Mouse::clearAll();
			Gamepad::clearAll();
			break;

		case QEvent::WindowDeactivate:
			Keyboard::clearAll();
			Mouse::clearAll();
			Gamepad::clearAll();
			break;
		};
		return QMainWindow::event(e);
	}

	void EditorWindow::mousePressEvent(QMouseEvent* event)
	{
		mousePressEvents.push(*event);
	}

	void EditorWindow::mouseReleaseEvent(QMouseEvent* event)
	{
		mouseReleaseEvents.push(*event);
	}

	void EditorWindow::wheelEvent(QWheelEvent* event)
	{
		mouseWheelEvents.push(*event);
	}

	void EditorWindow::keyPressEvent(QKeyEvent* event)
	{
		if (event->isAutoRepeat())
			return;
		keyPressEvents.push(*event);
	}

	void EditorWindow::keyReleaseEvent(QKeyEvent* event)
	{
		if (event->isAutoRepeat())
			return;

		keyReleaseEvents.push(*event);
	}

	void EditorWindow::closeEvent(QCloseEvent* event)
	{
		SceneManager::reset();
		QApplication::quit();
		exit(0);
	}

	void EditorWindow::_setClearColour(Colour const& colour)
	{
		QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
		f->glClearColor(colour.r, colour.g, colour.b, colour.a);
	}

	void EditorWindow::_draw()
	{
		GameView::instance()->update();
		QApplication::sendPostedEvents();
	}

	void EditorWindow::_setWindowTitle(std::string const& value)
	{
		QMainWindow::setWindowTitle(QString::fromStdString(value));
	}
}
#endif