#include "GameView.h"

#include <QOpenGLFunctions>

#include <Engine.h>
#include <Scenes/Scene.h>
#include <Scenes/SceneManager.h>
#include <Window.h>

namespace Tristeon
{
	GameView::GameView(Engine* engine, QWidget* parent) : QOpenGLWidget(parent), engine(engine)
	{
		engine->_view = this;
		show();

		//Disables sampling because it harms 2D transparency
		QSurfaceFormat format;
		format.setRenderableType(QSurfaceFormat::OpenGL);
		format.setProfile(QSurfaceFormat::CoreProfile);
		format.setSamples(0);
		format.setSwapBehavior(QSurfaceFormat::TripleBuffer);
		format.setSwapInterval(0);
		setFormat(format);
	}

	void GameView::setClearColour(Colour const& colour)
	{
		QOpenGLFunctions* f = context()->functions();
		f->glClearColor(colour.r, colour.g, colour.b, colour.a);
	}

	void GameView::makeContextCurrent()
	{
		instance()->context()->makeCurrent(instance()->context()->surface());
	}

	void GameView::initializeGL()
	{
		QOpenGLFunctions* f = context()->functions();
		f->glClearColor(0, 0, 0, 1);

		//Enable culling
		f->glEnable(GL_CULL_FACE);
		f->glCullFace(GL_BACK);

		//Enable transparency blending
		f->glEnable(GL_BLEND);
		f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Disable multisampling because any form of aliasing messes with transparency in 2D
		f->glDisable(GL_MULTISAMPLE);
	}

	void GameView::resizeGL(int w, int h)
	{
		paintGL();
	}

	void GameView::paintGL()
	{
		makeContextCurrent();

		QOpenGLFunctions* f = context()->functions();
		f->glClear(GL_COLOR_BUFFER_BIT);

		Scene* scene = SceneManager::current();
		if (scene != nullptr)
		{
			engine->_renderer->renderScene(scene);
			engine->_renderer->renderHUD(scene->getHUD());
		}

		update();
	}
}
