#include "GLContext.h"
#include <Rendering/Window.h>

namespace Tristeon
{
	GLContext* GLContext::instance = nullptr;

	GLContext::GLContext(Window* window) : window(window)
	{
		context = new QOpenGLContext(window);
		context->setFormat(window->requestedFormat());
		context->create();
		context->makeCurrent(window);
		
		initializeOpenGLFunctions();

		instance = this;

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	GLContext::~GLContext()
	{
		instance = nullptr;
	}

	GLContext* GLContext::getInstance()
	{
		return instance;
	}

	QOpenGLContext* GLContext::getQContext()
	{
		return getInstance()->context;
	}

	Window* GLContext::getWindow()
	{
		return getInstance()->window;
	}

	void GLContext::makeCurrent()
	{
		getQContext()->makeCurrent(getWindow());
	}
}
