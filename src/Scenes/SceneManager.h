#pragma once
#include <memory>

#include <TypeDefinitions.h>

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
		 */
		static void load(String const& name);

		/**
		 * Saves the given scene as a file at the filepath.
		 */
		static void save(Scene* scene, String const& filepath);
	private:
		/**
		 * Finds the path of the first scene with the given name.
		 */
		static String findPath(String const& name);

		/**
		 * TODO: Remove once we have an editor and this becomes unnecessary.
		 */
		static void saveTestScene();

		/**
		 * Clears the current scene.
		 */
		static void reset();
		
		static Unique<Scene> currentScene;
	};
}
