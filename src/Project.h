#pragma once
#include <Standard/String.h>
#include "Math/Vector.h"
#include "Serialization/Serializable.h"

int main(int argc, char** argv);

#ifdef TRISTEON_EDITOR
namespace TristeonEditor
{
	class ProjectWindow;
}
#endif

namespace Tristeon
{
	/**
	 * Project is the main interface for interacting with a project's settings, filepaths, etc.
	 *
	 * Some of these properties are also available through other class interfaces like Grid.
	 */
	class Project
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
		 * The path to the project.
		 */
		[[nodiscard]] static String assetPath();
		/**
		 * The first scene to be loaded in build mode.
		 */
		[[nodiscard]] static String firstSceneName();

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
		
		class Graphics
		{
			friend Project;
		public:
			/**
			 * The width of tiles in pixels.
			 */
			[[nodiscard]] static unsigned int tileWidth();
			/**
			 * The height of tiles in pixels.
			 */
			[[nodiscard]] static unsigned int tileHeight();

			/**
			 * If vertical sync is enabled.
			 */
			[[nodiscard]] static bool vsync();
			/**
			 * Enable/disable vsync in the Project's settings and for the runtime window.
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
			 * The current window mode.
			 */
			[[nodiscard]] static WindowMode windowMode();
			/**
			 * Set the window mode in the Project's settings and the runtime window.
			 */
			static void setWindowMode(const WindowMode& value);
			
			/**
			 * The resolution that the game will take when loaded in on fullscreen.
			 */
			[[nodiscard]] static VectorU preferredResolution();
			/**
			 * Set the preferred resolution.
			 * The game will attempt to take this resolution when loaded in on fullscreen.
			 *
			 * It also sets the current Window's resolution.
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
			 * This affects both the Project's and the runtime's settings.
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
			friend Project;
		public:
			/**
			 * The time between fixed frames.
			 */
			[[nodiscard]] static float fixedDeltaTime();
			/**
			 * The amount of pixels per meter.
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
	};
}