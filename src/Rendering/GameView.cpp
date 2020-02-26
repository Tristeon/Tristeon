#include "GameView.h"
#include "Engine.h"
#include <iostream>
#include <QMainWindow>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"
#include "Window.h"

namespace Tristeon
{
	GameView::GameView(Engine* engine, QWidget* parent) : QOpenGLWidget(parent), engine(engine)
	{
		timer = new QTimer(this);
		QObject::connect(timer, &QTimer::timeout, this, &GameView::update);
		timer->start(0);
		show();
	}

	void GameView::initializeGL()
	{
		QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
		f->glClearColor(1, 1, 0, 1);

		f->glEnable(GL_DEPTH_TEST);
		f->glEnable(GL_CULL_FACE);
		f->glCullFace(GL_BACK);
	}

	void GameView::resizeGL(int w, int h)
	{
		paintGL();
	}

	void GameView::paintGL()
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
		Window::main()->update();
	}

	void GameView::update()
	{
		if (engine == nullptr)
			return;

		Window::main()->pollEvents();

		Scene* scene = SceneManager::getCurrentScene();

		if (scene != nullptr)
			scene->update();

		paintGL();

		Mouse::reset();
		Keyboard::reset();
	}
}
