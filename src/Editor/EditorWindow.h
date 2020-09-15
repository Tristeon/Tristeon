#pragma once
#include "Input/Keyboard.h"
#ifdef TRISTEON_EDITOR
#include "GameView.h"
#include <queue>
#include <Window.h>

#include <QMainWindow>
#include <QGamepad>

#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>

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
		void gamepadsChanged();

		void _pollEvents() override;

		unsigned int _windowWidth() override;
		unsigned int _windowHeight() override;
		unsigned int _gameWidth() override;
		unsigned int _gameHeight() override;

		bool _isFullscreen() override;
		void _setFullscreen(bool const& value) override;

		void _close() override;
		bool _closingDown() override;

		void _setClearColour(Tristeon::Colour const& colour) override;
		void _draw() override;

		void _setWindowTitle(std::string const& value) override;
		
		Tristeon::Vector2 _screenToWorld(Tristeon::Vector2Int const& screenPoint, Tristeon::Camera* camera) override;
		Tristeon::Vector2Int _worldToScreen(Tristeon::Vector2 const& worldPoint, Tristeon::Camera* camera) override;

		Tristeon::Keyboard::Key mapToTristeonKey(Qt::Key key);
	private:
		std::queue<QMouseEvent> mousePressEvents;
		std::queue<QMouseEvent> mouseReleaseEvents;
		std::queue<QMouseEvent> mouseMoveEvents;

		std::queue<QKeyEvent> keyPressEvents;
		std::queue<QKeyEvent> keyReleaseEvents;
		std::queue<QWheelEvent> mouseWheelEvents;

		QPoint oldMousePos;
		QGamepad* activeGamepad = nullptr;

		bool closing = false;
		
		void connectGamepads();
	};
}
#endif