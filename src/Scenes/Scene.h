#pragma once
#include <vector>
#include <memory>
#include "Camera.h"

namespace Tristeon
{
	class SceneManager;
	class Layer;
	class Engine;
	class HUD;
	class GameView;
	
	class Scene
	{
		friend Engine;
		friend GameView;
		friend SceneManager;
	public:
		Scene() = default;
		~Scene() = default;
		
		Layer* getLayer(unsigned int const& index) const;
		unsigned int getLayerCount() const;
		HUD* getHUD() const;
		Camera* getCamera() const;
	private:
		void update();

		std::unique_ptr<Camera> camera = nullptr;
		HUD* hud = nullptr;
		std::vector<std::unique_ptr<Layer>> layers;
	};
}
