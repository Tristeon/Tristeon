#include "Window.h"
#include "Engine.h"
#include <iostream>
#include <QOpenGLFunctions>


#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"

namespace Tristeon
{
	Window::Window(Engine* engine, QWidget* parent) : QOpenGLWidget(parent), engine(engine)
	{
		timer = new QTimer(this);
		QObject::connect(timer, &QTimer::timeout, this, &Window::update);
		timer->start(0);
		show();
	}

	void Window::initializeGL()
	{
		QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
		f->glClearColor(1, 1, 0, 1);

		f->glEnable(GL_DEPTH_TEST);
		f->glEnable(GL_CULL_FACE);
		f->glCullFace(GL_BACK);
	}

	void Window::resizeGL(int w, int h)
	{
		paintGL();
	}

	void Window::paintGL()
	{
		context()->makeCurrent(context()->surface());

		QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
		f->glClearColor(1, 1, 0, 1);
		f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Scene* scene = SceneManager::getCurrentScene();
		if (scene != nullptr)
		{
			engine->renderer->renderScene(scene);
			engine->renderer->renderHUD(scene->getHUD());
		}
	}

	void Window::update()
	{
		if (engine == nullptr)
			return;

		Scene* scene = SceneManager::getCurrentScene();

		if (scene != nullptr)
			scene->update();

		paintGL();
	}
}
