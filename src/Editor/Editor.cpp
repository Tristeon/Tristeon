#ifdef TRISTEON_EDITOR
#include "Editor.h"
#include <Scenes/SceneManager.h>

namespace TristeonEditor
{
	void Editor::initialize()
	{
		//Setup scene load callback
		Tristeon::SceneManager::sceneLoaded += [&](Tristeon::Scene * scene)
		{
			for (auto window : windows)
				window->loadScene(scene);
		};

		//Initialize windows
		for (auto window : windows)
			window->initialize();
	}
}
#endif