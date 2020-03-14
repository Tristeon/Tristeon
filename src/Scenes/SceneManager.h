#pragma once
#include <memory>

namespace Tristeon
{
	class Window;
	class Engine;
	class Scene;

	/**
	 * SceneManager is a static class that manages scene loading.
	 * Only one scene can be loaded at the same time and it can be accessed through SceneManager::current()
	 */
	class SceneManager final
	{
		friend Engine;
		friend Window;
	public:
		/**
		 * Returns the currently active scene.
		 * The scene is never nullptr but it could be empty within the editor.
		 */
		static Scene* current();

	private:
		static void loadScene();
		static void reset();
		
		static std::unique_ptr<Scene> currentScene;
	};
}
