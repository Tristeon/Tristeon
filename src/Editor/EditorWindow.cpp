#ifdef TRISTEON_EDITOR
#include "Rendering/Renderer.h"
#include "Rendering/Camera.h"
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
			Mouse::onPress(static_cast<Mouse::MouseButton>(mapToTristeonButton(mousePressEvents.front().button())));
			mousePressEvents.pop();
		}

		while (!mouseReleaseEvents.empty())
		{
			Mouse::onRelease(static_cast<Mouse::MouseButton>(mapToTristeonButton(mouseReleaseEvents.front().button())));
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
			VectorU newPos = VectorU((unsigned int)mouseMoveEvents.front().pos().x(), Window::height() - (unsigned int)mouseMoveEvents.front().pos().y());
			//if (!Window::fullScreen())
				newPos.y += QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight);
						
			Mouse::onMove(newPos);
			mouseMoveEvents.pop();
		}

		while (!mouseWheelEvents.empty())
		{
			Mouse::onScroll(VectorI(mouseWheelEvents.front().angleDelta().x(), mouseWheelEvents.front().angleDelta().y()));
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

	void EditorWindow::_setVsync(const bool& value)
	{
		auto f = GameView::instance()->format();
		f.setSwapInterval(value);
		GameView::instance()->setFormat(f);
	}

	void EditorWindow::_setWindowMode(const Tristeon::Settings::Graphics::WindowMode& mode)
	{
		//Do nothing because the editor window should always stay windowed
	}

	void EditorWindow::_setResolution(const Tristeon::VectorU& resolution)
	{
		QMainWindow::resize(resolution.x, resolution.y);
	}

	void EditorWindow::_setDisplay(const unsigned& display)
	{
		//Not relevant for the editor window
	}

	unsigned EditorWindow::_currentDisplay()
	{
		return 0;
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

	Vector EditorWindow::_screenToWorld(VectorU const& screenPoint, Camera* camera)
	{
		//Convert into Qt coords
		VectorU point = screenPoint;
		//if (!fullScreen())
			point.y -= QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight);
		point = VectorI(screenPoint.x, Window::height() - point.y);

		//Convert into local coords
		QPoint local = GameView::instance()->mapFromGlobal(QPoint(point.x, point.y));
		//Flip Y and remove area below gameview because apparently mapFromGlobal() doesnt handle that space?
		local.setY(Window::height() - (Window::height() - GameView::instance()->rect().bottom()) - local.y());

		//Adjust for camera position on screen
		Vector result = Vector(local.x(), local.y());
		result -= ((camera->screenCoordinates + Vector::one()) / 2.0f)* Window::gameSize();
		result -= (Vector)Window::gameSize() * camera->screenSize / 2.0f; //Adjust for center
		
		//Adjust for camera
		result *= 1.0f / camera->zoom;
		result += (VectorI)camera->position;
		return result;
	}

	VectorU EditorWindow::_worldToScreen(Vector const& worldPoint, Camera* camera)
	{
		Vector point = worldPoint;
		point -= (VectorI)camera->position;
		point /= (1.0f / camera->zoom);

		point += ((camera->screenCoordinates + Vector::one()) / 2.0f) * Window::gameSize();
		point += (Vector)Window::gameSize() * camera->screenSize / 2.0f; //Adjust for center

		point.y = Window::height() - (Window::height() - GameView::instance()->rect().bottom()) - point.y;
		const QPoint global = GameView::instance()->mapToGlobal(QPoint(point.x, point.y));

		VectorU result = VectorU((unsigned int)global.x(), Window::height() - (unsigned int)global.y());
		//if (!fullScreen())
			result.y += QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight);
		return result;
	}

	Keyboard::Key EditorWindow::mapToTristeonKey(const Qt::Key& key) const
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

		case Qt::Key::Key_QuoteLeft: return Keyboard::GraveAccent;

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
			
		case Qt::Key::Key_Return: return Keyboard::Enter;
			
		case Qt::Key::Key_Shift: return Keyboard::Shift;
		case Qt::Key::Key_Control: return Keyboard::Control;
		case Qt::Key::Key_Alt: return Keyboard::Alt;
		case Qt::Key::Key_Menu: return Keyboard::Menu;

		case Qt::Key::Key_Super_L: return Keyboard::LeftSuper;
		case Qt::Key::Key_Super_R: return Keyboard::RightSuper;
		}

		return Keyboard::Unknown;
	}

	Tristeon::Mouse::MouseButton EditorWindow::mapToTristeonButton(const Qt::MouseButton& button) const
	{
		switch(button)
		{
		case Qt::NoButton: return Mouse::Unknown;
		case Qt::LeftButton: return Mouse::Left;
		case Qt::RightButton: return Mouse::Right;
		case Qt::MidButton: return Mouse::Middle;
		case Qt::BackButton: return Mouse::Back;
		case Qt::ForwardButton: return Mouse::Forward;
		case Qt::TaskButton: return Mouse::Task;
		case Qt::ExtraButton4: return Mouse::Extra4;
		default: TRISTEON_WARNING("Invalid/non-covered mouse button: " + std::to_string(button));
		}

		return Mouse::Unknown;
	}

	void EditorWindow::connectGamepads()
	{
		QLoggingCategory::setFilterRules(QStringLiteral("qt.gamepad.debug=true"));

		connect(QGamepadManager::instance(), &QGamepadManager::gamepadConnected, this, [](int deviceID)
		{
			if (deviceID < Gamepad::maximumGamepads)
			{
				Gamepad::gamepads[deviceID]._connected = true;
				Gamepad::gamepads[deviceID]._name = QGamepadManager::instance()->gamepadName(deviceID).toStdString();
				TRISTEON_LOG("Gamepad connected: " + Gamepad::name(deviceID));

				Gamepad::clearGamepad(deviceID);
			}
		});

		connect(QGamepadManager::instance(), &QGamepadManager::gamepadDisconnected, this, [](int deviceID)
		{
			if (deviceID < Gamepad::maximumGamepads)
			{
				Gamepad::gamepads[deviceID]._connected = false;
				TRISTEON_LOG("Gamepad disconnected: " + Gamepad::name(deviceID));

				Gamepad::clearGamepad(deviceID);
			}
		});

		bindGamepads();
	}
	
	void EditorWindow::bindGamepads()
	{
		for (unsigned int i = 0; i < Gamepad::maximumGamepads; i++)
		{
			gamepads[i].setDeviceId(i);
			
			connect(&gamepads[i], &QGamepad::axisLeftXChanged, this, [=](double const& value) { Gamepad::gamepads[i]._left.x = value; });
			connect(&gamepads[i], &QGamepad::axisLeftYChanged, this,
				[=](double const& value) { Gamepad::gamepads[i]._left.y = -value; });

			connect(&gamepads[i], &QGamepad::axisRightXChanged, this,
				[=](double const& value) { Gamepad::gamepads[i]._right.x = value; });
			connect(&gamepads[i], &QGamepad::axisRightYChanged, this,
				[=](double const& value) { Gamepad::gamepads[i]._right.y = -value; });

			connect(&gamepads[i], &QGamepad::buttonAChanged, this, [=](bool const& pressed)
				{
					Gamepad::buttonChanged(i, Gamepad::A, pressed);
				});
			connect(&gamepads[i], &QGamepad::buttonBChanged, this, [=](bool const& pressed)
				{
					Gamepad::buttonChanged(i, Gamepad::B, pressed);
				});
			connect(&gamepads[i], &QGamepad::buttonXChanged, this, [=](bool const& pressed)
				{
					Gamepad::buttonChanged(i, Gamepad::X, pressed);
				});
			connect(&gamepads[i], &QGamepad::buttonYChanged, this, [=](bool const& pressed)
				{
					Gamepad::buttonChanged(i, Gamepad::Y, pressed);
				});

			connect(&gamepads[i], &QGamepad::buttonL1Changed, this, [=](bool const& pressed)
				{
					Gamepad::buttonChanged(i, Gamepad::L1, pressed);
				});
			connect(&gamepads[i], &QGamepad::buttonL2Changed, this, [=](float const& value) { Gamepad::gamepads[i]._l2 = value; });
			connect(&gamepads[i], &QGamepad::buttonL3Changed, this, [=](bool const& pressed)
				{
					Gamepad::buttonChanged(i, Gamepad::L3, pressed);
				});

			connect(&gamepads[i], &QGamepad::buttonR1Changed, this, [=](bool const& pressed)
				{
					Gamepad::buttonChanged(i, Gamepad::R1, pressed);
				});
			connect(&gamepads[i], &QGamepad::buttonR2Changed, this, [=](float const& value) { Gamepad::gamepads[i]._r2 = value; });
			connect(&gamepads[i], &QGamepad::buttonR3Changed, this, [=](bool const& pressed)
				{
					Gamepad::buttonChanged(i, Gamepad::R3, pressed);
				});

			connect(&gamepads[i], &QGamepad::buttonSelectChanged, this, [=](bool const& pressed)
				{
					Gamepad::buttonChanged(i, Gamepad::Select, pressed);
				});
			connect(&gamepads[i], &QGamepad::buttonStartChanged, this, [=](bool const& pressed)
				{
					Gamepad::buttonChanged(i, Gamepad::Start, pressed);
				});
			connect(&gamepads[i], &QGamepad::buttonGuideChanged, this, [=](bool const& pressed)
				{
					Gamepad::buttonChanged(i, Gamepad::Guide, pressed);
				});

			connect(&gamepads[i], &QGamepad::buttonLeftChanged, this, [=](bool const& pressed)
				{
					Gamepad::buttonChanged(i, Gamepad::Left, pressed);
				});
			connect(&gamepads[i], &QGamepad::buttonRightChanged, this, [=](bool const& pressed)
				{
					Gamepad::buttonChanged(i, Gamepad::Right, pressed);
				});
			connect(&gamepads[i], &QGamepad::buttonUpChanged, this, [=](bool const& pressed)
				{
					Gamepad::buttonChanged(i, Gamepad::Up, pressed);
				});
			connect(&gamepads[i], &QGamepad::buttonDownChanged, this, [=](bool const& pressed)
				{
					Gamepad::buttonChanged(i, Gamepad::Down, pressed);
				});
		}
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