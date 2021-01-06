#pragma once
#include <Utils/Singleton.h>
#include <Math/Vector.h>

#include "Project.h"
#include <set>

namespace Tristeon
{
	struct Colour;
	class Camera;

	/**
	 * Window handles window creation and system input.
	 * It also contains useful information about screen and game sizes and screen to world (and back) transformations.
	 *
	 * While statically accessed, Window handles all functionality internally through virtual methods.
	 * This is because the editor and non-editor build can contain different functionality.
	 */
	class Window : private Singleton<Window>
	{
		friend Project::Graphics;
		friend Singleton<Window>;
		friend class Engine;
	public:
		Window()
		{
			populateResolutions();
		}
		virtual ~Window() = default;

		DELETE_COPY(Window);
		DEFAULT_MOVE(Window);
		
		/**
		 * The width of the window. This may potentially be different from the world/game size.
		 * For rendering purposes, it is recommended to use Window::gameWidth()
		 */
		[[nodiscard]] static unsigned int width() { return instance()->_windowWidth(); }

		/**
		 * The height of the window. This may potentially be different from the world/game size.
		 * For rendering purposes, it is recommended to use Window::gameHeight()
		 */
		[[nodiscard]] static unsigned int height() { return instance()->_windowHeight(); }

		/**
		 * The size of the window. This may potentially be different from the world/game size.
		 * For rendering purposes, it is recommended to use Window::gameSize()
		 */
		[[nodiscard]] static VectorU size() { return { width(), height() }; }

		/**
		 * Returns the width of the game screen. This may not be the width of the entire Window.
		 */
		[[nodiscard]] static unsigned int gameWidth() { return instance()->_gameWidth(); }

		/**
		 * Returns the height of the game screen. This may not be the height of the entire Window.
		 */
		[[nodiscard]] static unsigned int gameHeight() { return instance()->_gameHeight(); }

		/**
		 * Returns the size of the game screen. This may not be the size of the entire Window.
		 */
		[[nodiscard]] static VectorU gameSize() { return { gameWidth(), gameHeight() }; }
		
		/**
		 * Converts a screen point (Like Mouse::position() into world coordinates.
		 * This function takes into account that game's screen might not always take up the full window.
		 */
		[[nodiscard]] static Vector screenToWorld(const VectorU& screenPoint, Camera* camera) { return instance()->_screenToWorld(screenPoint, camera); }

		/**
		 * Converts a world coordinate into a screen point.
		 * This function takes into account that game's screen might not always take up the full window.
		 */
		[[nodiscard]] static VectorU worldToScreen(const Vector& worldPoint, Camera* camera) { return instance()->_worldToScreen(worldPoint, camera); }

		/**
		 * Closes the window and the application.
		 */
		static void close() { instance()->_close(); }

		/**
		 * Returns true if the window is about to close
		 */
		[[nodiscard]] static bool closingDown() { return instance()->_closingDown(); }

		/**
		 * Sets the window title to the given value.
		 */
		static void setWindowTitle(const String& title) { instance()->_setWindowTitle(title); }

		/**
		 * Returns a set of the resolutions available to the current monitor.
		 */
		static std::set<VectorU> availableResolutions() { return instance()->_resolutions; }
	protected:
		/**
		 * Iterates over the event queues and sends each event to their respective classes (Mouse, Keyboard, Gamepad)
		 */
		static void pollEvents() { instance()->_pollEvents(); }
		static void draw() { instance()->_draw(); }

		//This set of functions are all implemented in inheriting Window implementations and are statically accessed through the functions above.
#pragma region virtual functions
		virtual void _pollEvents() = 0;
		virtual void _draw() = 0;
		
		virtual unsigned int _windowWidth() = 0;
		virtual unsigned int _windowHeight() = 0;

		virtual unsigned int _gameWidth() = 0;
		virtual unsigned int _gameHeight() = 0;
		
		virtual void _setWindowMode(const Project::Graphics::WindowMode& mode) = 0;
		virtual void _setVsync(const bool& value) = 0;
		virtual void _setResolution(const VectorU& resolution) = 0;

		virtual void _close() = 0;

		virtual void _setClearColour(Colour const& colour) = 0;

		virtual bool _closingDown() = 0;

		virtual void _setWindowTitle(std::string const& value) = 0;

		virtual Vector _screenToWorld(VectorU const& screenPoint, Camera* camera) = 0;
		virtual VectorU _worldToScreen(Vector const& worldPoint, Camera* camera) = 0;
#pragma endregion

	private:
		void populateResolutions();
		
		std::set<VectorU> _resolutions;
	};
}