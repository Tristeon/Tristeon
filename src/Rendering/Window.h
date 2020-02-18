#pragma once

#include <QWindow>
#include <QTimer>
#include <QOpenGLFunctions>
#include <QOpenGLPaintDevice>
#include <QOpenGLContext>
#include <QPainter>

namespace Tristeon
{
	class Engine;

	class Window : public QWindow, private QOpenGLFunctions
	{
		Q_OBJECT
		friend Engine;
	public:
		explicit Window(Engine* engine);
		~Window();
	private:
		void preRender();
		void postRender();

		void initialize();
		void update();

		Engine* engine = nullptr;

		QOpenGLContext* context = nullptr;

		QTimer* timer;
	};
}