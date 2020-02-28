#pragma once
#include <memory>

namespace Tristeon
{
	class Engine;
	class Scene;

	class SceneManager final
	{
		friend Engine;
	public:
		static Scene* getCurrentScene();
	private:
		static void loadScene();
		static void reset();
		
		static std::unique_ptr<Scene> currentScene;
	};
}
