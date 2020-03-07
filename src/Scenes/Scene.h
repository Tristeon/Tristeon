#pragma once
#include <memory>
#include <vector>

#include <Scenes/Camera.h>
#include <Serialization/Serializable.h>
#include <Serialization/TypeRegister.h>
#include <TypeDefinitions.h>

namespace Tristeon
{
	class SceneManager;
	class Layer;
	class Engine;
	class HUD;
	class GameView;
	
	class Scene : public Serializable
	{
		friend Engine;
		friend GameView;
		friend SceneManager;

		REGISTER_TYPE_H(Scene)
		
	public:
		Scene();
		~Scene() = default;

		json serialize() override;
		void deserialize(json j) override;
		
		Layer* getLayer(unsigned int const& index) const;
		unsigned int getLayerCount() const;
		HUD* getHUD() const;
		Camera* getCamera() const;
	private:
		void update();

		Unique<Camera> camera = nullptr;
		Unique<HUD> hud = nullptr;
		Vector<Unique<Layer>> layers;
	};
}
