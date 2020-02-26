#pragma once
#include <QMainWindow>
#include <QKeyEvent>
#include <QMouseEvent>
#include <queue>

namespace Tristeon
{
	class GameView;
	class Window : public QMainWindow
	{
		friend GameView;
		
	public:
		Window();
		~Window();

		static Window* main();
	protected:
		static Window* instance;
		void mousePressEvent(QMouseEvent* event) override;
		void mouseReleaseEvent(QMouseEvent* event) override;
		void mouseDoubleClickEvent(QMouseEvent* event) override;
		void mouseMoveEvent(QMouseEvent* event) override;
		void keyPressEvent(QKeyEvent* event) override;
		void keyReleaseEvent(QKeyEvent* event) override;
		void focusInEvent(QFocusEvent* event) override;
		void focusOutEvent(QFocusEvent* event) override;
		void moveEvent(QMoveEvent* event) override;
		void resizeEvent(QResizeEvent* event) override;
		void closeEvent(QCloseEvent* event) override;

	private:
		std::queue<QMouseEvent> mousePressEvents;
		std::queue<QMouseEvent> mouseReleaseEvents;
		std::queue<QMouseEvent> mouseDoubleClickEvents;
		std::queue<QMouseEvent> mouseMoveEvents;

		std::queue<QKeyEvent> keyPressEvents;
		std::queue<QKeyEvent> keyReleaseEvents;

		void pollEvents();
	};
}
