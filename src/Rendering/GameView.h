#pragma once
#include <QOpenGLWidget>
#include <QResizeEvent>
#include <QTimer>

#include <Utils/Singleton.h>
#include <Utils/Colour.h>

#include "Math/Vector2Int.h"

namespace Tristeon
{
	class Engine;

	/**
	 * GameView is a OpenGL Widget, created by Qt to which the scene/game is rendered. 
	 */
	class GameView : protected QOpenGLWidget, public Singleton<GameView>
	{
		Q_OBJECT;
		friend Engine;
	public:
		/**
		 * Called by the Qt UI loader, do not call again.
		 */
		GameView(Engine* engine, QWidget* parent);

		virtual ~GameView() = default;
		
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
		 * Converts a screen point (Like Mouse::position() into world coordinates.
		 * This function takes into account that gameview might not always be fullscreen.
		 */
		static Vector2 screenToWorld(Vector2Int screenPoint);
		/**
		 * Converts a world coordinate into a screen point.
		 * This function takes into account that gameview might not always be fullscreen.
		 */
		static Vector2Int worldToScreen(Vector2 worldPoint);
	protected:
		Engine* engine = nullptr;

		void initializeGL() override;
		void resizeGL(int w, int h) override;
		void paintGL() override;
	};
}