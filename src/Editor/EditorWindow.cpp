#ifdef TRISTEON_EDITOR
#include "Rendering/Renderer.h"
#include "Actors/Camera.h"
#include <qsettings.h>
#include <qshortcut.h>
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

		QShortcut* shortcut = new QShortcut(QKeySequence(tr("Ctrl+L", "Layout|Save")), this);
		connect(shortcut, &QShortcut::activated, this, [&]() 
			{
				QSettings settings("Tristeon", "Tristeon2D");
				settings.setValue("geometry", saveGeometry());
				settings.setValue("windowState", saveState());
			});
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
			Keyboard::onPress(static_cast<Keyboard::Key>(mapToTristeonKey((Qt::Key) keyPressEvents.front().key())));
			keyPressEvents.pop();
		}

		while (!keyReleaseEvents.empty())
		{
			Keyboard::onRelease(static_cast<Keyboard::Key>(mapToTristeonKey((Qt::Key)keyReleaseEvents.front().key())));
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

		//Adjust for center
		Vector2 const halfSize = Vector2(GameView::instance()->width() / 2.0f, GameView::instance()->height() / 2.0f);

		Vector2 result = Vector2(local.x(), local.y()) - halfSize;
		//Adjust for camera
		result *= 1.0f / Renderer::editorCamera()->zoom;
		result += (Vector2Int)Renderer::editorCamera()->position;
		return result;
	}

	Vector2Int EditorWindow::_worldToScreen(Vector2 const& worldPoint)
	{
		Vector2 const halfSize = Vector2(GameView::instance()->width() / 2.0f, GameView::instance()->height() / 2.0f);

		Vector2 point = worldPoint;
		point -= (Vector2Int)Renderer::editorCamera()->position;
		point /= (1.0f / Renderer::editorCamera()->zoom);

		point += halfSize;

		point.y = Window::height() - (Window::height() - GameView::instance()->rect().bottom()) - point.y;
		const QPoint global = GameView::instance()->mapToGlobal(QPoint(point.x, point.y));

		Vector2Int result = Vector2Int(global.x(), Window::height() - global.y());
		if (!isFullscreen())
			result.y += QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight);
		return result;
	}

	Keyboard::Key EditorWindow::mapToTristeonKey(Qt::Key key)
	{
		switch(key)
		{
		case Qt::Key::Key_unknown: return Keyboard::Unknown;
		case Qt::Key::Key_Space: return Keyboard::Space;
		case Qt::Key::Key_Apostrophe: return Keyboard::Apostrophe;
		case Qt::Key::Key_Comma: return Keyboard::Comma;
		case Qt::Key::Key_Minus: return Keyboard::Minus;
		case Qt::Key::Key_Period: return Keyboard::Period;
		case Qt::Key::Key_Slash: return Keyboard::Slash;

		case Qt::Key::Key_0: return Keyboard::Number0;
		case Qt::Key::Key_1: return Keyboard::Number1;
		case Qt::Key::Key_2: return Keyboard::Number2;
		case Qt::Key::Key_3: return Keyboard::Number3;
		case Qt::Key::Key_4: return Keyboard::Number4;
		case Qt::Key::Key_5: return Keyboard::Number5;
		case Qt::Key::Key_6: return Keyboard::Number6;
		case Qt::Key::Key_7: return Keyboard::Number7;
		case Qt::Key::Key_8: return Keyboard::Number8;
		case Qt::Key::Key_9: return Keyboard::Number9;

		case Qt::Key::Key_Semicolon: return Keyboard::Semicolon;
		case Qt::Key::Key_Equal: return Keyboard::Equals;

		case Qt::Key::Key_A: return Keyboard::A;
		case Qt::Key::Key_B: return Keyboard::B;
		case Qt::Key::Key_C: return Keyboard::C;
		case Qt::Key::Key_D: return Keyboard::D;
		case Qt::Key::Key_E: return Keyboard::E;
		case Qt::Key::Key_F: return Keyboard::F;
		case Qt::Key::Key_G: return Keyboard::G;
		case Qt::Key::Key_H: return Keyboard::H;
		case Qt::Key::Key_I: return Keyboard::I;
		case Qt::Key::Key_J: return Keyboard::J;
		case Qt::Key::Key_K: return Keyboard::K;
		case Qt::Key::Key_L: return Keyboard::L;
		case Qt::Key::Key_M: return Keyboard::M;
		case Qt::Key::Key_N: return Keyboard::N;
		case Qt::Key::Key_O: return Keyboard::O;
		case Qt::Key::Key_P: return Keyboard::P;
		case Qt::Key::Key_Q: return Keyboard::Q;
		case Qt::Key::Key_R: return Keyboard::R;
		case Qt::Key::Key_S: return Keyboard::S;
		case Qt::Key::Key_T: return Keyboard::T;
		case Qt::Key::Key_U: return Keyboard::U;
		case Qt::Key::Key_V: return Keyboard::V;
		case Qt::Key::Key_W: return Keyboard::W;
		case Qt::Key::Key_X: return Keyboard::X;
		case Qt::Key::Key_Y: return Keyboard::Y;
		case Qt::Key::Key_Z: return Keyboard::Z;

		case Qt::Key::Key_BracketLeft: return Keyboard::LeftBracket;
		case Qt::Key::Key_Backslash: return Keyboard::Backslash;
		case Qt::Key::Key_BracketRight: return Keyboard::RightBracket;
		//case Qt::Key::Key_GraveAccent: return Keyboard::GraveAccent; //TODO: Qt Key Grave binding

		case Qt::Key::Key_Escape: return Keyboard::Escape;
		case Qt::Key::Key_Enter: return Keyboard::Enter;
		case Qt::Key::Key_Tab: return Keyboard::Tab;
		case Qt::Key::Key_Backspace: return Keyboard::Backspace;
		case Qt::Key::Key_Insert: return Keyboard::Insert;
		case Qt::Key::Key_Delete: return Keyboard::Delete;

		case Qt::Key::Key_Left: return Keyboard::Left;
		case Qt::Key::Key_Right: return Keyboard::Right;
		case Qt::Key::Key_Up: return Keyboard::Up;
		case Qt::Key::Key_Down: return Keyboard::Down;

		case Qt::Key::Key_PageUp: return Keyboard::PageUp;
		case Qt::Key::Key_PageDown: return Keyboard::PageDown;

		case Qt::Key::Key_Home: return Keyboard::Home;
		case Qt::Key::Key_End: return Keyboard::End;

		case Qt::Key::Key_CapsLock: return Keyboard::CapsLock;
		case Qt::Key::Key_ScrollLock: return Keyboard::ScrollLock;
		case Qt::Key::Key_NumLock: return Keyboard::NumLock;

		case Qt::Key::Key_Print: return Keyboard::PrintScreen;
		case Qt::Key::Key_Pause: return Keyboard::Pause;

		case Qt::Key::Key_F1: return Keyboard::F1;
		case Qt::Key::Key_F2: return Keyboard::F2;
		case Qt::Key::Key_F3: return Keyboard::F3;
		case Qt::Key::Key_F4: return Keyboard::F4;
		case Qt::Key::Key_F5: return Keyboard::F5;
		case Qt::Key::Key_F6: return Keyboard::F6;
		case Qt::Key::Key_F7: return Keyboard::F7;
		case Qt::Key::Key_F8: return Keyboard::F8;
		case Qt::Key::Key_F9: return Keyboard::F9;
		case Qt::Key::Key_F10: return Keyboard::F10;
		case Qt::Key::Key_F11: return Keyboard::F11;
		case Qt::Key::Key_F12: return Keyboard::F12;
		case Qt::Key::Key_F13: return Keyboard::F13;
		case Qt::Key::Key_F14: return Keyboard::F14;
		case Qt::Key::Key_F15: return Keyboard::F15;
		case Qt::Key::Key_F16: return Keyboard::F16;
		case Qt::Key::Key_F17: return Keyboard::F17;
		case Qt::Key::Key_F18: return Keyboard::F18;
		case Qt::Key::Key_F19: return Keyboard::F19;
		case Qt::Key::Key_F20: return Keyboard::F20;
		case Qt::Key::Key_F21: return Keyboard::F21;
		case Qt::Key::Key_F22: return Keyboard::F22;
		case Qt::Key::Key_F23: return Keyboard::F23;
		case Qt::Key::Key_F24: return Keyboard::F24;
		case Qt::Key::Key_F25: return Keyboard::F25;

		case Qt::Key::Key_Shift: return Keyboard::Shift;
		case Qt::Key::Key_Control: return Keyboard::Control;
		case Qt::Key::Key_Alt: return Keyboard::Alt;
		case Qt::Key::Key_Menu: return Keyboard::Menu;

		case Qt::Key::Key_Super_L: return Keyboard::LeftSuper;
		case Qt::Key::Key_Super_R: return Keyboard::RightSuper;
		}

		return Keyboard::Unknown;
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
			Console::write("Gamepad disconnected: " + activeGamepad->name().toStdString());

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
			Console::write("Deselected Gamepad: " + activeGamepad->name().toStdString());
		}

		activeGamepad = new QGamepad(gamepads[0]);
		Console::write("Selected Gamepad: " + activeGamepad->name().toStdString());

		connect(activeGamepad, &QGamepad::axisLeftXChanged, this, [](double const& value) { Gamepad::_left.x = value; });
		connect(activeGamepad, &QGamepad::axisLeftYChanged, this,
			[](double const& value) { Gamepad::_left.y = -value; });

		connect(activeGamepad, &QGamepad::axisRightXChanged, this,
			[](double const& value) { Gamepad::_right.x = value; });
		connect(activeGamepad, &QGamepad::axisRightYChanged, this,
			[](double const& value) { Gamepad::_right.y = -value; });

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
		connect(activeGamepad, &QGamepad::buttonL2Changed, this, [](float const& value) { Gamepad::_l2 = value; });
		connect(activeGamepad, &QGamepad::buttonL3Changed, this, [](bool const& pressed)
			{
				Gamepad::buttonChanged(Gamepad::L3, pressed);
			});

		connect(activeGamepad, &QGamepad::buttonR1Changed, this, [](bool const& pressed)
			{
				Gamepad::buttonChanged(Gamepad::R1, pressed);
			});
		connect(activeGamepad, &QGamepad::buttonR2Changed, this, [](float const& value) { Gamepad::_r2 = value; });
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