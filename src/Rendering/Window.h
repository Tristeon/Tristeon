#pragma once

#include <QWindow>
#include <QTimer>
#include <QResizeEvent>
#include "GLContext.h"

namespace Tristeon
{
	class Engine;

	class Window : public QWindow
	{
		Q_OBJECT
		friend Engine;
	public:
		explicit Window(Engine* engine);
	private:
		void preRender();
		void postRender();

		void initialize();
		void update();

		void resizeEvent(QResizeEvent* ev) override;

		Engine* engine = nullptr;

		std::unique_ptr<GLContext> context = nullptr;

		QTimer* timer;
	};
}
