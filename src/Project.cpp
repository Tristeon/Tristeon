#include "Project.h"

#include <json.h>
#include "Serialization/JsonSerializer.h"

namespace Tristeon
{
	Project::Graphics Project::_graphics;
	Project::Physics Project::_physics;
	String Project::_assetPath;
	String Project::_firstScene;
	
	String Project::assetPath()
	{
		return _assetPath;
	}

	String Project::firstSceneName()
	{
		return _firstScene;
	}

	unsigned Project::Graphics::tileWidth()
	{
		return _graphics._tileWidth;
	}

	unsigned Project::Graphics::tileHeight()
	{
		return _graphics._tileHeight;
	}

	bool Project::Graphics::vsync()
	{
		return _graphics._vsync;
	}

	bool Project::Graphics::tripleBuffering()
	{
		return _graphics._tripleBuffering;
	}

	bool Project::Graphics::fullscreen()
	{
		return _graphics._fullScreen;
	}

	float Project::Physics::fixedDeltaTime()
	{
		return _physics._fixedDeltaTime;
	}

	unsigned Project::Physics::pixelsPerMeter()
	{
		return _physics._pixelsPerMeter;
	}

	void Project::loadBuild()
	{
		json const file = JsonSerializer::load("build.json");

		String const projectName = file.value("name", "");
		load(projectName + "/");
	}

	void Project::load(const String& folder)
	{
		auto const file = JsonSerializer::load(folder + "settings.tristeon");

		if (file.empty() || file.is_null())
		{
			Console::warning("Failed to load project at folder " + folder);
			return;
		}

		Console::write("Loaded project folder " + folder);
		
		_assetPath = folder;
		_firstScene = file.value("firstScene", "");
		
		if (file.contains("graphics"))
		{
			json const graphics = file["graphics"];
			_graphics._tileWidth = graphics.value("tileWidth", 64);
			_graphics._tileHeight = graphics.value("tileHeight", 64);

			_graphics._vsync = graphics.value("vsync", false);
			_graphics._tripleBuffering = graphics.value("tripleBuffering", true);
			_graphics._fullScreen = graphics.value("fullscreen", true);
		}

		if (file.contains("physics"))
		{
			json const physics = file["physics"];
			_physics._fixedDeltaTime = physics.value("fixedDeltaTime", 1.0f / 50.0f * 1000.0f);
			_physics._pixelsPerMeter = physics.value("pixelsPerMeter", 64);
		}
	}
}