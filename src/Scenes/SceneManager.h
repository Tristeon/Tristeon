#pragma once
#include <memory>

#include <TypeDefinitions.h>

#include <Actors/Behaviours/Behaviour.h>

#include "Utils/Delegate.h"

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

		/**
		 * Unloads the current scene and then loads a scene with the given name (no file extension).
		 *
		 * This change gets applied after the full frame has finished.
		 */
		static void load(String const name);

		/**
		 * Reloads the current scene from file.
		 */
		static void reload();
		
		/**
		 * Saves the given scene as a file at the filepath.
		 */
		static void save(Scene* scene, String const& filepath);

		/**
		 * Saves the current scene if it has a filepath. If not, nothing happens.
		 */
		static void saveCurrent();

		static Delegate<Scene*> sceneLoaded;
	private:
		/**
		 * Finds the actor's layer and removes the actor from said layer.
		 * Then destroys the actor itself.
		 * Used internally by Engine to avoid deleting actors within critical loops.
		 */
		static void destroyActor(Actor* actor);

		/**
		 * Loads the scene after the frame has finished.
		 * This is to prevent incorrect destruction mid-loop
		 */
		static void processCachedLoad();
		
		/**
		 * Clears the current scene.
		 */
		static void reset();
		
		static Unique<Scene> currentScene;
		static String cachedSceneName;
	};
}
