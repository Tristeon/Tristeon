#pragma once
#include <vector>
#include <memory>
#include "Camera.h"
#include "TypeDefinitions.h"

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

		Unique<Camera> camera = nullptr;
		HUD* hud = nullptr;
		Vector<Unique<Layer>> layers;
	};
}
