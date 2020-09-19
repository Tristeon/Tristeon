#pragma once
#include <memory>
#include <Utils/TypeDefinitions.h>
#include <Scenes/Actors/Behaviour.h>
#include <Utils/Delegate.h>

#ifdef TRISTEON_EDITOR
namespace TristeonEditor { class EditorWindow; }
#endif

namespace Tristeon
{
	class Scene;

	/**
	 * SceneManager is a static class that manages scene loading.
	 * Only one scene can be loaded at the same time and it can be accessed through SceneManager::current()
	 *
	 * Scenes can be loaded by name or through json data, using the load() functions.
	 * For the sake of safety and consistency, scenes are only loaded at the end of the game loop.
	 */
	class SceneManager final
	{
		friend class Engine;
		friend class Window;

#ifdef TRISTEON_EDITOR
		friend TristeonEditor::EditorWindow;
#endif

	public:
		/**
		 * Returns the currently active scene.
		 * The scene is never nullptr but it could be empty within the editor.
		 */
		[[nodiscard]] static Scene* current();

		/**
		 * Unloads the current scene and then loads a scene with the given name (no file extension).
		 *
		 * This change gets applied at the end of the game loop.
		 */
		static void load(const String& name);

		/**
		 * Unloads the current scene and then loads a scene with the given data.
		 *
		 * This change gets applied at the end of the game loop.
		 */
		static void load(const json& data);
		
		/**
		 * Reloads the current scene from file.
		 */
		static void reload();
		
		/**
		 * Saves the given scene as a file at the filepath.
		 */
		static void save(Scene* scene, const String& filepath);

		/**
		 * Saves the current scene if it has a filepath. If not, nothing happens.
		 */
		static void saveCurrent();

		static Delegate<Scene*> sceneLoaded;
	private:
		/**
		 * Loads the scene after the frame has finished.
		 * This is to prevent incorrect destruction mid-loop
		 */
		static void processCachedLoad();
		
		/**
		 * Clears the current scene.
		 */
		static void reset();
		
		static Unique<Scene> _current;
		static String _cachedName;
		static json _cachedData;
	};
}
