#pragma once
#ifdef TRISTEON_EDITOR
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "GameView.h"
#include <queue>
#include <Window.h>

#include <QMainWindow>
#include <QGamepad>

#include <QKeyEvent>

namespace TristeonEditor
{
	class EditorWindow : public Tristeon::Window, public QMainWindow
	{
	public:
		EditorWindow();
	protected:
		bool event(QEvent* e) override;
		void mousePressEvent(QMouseEvent* event) override;
		void mouseReleaseEvent(QMouseEvent* event) override;
		void wheelEvent(QWheelEvent* event) override;
		void keyPressEvent(QKeyEvent* event) override;
		void keyReleaseEvent(QKeyEvent* event) override;
		void closeEvent(QCloseEvent* event) override;
		void bindGamepads();

		void _pollEvents() override;

		unsigned int _windowWidth() override;
		unsigned int _windowHeight() override;
		unsigned int _gameWidth() override;
		unsigned int _gameHeight() override;

		void _setVsync(const bool& value) override;
		void _setWindowMode(const Tristeon::Settings::Graphics::WindowMode& mode) override;
		void _setResolution(const Tristeon::VectorU& resolution) override;
		void _setDisplay(const unsigned& display) override;
		unsigned _currentDisplay() override;

		void _close() override;
		bool _closingDown() override;

		void _setClearColour(Tristeon::Colour const& colour) override;
		void _draw() override;

		void _setWindowTitle(std::string const& value) override;
		
		Tristeon::Vector _screenToWorld(Tristeon::VectorU const& screenPoint, Tristeon::Camera* camera) override;
		Tristeon::VectorU _worldToScreen(Tristeon::Vector const& worldPoint, Tristeon::Camera* camera) override;

		Tristeon::Keyboard::Key mapToTristeonKey(const Qt::Key& key) const;
		Tristeon::Mouse::MouseButton mapToTristeonButton(const Qt::MouseButton& button) const;
	private:
		std::queue<QMouseEvent> mousePressEvents;
		std::queue<QMouseEvent> mouseReleaseEvents;
		std::queue<QMouseEvent> mouseMoveEvents;

		std::queue<QKeyEvent> keyPressEvents;
		std::queue<QKeyEvent> keyReleaseEvents;
		std::queue<QWheelEvent> mouseWheelEvents;

		QPoint oldMousePos;
		bool closing = false;

		QGamepad gamepads[15];
		
		void connectGamepads();
	};
}
#endif