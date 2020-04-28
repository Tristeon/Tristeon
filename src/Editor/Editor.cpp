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
			Tristeon::Window::instance()->setWindowTitle(QString::fromStdString("Tristeon2D | " + scene->getName()));
			
			selectedLayer(nullptr);
			for (auto window : windows)
				window->sceneLoaded(scene);
		};

		if (menuBar != nullptr)
			menuBar->initialize();
		
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

		selectedActor(nullptr);
		
		for (auto window : windows)
			window->selectedLayerChanged(value);
	}

	Tristeon::Actor* Editor::selectedActor() const
	{
		return _selectedActor;
	}

	void Editor::selectedActor(Tristeon::Actor* value)
	{
		_selectedActor = value;

		for (auto window : windows)
			window->selectedActorChanged(value);
	}

	void Editor::setMenuBar(MenuBar* menu_bar)
	{
		menuBar = menu_bar;
	}
}
#endif