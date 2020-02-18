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

	Window::~Window()
	{
		delete context;
	}

	void Window::preRender()
	{
		if (!context)
			initialize();

		if (!isExposed())
			return;

		context->makeCurrent(this);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	}

	void Window::postRender()
	{
		if (!isExposed())
			return;

		context->swapBuffers(this);
	}

	void Window::initialize()
	{
		context = new QOpenGLContext(this);
		context->setFormat(requestedFormat());
		context->create();
		context->makeCurrent(this);

		initializeOpenGLFunctions();

		engine->renderer->initialize();
	}

	void Window::update()
	{
		engine->update();
	}
}