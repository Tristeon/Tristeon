#pragma once

#include <QTimer>
#include <QOpenGLWidget>
#include <QResizeEvent>

namespace Tristeon
{
	class Engine;

	class Window : public QOpenGLWidget
	{
		Q_OBJECT
		friend Engine;
	public:
		Window(Engine* engine, QWidget* parent);
	private:
		void initializeGL() override;
		void resizeGL(int w, int h) override;
		void paintGL() override;

		void update();
		
		Engine* engine = nullptr;
		QTimer* timer = nullptr;
	};
}
