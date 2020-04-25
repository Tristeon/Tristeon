#ifdef TRISTEON_EDITOR
#include "Editor.h"
#include <Window.h>
#include <Scenes/SceneManager.h>

namespace TristeonEditor
{
	void Editor::initialize()
	{
		//Setup scene load callback
		Tristeon::SceneManager::sceneLoaded += [&](Tristeon::Scene * scene)
		{
			selectedLayer(nullptr);
			for (auto window : windows)
				window->loadScene(scene);
		};

		//Initialize windows
		for (auto window : windows)
			window->initialize();
	}

	void Editor::addWindow(EditorWindow* window)
	{
		windows.add(window);
		window->editor = this;
	}

	Tristeon::Layer* Editor::selectedLayer() const
	{
		return _selectedLayer;
	}

	void Editor::selectedLayer(Tristeon::Layer* value)
	{
		_selectedLayer = value;

		for (auto window : windows)
			window->selectedLayerChanged(value);
	}
}
#endif