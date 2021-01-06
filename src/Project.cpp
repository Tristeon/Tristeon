#include "Project.h"

#include <json.h>

#include "Window.h"
#include "Serialization/JsonSerializer.h"

namespace Tristeon
{
	Project::Graphics Project::_graphics;
	Project::Physics Project::_physics;
	String Project::_assetPath;
	String Project::_firstScene;
	nlohmann::json Project::_data;
	
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

	void Project::Graphics::setVsync(const bool& value)
	{
		_graphics._vsync = value;
		_data["graphics"]["vsync"] = value;
		save();
		Window::instance()->_setVsync(value);
	}

	Project::Graphics::WindowMode Project::Graphics::windowMode()
	{
		return _graphics._windowMode;
	}

	void Project::Graphics::setWindowMode(const WindowMode& value)
	{
		_graphics._windowMode = value;
		_data["graphics"]["windowMode"] = value;
		save();
		Window::instance()->_setWindowMode(value);
	}

	VectorU Project::Graphics::preferredResolution()
	{
		return _graphics._preferredResolution;
	}

	void Project::Graphics::setPreferredResolution(const VectorU& value)
	{
		_graphics._preferredResolution = value;
		_data["graphics"]["preferredResolution"] = value;
		save();
		Window::instance()->_setResolution(value);
	}

	unsigned Project::Graphics::maxFPS()
	{
		return _graphics._maxFPS;
	}

	void Project::Graphics::setMaxFPS(const unsigned& value)
	{
		_graphics._maxFPS = value;
		_data["graphics"]["maxFPS"] = value;
		save();
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
		_firstScene = file.value("firstScene", "Scene");
		if (_firstScene.empty())
			_firstScene = "Scene";
		
		if (file.contains("graphics"))
		{
			json const graphics = file["graphics"];
			_graphics._tileWidth = graphics.value("tileWidth", 64u);
			_graphics._tileHeight = graphics.value("tileHeight", 64u);

			_graphics._vsync = graphics.value("vsync", false);
			_graphics._windowMode = graphics.value("windowMode", Graphics::WindowMode::Fullscreen);
			_graphics._preferredResolution = graphics.value("preferredResolution", VectorI{ 0, 0 });
			_graphics._maxFPS = graphics.value("maxFPS", 0);
		}

		if (file.contains("physics"))
		{
			json const physics = file["physics"];
			_physics._fixedDeltaTime = physics.value("fixedDeltaTime", 1.0f / 50.0f * 1000.0f);
			_physics._pixelsPerMeter = physics.value("pixelsPerMeter", 64u);
		}
	}

	void Project::save()
	{
		JsonSerializer::save(_data, _assetPath + "settings.tristeon");
	}
}