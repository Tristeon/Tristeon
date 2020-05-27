#pragma once
#include <QMainWindow>
#include <QMouseEvent>
#include <QtGamepad/QtGamepad>

#include <Utils/Singleton.h>

#include <queue>

namespace Tristeon
{
	class Engine;

	/**
	 * Window inherits from QMainWindow and handles all the input callbacks.
	 * It collects the events in a queue and plays them whenever the update loop calls pollEvents()
	 */
	class Window : public QMainWindow, public Singleton<Window>
	{
		friend Engine;
	public:
		Window();
		
		/**
		 * The width of the full main window.
		 * For rendering purposes, it is recommended to use GameView::width()
		 */
		static unsigned int width() { return instance()->QMainWindow::width(); }

		/**
		 * The height of the full main window.
		 * For rendering purposes, it is recommended to use GameView::height()
		 */
		static unsigned int height() { return instance()->QMainWindow::height(); }

		/**
		 * Returns true if the window is fullscreen, false if it's not
		 */
		static bool fullScreen() { return instance()->isFullScreen(); }

		static void close() { ((QMainWindow*)instance())->close(); }
	protected:
		void mousePressEvent(QMouseEvent* event) override;
		void mouseReleaseEvent(QMouseEvent* event) override;
		void mouseDoubleClickEvent(QMouseEvent* event) override;
		void wheelEvent(QWheelEvent* event) override;
		void keyPressEvent(QKeyEvent* event) override;
		void keyReleaseEvent(QKeyEvent* event) override;
		void closeEvent(QCloseEvent* event) override;

		void gamepadsChanged();
		
	private:
		std::queue<QMouseEvent> mousePressEvents;
		std::queue<QMouseEvent> mouseReleaseEvents;
		std::queue<QMouseEvent> mouseDoubleClickEvents;
		std::queue<QMouseEvent> mouseMoveEvents;

		std::queue<QKeyEvent> keyPressEvents;
		std::queue<QKeyEvent> keyReleaseEvents;
		std::queue<QWheelEvent> mouseWheelEvents;

		QPoint oldMousePos;

		QGamepad* activeGamepad = nullptr;
		/**
		 * Iterates over the event queues and sends each event to their respective classes (Mouse, Keyboard, ...)
		 */
		void pollEvents();

		void connectGamepads();
	};
}