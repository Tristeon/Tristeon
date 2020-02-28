#pragma once
#include <QTimer>
#include <QOpenGLWidget>
#include <QResizeEvent>

namespace Tristeon
{
	class Engine;

	class GameView : public QOpenGLWidget
	{
		Q_OBJECT
		friend Engine;
	public:
		GameView(Engine* engine, QWidget* parent);

		void makeContextCurrent();
	private:
		void initializeGL() override;
		void resizeGL(int w, int h) override;
		void paintGL() override;

		void update();

		bool firstUpdate = true;
		Engine* engine = nullptr;
		QTimer* timer = nullptr;
	};
}
