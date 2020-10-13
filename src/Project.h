#pragma once
#include <Standard/String.h>

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
			 * If triple buffering is enabled.
			 */
			[[nodiscard]] static bool tripleBuffering();
			/**
			 * If fullscreen is enabled in the build.
			 */
			[[nodiscard]] static bool fullscreen();

		private:
			unsigned int _tileWidth = 64;
			unsigned int _tileHeight = 64;

			bool _vsync = false;
			bool _tripleBuffering = true;

			bool _fullScreen = true;
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

		static void loadBuild();
		static void load(const String& folder);
	};
}