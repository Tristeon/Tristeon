#include "GameView.h"

#include <Engine.h>
#include <QApplication>
#include <QCommonStyle>
#include <Scenes/Scene.h>
#include <Scenes/SceneManager.h>
#include <Window.h>

#include "Project.h"

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
		format.setSwapBehavior(Project::Graphics::tripleBuffering() ? QSurfaceFormat::TripleBuffer : QSurfaceFormat::SwapBehavior::DoubleBuffer);
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
		context()->makeCurrent(context()->surface());
	}

	Vector2 GameView::screenToWorld(Vector2Int screenPoint)
	{
		//Convert into Qt coords
		if (!Window::fullScreen())
			screenPoint.y -= QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight);
		screenPoint = Vector2Int(screenPoint.x, Window::height() - screenPoint.y);

		//Convert into local coords
		QPoint local = instance()->mapFromGlobal(QPoint(screenPoint.x, screenPoint.y));
		//Flip Y and remove area below gameview because apparently mapFromGlobal() doesnt handle that space?
		local.setY(Window::height() - (Window::height() - instance()->rect().bottom()) - local.y());

		//Scale with gameview screen 
		Vector2 const screenSize = Vector2(Window::width(), Window::height());
		Vector2 const viewSize = Vector2(width(), height());

		//Adjust for center
		Vector2 const halfSize = Vector2(width() / 2.0f, height() / 2.0f);

		Vector2 result = Vector2(local.x(), local.y()) - halfSize;
		//Adjust for camera
		result *= screenSize / viewSize;
		result *= 1.0f / Camera::main()->zoom;
		result += Camera::main()->position;
		return result;
	}

	Vector2Int GameView::worldToScreen(Vector2 worldPoint)
	{
		Vector2 const screenSize = Vector2(Window::width(), Window::height());
		Vector2 const viewSize = Vector2(width(), height());
		Vector2 const halfSize = Vector2(width() / 2.0f, height() / 2.0f);

		worldPoint -= Camera::main()->position;
		worldPoint /= (1.0f / Camera::main()->zoom);
		worldPoint /= (screenSize / viewSize);

		worldPoint += halfSize;

		worldPoint.y = Window::height() - (Window::height() - instance()->rect().bottom()) - worldPoint.y;
		QPoint global = instance()->mapToGlobal(QPoint(worldPoint.x, worldPoint.y));

		Vector2Int result = Vector2Int(global.x(), Window::height() - global.y());
		if (!Window::fullScreen())
			result.y += QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight);
		return result;
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
	}
}
