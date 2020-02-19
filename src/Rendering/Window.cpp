#include "Window.h"
#include "Engine.h"
#include <iostream>

namespace Tristeon
{
	Window::Window(Engine* engine) : engine(engine)
	{
		setSurfaceType(QWindow::OpenGLSurface);
		show();

		timer = new QTimer(this);
		QObject::connect(timer, &QTimer::timeout, this, &Window::update);
		timer->start(0);
	}

	void Window::preRender()
	{
		if (!context)
			initialize();
		
		if (!isExposed())
			return;

		context->context->makeCurrent(this);

		context->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		context->glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	}

	void Window::postRender()
	{
		if (!isExposed())
			return;

		context->context->swapBuffers(this);
	}

	void Window::initialize()
	{
		context = std::make_unique<GLContext>(this);
		engine->renderer->initialize();
	}

	void Window::update()
	{
		engine->update();
	}

	void Window::resizeEvent(QResizeEvent* ev)
	{
		if (!context)
			return;
		context->glViewport(0, 0, ev->size().width(), ev->size().height());
	}
}
