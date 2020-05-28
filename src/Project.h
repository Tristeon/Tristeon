#pragma once
#include <TypeDefinitions.h>

#ifdef TRISTEON_EDITOR
namespace TristeonEditor
{
	class Editor;
}
#endif

namespace Tristeon
{
	class Project
	{
		friend class Engine;
		friend class Graphics;
		friend class Physics;
		
#ifdef TRISTEON_EDITOR
		friend TristeonEditor::Editor;
#endif

	public:
		static String assetPath();
		static String firstSceneName();
		
		class Graphics
		{
			friend Project;
		public:
			static unsigned int tileWidth();
			static void setTileWidth(const unsigned int& value);

			static unsigned int tileHeight();
			static void setTileHeight(const unsigned int& value);

			static bool vsync();
			static void setVSync(const bool& value);

			static bool tripleBuffering();
			static void setTripleBuffering(const bool& value);

			static bool fullscreen();
			static void setFullscreen(const bool& value);

		private:
			unsigned int m_tileWidth = 64;
			unsigned int m_tileHeight = 64;

			bool m_vsync = false;
			bool m_tripleBuffering = true;

			bool m_fullscreen = true;
		};

		class Physics
		{
			friend Project;
		public:
			static float fixedDeltaTime();
			static unsigned int pixelsPerMeter();

		private:
			float m_fixedDeltaTime = 1 / 50.0f * 1000.0f;
			unsigned int m_pixelsPerMeter = 64;
		};

	private:
		static Graphics m_graphics;
		static Physics m_physics;
		static String m_assetPath;
		static String m_firstScene;

		static void loadBuild();
		static void load(String const& folder);
	};
}