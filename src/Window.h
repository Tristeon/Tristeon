#pragma once
#include <Utils/Singleton.h>
#include <Math/Vector.h>
#include <Settings.h>
#include <set>

namespace Tristeon
{
	struct Colour;
	class Camera;

	/**
	 * Window handles window creation and system input.
	 * It also contains useful information about screen and game sizes and screen to world (and back) transformations.
	 *
	 * Window is statically accessed but it handles all functionality internally through virtual methods to accommodate for different window types when running either the editor or the non-editor build.
	 */
	class Window : private Singleton<Window>
	{
		friend Settings::Graphics;
		friend Singleton<Window>;
		friend class Engine;
	public:
		Window() = default;
		virtual ~Window() = default;

		DELETE_COPY(Window);
		DEFAULT_MOVE(Window);

		/**
		 * The width of the window. This may potentially be different from the world/game size, because in-editor mode the game is represented using a smaller view.
		 * For rendering purposes, it is recommended to use Window::gameWidth()
		 */
		[[nodiscard]] static unsigned int width() { return instance()->_windowWidth(); }

		/**
		 * The height of the window. This may potentially be different from the world/game size, because in-editor mode the game is represented using a smaller view.
		 * For rendering purposes, it is recommended to use Window::gameHeight()
		 */
		[[nodiscard]] static unsigned int height() { return instance()->_windowHeight(); }

		/**
		 * The size of the window. This may potentially be different from the world/game size, because in-editor mode the game is represented using a smaller view.
		 * For rendering purposes, it is recommended to use Window::gameSize()
		 */
		[[nodiscard]] static VectorU size() { return { width(), height() }; }

		/**
		 * Returns the width of the game screen. This may not be the width of the entire Window, because in-editor mode the game is represented using a smaller view.
		 */
		[[nodiscard]] static unsigned int gameWidth() { return instance()->_gameWidth(); }

		/**
		 * Returns the height of the game screen. This may not be the height of the entire Window, because in-editor mode the game is represented using a smaller view.
		 */
		[[nodiscard]] static unsigned int gameHeight() { return instance()->_gameHeight(); }

		/**
		 * Returns the size of the game screen. This may not be the size of the entire Window, because in-editor mode the game is represented using a smaller view.
		 */
		[[nodiscard]] static VectorU gameSize() { return { gameWidth(), gameHeight() }; }

		/**
		 * Converts a screen point (Like Mouse::position() into world coordinates.
		 *
		 * It is possible for this function to extend beyond visible coordinates in editor mode because of the smaller game view.
		 */
		[[nodiscard]] static Vector screenToWorld(const VectorU& screenPoint, Camera* camera) { return instance()->_screenToWorld(screenPoint, camera); }

		/**
		 * Converts a world coordinate into a screen point.
		 */
		[[nodiscard]] static VectorU worldToScreen(const Vector& worldPoint, Camera* camera) { return instance()->_worldToScreen(worldPoint, camera); }

		/**
		 * Closes the window and the application.
		 *
		 * Before the application closes, the current scene is unloaded and all objects are destroyed.
		 */
		static void close() { instance()->_close(); }

		/**
		 * Returns true if the window is about to close.
		 */
		[[nodiscard]] static bool closingDown() { return instance()->_closingDown(); }

		/**
		 * Sets the window title to the given value.
		 */
		static void setWindowTitle(const String& title) { instance()->_setWindowTitle(title); }

		/**
		 * Returns a set of the resolutions available to the current monitor.
		 */
		[[nodiscard]] static std::set<VectorU> availableResolutions() { return instance()->_resolutions; }

		/**
		 * Returns the number of displays available.
		 */
		[[nodiscard]] static unsigned int numberOfDisplays();

		/**
		 * The current display that the Window is on.
		 * In fullscreen, this is usually the same as Project::Graphics::preferredDisplay(), but it may differ if the hardware situation is different from previous runs.
		 *
		 * In windowed mode, this function always returns 0 regardless of where the window is.
		 */
		[[nodiscard]] static unsigned int currentDisplay() { return instance()->_currentDisplay(); };
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
		
		virtual void _setWindowMode(const Settings::Graphics::WindowMode& mode) = 0;
		virtual void _setVsync(const bool& value) = 0;
		virtual void _setResolution(const VectorU& resolution) = 0;
		virtual void _setDisplay(const unsigned int& display) = 0;
		virtual unsigned int _currentDisplay() = 0;

		virtual void _close() = 0;

		virtual void _setClearColour(Colour const& colour) = 0;

		virtual bool _closingDown() = 0;

		virtual void _setWindowTitle(std::string const& value) = 0;

		virtual Vector _screenToWorld(VectorU const& screenPoint, Camera* camera) = 0;
		virtual VectorU _worldToScreen(Vector const& worldPoint, Camera* camera) = 0;
#pragma endregion

		void populateResolutions(const unsigned int& monitor);
		std::set<VectorU> _resolutions;
	};
}