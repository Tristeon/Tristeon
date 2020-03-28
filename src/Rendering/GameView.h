#pragma once
#include <QOpenGLWidget>
#include <QResizeEvent>
#include <QTimer>

#include <Utils/Singleton.h>
#include <Utils/Colour.h>

namespace Tristeon
{
	class Engine;

	/**
	 * GameView is a OpenGL Widget, created by Qt to which the scene/game is rendered. 
	 */
	class GameView : private QOpenGLWidget, public Singleton<GameView>
	{
		Q_OBJECT;
		friend Engine;
	public:
		/**
		 * Called by the Qt UI loader, do not call again.
		 */
		GameView(Engine* engine, QWidget* parent);

		/**
		 * Sets the clear colour of the GameView.
		 */
		static void setClearColour(Colour const& colour);

		/**
		 * Sets QOpenGLContext::currentContext to the context of this widget.
		 * This may be used by objects that ought to be rendered using the Qt OpenGL framework.
		 */
		static void makeContextCurrent();

		/**
		 * Returns the current width of the GameView. This may not be the width of the entire Window.
		 * For the full window width use the Window class.
		 */
		static unsigned int width() { return instance()->QOpenGLWidget::width(); }
		
		/**
		 * Returns the current height of the GameView. This may not be the height of the entire Window.
		 * For the full window height use the Window class.
		 */
		static unsigned int height() { return instance()->QOpenGLWidget::height(); }

		/**
		 * Get access to the QOpenGLWidget base class.
		 * This access is normally not granted because most customizable behaviour is intended to be set by the Engine only.
		 */
		QOpenGLWidget* widget() const { return (QOpenGLWidget*)this; }

		/**
		 * Get the OpenGL Context for the GameView Widget.
		 */
		static QOpenGLContext* context() { return instance()->widget()->context(); }

		/**
		 * The time it took for a single frame to execute in ms.
		 */
		static float deltaTime() { return instance()->_deltaTime; }

		/**
		 * The amount of frames per second the program is running at.
		 */
		static uint fps() { return instance()->_fps; }
	private:
		Engine* engine = nullptr;

		uint _fps = 0;
		float _deltaTime = 0;
		
		void initializeGL() override;
		void resizeGL(int w, int h) override;
		void paintGL() override;
	};
}
