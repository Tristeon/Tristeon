#include "Project.h"

#include <json.h>

#include "Serialization/JsonSerializer.h"

namespace Tristeon
{
	Project::Graphics Project::m_graphics;
	Project::Physics Project::m_physics;
	String Project::m_assetPath;
	String Project::m_firstScene;
	
	String Project::assetPath()
	{
		return m_assetPath;
	}

	String Project::firstSceneName()
	{
		return m_firstScene;
	}

	unsigned Project::Graphics::tileWidth()
	{
		return m_graphics.m_tileWidth;
	}

	void Project::Graphics::setTileWidth(const unsigned& value)
	{
		m_graphics.m_tileWidth = value;
	}

	unsigned Project::Graphics::tileHeight()
	{
		return m_graphics.m_tileHeight;
	}

	void Project::Graphics::setTileHeight(const unsigned& value)
	{
		m_graphics.m_tileHeight = value;
	}

	bool Project::Graphics::vsync()
	{
		return m_graphics.m_vsync;
	}

	void Project::Graphics::setVSync(const bool& value)
	{
		m_graphics.m_vsync = value;
	}

	bool Project::Graphics::tripleBuffering()
	{
		return m_graphics.m_tripleBuffering;
	}

	void Project::Graphics::setTripleBuffering(const bool& value)
	{
		m_graphics.m_tripleBuffering = value;
	}

	bool Project::Graphics::fullscreen()
	{
		return m_graphics.m_fullscreen;
	}

	void Project::Graphics::setFullscreen(const bool& value)
	{
		m_graphics.m_fullscreen = value;
	}

	float Project::Physics::fixedDeltaTime()
	{
		return m_physics.m_fixedDeltaTime;
	}

	unsigned Project::Physics::pixelsPerMeter()
	{
		return m_physics.m_pixelsPerMeter;
	}

	void Project::loadBuild()
	{
		json const file = JsonSerializer::load("build.json");

		String const projectName = file["name"];
		load(projectName + "/");
	}

	void Project::load(String const& folder)
	{
		json const file = JsonSerializer::load(folder + "settings.tristeon");

		if (file.empty() || file.is_null())
		{
			std::cout << "Failed to load project" << std::endl;
			return;
		}

		m_assetPath = folder;
		m_firstScene = file.value("firstScene", "");
		
		if (file.contains("graphics"))
		{
			json const graphics = file["graphics"];
			m_graphics.m_tileWidth = graphics.value("tileWidth", 64);
			m_graphics.m_tileHeight = graphics.value("tileHeight", 64);

			m_graphics.m_vsync = graphics.value("vsync", false);
			m_graphics.m_tripleBuffering = graphics.value("tripleBuffering", true);
			m_graphics.m_fullscreen = graphics.value("fullscreen", true);
		}

		if (file.contains("physics"))
		{
			json const physics = file["physics"];
			m_physics.m_fixedDeltaTime = physics.value("fixedDeltaTime", 1.0f / 50.0f * 1000.0f);
			m_physics.m_pixelsPerMeter = physics.value("pixelsPerMeter", 64);
		}
	}
}
