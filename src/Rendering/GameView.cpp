#include "GameView.h"
#include "Engine.h"
#include <iostream>
#include <QApplication>
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

		QSurfaceFormat format;
		format.setRenderableType(QSurfaceFormat::OpenGL);
		format.setProfile(QSurfaceFormat::CoreProfile);
		format.setSamples(0);
		setFormat(format);
	}

	void GameView::makeContextCurrent()
	{
		context()->makeCurrent(context()->surface());
	}

	void GameView::initializeGL()
	{
		QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
		f->glClearColor(1, 1, 0, 1);
		f->glEnable(GL_CULL_FACE);
		f->glCullFace(GL_BACK);
		//f->glEnable(GL_BLEND);
		f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		f->glDisable(GL_MULTISAMPLE);
	}

	void GameView::resizeGL(int w, int h)
	{
		paintGL();
	}

	void GameView::paintGL()
	{
		makeContextCurrent();

		QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
		f->glClearColor(1, 0.75, 0.75, 1);
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
		if (firstUpdate)
		{
			firstUpdate = false;
			engine->initialize();
		}
		
		Window::main()->pollEvents();

		Scene* scene = SceneManager::getCurrentScene();

		if (scene != nullptr)
			scene->update();

		paintGL();

		Mouse::reset();
		Keyboard::reset();
	}
}
