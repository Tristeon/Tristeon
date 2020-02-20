#pragma once
#include <vector>
#include <memory>

namespace Tristeon
{
	class SceneManager;
	class Layer;
	class Engine;
	class HUD;
	class Window;
	
	class Scene
	{
		friend Engine;
		friend Window;
		friend SceneManager;
	public:
		Scene() = default;
		~Scene() = default;
		
		Layer* getLayer(unsigned int index) const;
		unsigned int getLayerCount() const;
		HUD* getHUD() const;
	private:
		void update();

		HUD* currentHUD;
		std::vector<std::unique_ptr<Layer>> layers;
	};
}