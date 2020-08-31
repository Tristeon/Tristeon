#pragma once
#include <Utils/Singleton.h>
#include <Math/Vector2.h>
#include <Math/Vector2Int.h>

namespace Tristeon
{
	struct Colour;
	struct Vector2Int;
	class Engine;

	/**
	 * Window handles window creation and system input.
	 * It also contains useful information about screen and game sizes and screen to world (and back) transformations.
	 */
	class Window : private Singleton<Window>
	{
		friend Singleton<Window>;
		friend Engine;
	public:
		virtual ~Window() = default;
		
		/**
		 * The width of the window. This may potentially be different from the world/game size.
		 * For rendering purposes, it is recommended to use GameView::width()
		 */
		static unsigned int width() { return instance()->_windowWidth(); }

		/**
		 * The height of the window. This may potentially be different from the world/game size.
		 * For rendering purposes, it is recommended to use GameView::height()
		 */
		static unsigned int height() { return instance()->_windowHeight(); }

		/**
		 * Returns the width of the game screen. This may not be the width of the entire Window.
		 */
		static unsigned int gameWidth() { return instance()->_gameWidth(); }

		/**
		 * Returns the height of the game screen. This may not be the height of the entire Window.
		 */
		static unsigned int gameHeight() { return instance()->_gameHeight(); }

		/**
		 * Converts a screen point (Like Mouse::position() into world coordinates.
		 * This function takes into account that game's screen might not always take up the full window.
		 */
		static Vector2 screenToWorld(Vector2Int const& screenPoint) { return instance()->_screenToWorld(screenPoint); }

		/**
		 * Converts a world coordinate into a screen point.
		 * This function takes into account that game's screen might not always take up the full window.
		 */
		static Vector2Int worldToScreen(Vector2 const& worldPoint) { return instance()->_worldToScreen(worldPoint); }

		/**
		 * Returns true if the window is fullscreen, false if it's not
		 */
		static bool isFullscreen() { return instance()->_isFullscreen(); }

		/**
		 * Sets the window's fullscreen status.
		 */
		static void setFullscreen(bool const& value) { instance()->_setFullscreen(value); }

		/**
		 * Closes the window and the application.
		 */
		static void close() { instance()->_close(); }

		/**
		 * Returns true if the window is about to close
		 */
		static bool closingDown() { return instance()->_closingDown(); }

		/**
		 * Sets the window title to the given value.
		 */
		static void setWindowTitle(std::string const& title) { instance()->_setWindowTitle(title); }

	protected:
		/**
		 * Iterates over the event queues and sends each event to their respective classes (Mouse, Keyboard, Gamepad)
		 */
		static void pollEvents() { instance()->_pollEvents(); }
		static void draw() { instance()->_draw(); }

		virtual void _pollEvents() = 0;
		virtual void _draw() = 0;
		
		virtual unsigned int _windowWidth() = 0;
		virtual unsigned int _windowHeight() = 0;

		virtual unsigned int _gameWidth() = 0;
		virtual unsigned int _gameHeight() = 0;
		
		virtual bool _isFullscreen() = 0;
		virtual void _setFullscreen(bool const& value) = 0;

		virtual void _close() = 0;

		virtual void _setClearColour(Colour const& colour) = 0;

		virtual bool _closingDown() = 0;

		virtual void _setWindowTitle(std::string const& value) = 0;

		virtual Vector2 _screenToWorld(Vector2Int const& screenPoint) = 0;
		virtual Vector2Int _worldToScreen(Vector2 const& worldPoint) = 0;
	};
}