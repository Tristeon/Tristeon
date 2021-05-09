#pragma once
#include <Standard/String.h>
#include <Math/Vector.h>
#include <Serialization/Serializable.h>

//Forward declare main so that we can friend it later on
//this allows main to call Engine::run()
int main(int argc, char** argv);

#ifdef TRISTEON_EDITOR
namespace TristeonEditor { class ProjectWindow; }
#endif

namespace Tristeon
{
	/**
	 * Settings is the main interface for interacting with graphics and physics settings.
	 * Upon application startup, settings loads in the project's settings from the settings.tristeon file.
	 *
	 * Some settings are easily adjustable and can be changed dynamically, others can only be modified using the project window.
	 * For convenience' sake, some of the settings are also available through other class interfaces like for example Grid.
	 */
	class Settings
	{
		friend class Engine;
		friend class Graphics;
		friend class Physics;
		friend int ::main(int argc, char** argv);
		
#ifdef TRISTEON_EDITOR
		friend TristeonEditor::ProjectWindow;
#endif

	public:
		/**
		 * The path to the asset folder.
		 * This folder contains all of the project's assets (images, scenes, etc.).
		 */
		[[nodiscard]] static String assetPath();
		/**
		 * The first scene to be loaded in build mode.
		 * If the scene can't be found upon load time, an empty scene will be created & loaded (but not saved).
		 */
		[[nodiscard]] static String firstSceneName();
		
		class Graphics
		{
			friend Settings;
		public:
			/**
			 * The width (size on the x-axis) of a single tile on the grid in pixels.
			 *
			 * Tiles are un-rotated squares, defined by tileWidth() and tileHeight().
			 */
			[[nodiscard]] static unsigned int tileWidth();
			/**
			 * The height (size on the y-axis) of a single tile on the grid in pixels.
			 *
			 * Tiles are un-rotated squares, defined by tileWidth() and tileHeight().
			 */
			[[nodiscard]] static unsigned int tileHeight();
			/**
			 * The size of a single tile on the grid in pixels.
			 */
			[[nodiscard]] static VectorU tileSize();

			/**
			 * If vertical sync is enabled.
			 *
			 * Vertical sync limits the application's refresh rate to the screen's refresh rate (usually represented in hz).
			 * By doing so, you can prevent effects like screen tearing, but you might increase input lag and reduce the frame rate, which might be undesired by the user.
			 *
			 * It is usually a good idea to allow users to change this in a settings menu.
			 */
			[[nodiscard]] static bool vsync();
			/**
			 * Enable/disable vsync in the settings and for the runtime window.
			 *
			 * Vertical sync limits the application's refresh rate to the screen's refresh rate (usually represented in hz).
			 * By doing so, you can prevent effects like screen tearing, but you might increase input lag and reduce the frame rate, which might be undesired by the user.
			 *
			 * It is usually a good idea to allow users to change this in a settings menu.
			 * Setting this affects the settings and the runtime property.
			 */
			static void setVsync(const bool& value);

			/**
			 * The window's fullscreen settings and decoration.
			 * This only applies to built games (no editor).
			 */
			enum class WindowMode
			{
				/**
				 * The window has a normal window with borders and open/close/maximize buttons.
				 * It can be moved and resized freely.
				 */
				Windowed,
				/**
				 * Just like Windowed, but now without borders or open/close/maximize buttons.
				 * The window is always maximized (similar to fullscreen).
				 */
				Borderless,
				/**
				 * The window is fullscreen.
				 */
				Fullscreen
			};
			/**
			 * The current window mode (fullscreen status & decoration).
			 */
			[[nodiscard]] static WindowMode windowMode();
			/**
			 * Set the window mode in the settings and the runtime window.
			 *
			 * This determines the window's fullscreen status & decoration. See the WindowMode enum for more.
			 * Setting this affects the settings and the runtime property.
			 */
			static void setWindowMode(const WindowMode& value);
			
			/**
			 * The resolution that the game will take when loaded in on fullscreen.
			 *
			 * If the game is unable to use this resolution it'll attempt to find the most suitable option.
			 */
			[[nodiscard]] static VectorU preferredResolution();
			/**
			 * Set the preferred resolution.
			 * The game will attempt to take this resolution when loaded in on fullscreen.
			 * If the game is unable to use this resolution it'll attempt to find the most suitable option.
			 *
			 * Setting this affects the settings and the runtime property.
			 */
			static void setPreferredResolution(const VectorU& value);

			/**
			 * The maximum number of frames per second.
			 * If frames render faster than this, then the update loop waits at the end of the frame until enough time has past.
			 * If the value is set to 0, the engine runs as if there were no limit.
			 */
			[[nodiscard]] static unsigned int maxFPS();

			/**
			 * Set the maximum number of frames per second.
			 * If frames render faster than this, then the update loop waits at the end of the frame until enough time has past.
			 * If the value is set to 0, the engine runs as if there were no limit.
			 *
			 * Setting this affects the settings and the runtime property.
			 */
			static void setMaxFPS(const unsigned int& value);

			/**
			 * The preferred display to use for borderless and fullscreen applications.
			 * The application will attempt to use this display but will default to display 0 if it can't be found.
			 */
			[[nodiscard]] static unsigned int preferredDisplay();
			
			/**
			 * Set the preferred display used for borderless and fullscreen applications.
			 * Upon loading, the application will attempt to use this monitor but will default to display 0 if it can't be found.
			 *
			 * If called in runtime, and the monitor exists, the application will switch to the given display.
			 * Whenever the display is changed, Window::availableResolutions updates.
			 *
			 * Setting this affects the settings and the runtime property.
			 */
			static void setPreferredDisplay(const unsigned int& display);

		private:
			unsigned int _tileWidth = 64;
			unsigned int _tileHeight = 64;

			bool _vsync = false;
			WindowMode _windowMode = WindowMode::Fullscreen;
			unsigned int _maxFPS = 0;

			VectorU _preferredResolution{ 0, 0 };
			unsigned int _preferredDisplay = 0;
		};

		class Physics
		{
			friend Settings;
		public:
			/**
			 * The time between fixed frames in ms.
			 *
			 * Fixed frames call callback functions like fixedUpdate() at this interval, regardless of the application's frame rate. For example, if this value is 20, then there are 1000 / 20 = 50 calls per second.
			 * In the event that fixedUpdate() takes longer than its own delta-time, it may skip frames to catch up.
			 */
			[[nodiscard]] static float fixedDeltaTime();

			/**
			 * The amount of pixels per meter.
			 * This is used by the physics engine because Box2D uses a meter-based system where Tristeon uses pixel-based sizes.
			 * To make up for this, the size of an object is divided by this value. Try to maintain a relatively accurate conversion rate to make for sensible physics interactions because Box2D uses the size in meters for mass calculations.
			 *
			 * For example, if an object is 512 pixels tall, and pixelsPerMeter is 256, then the object is seen as a 2 meter tall object in physics.
			 */
			[[nodiscard]] static unsigned int pixelsPerMeter();

		private:
			float _fixedDeltaTime = 1 / 50.0f * 1000.0f;
			unsigned int _pixelsPerMeter = 64;
		};

	private:
		static Graphics _graphics;
		static Physics _physics;
		static String _assetPath;
		static String _firstScene;

		static nlohmann::json _data;
		
		static void loadBuild();
		static void load(const String& folder);
		static void save();

		/**
		 * Static version of the Serializable::serialize function.
		 * Writes the data into a json object.
		 */
		[[nodiscard]] static json serialize();

		/**
		 * Static version of the Serializable::deserialize function.
		 * Reads the data from a json object.
		 */
		static void deserialize(json j);
	};
}