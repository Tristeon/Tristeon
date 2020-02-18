#pragma once
#include <vector>
#include <memory>

namespace Tristeon
{
	class Layer;
	class Engine;
	class HUD;

	class Scene
	{
		friend Engine;
	public:
		Layer* getLayer(unsigned int index) const;
		unsigned int getLayerCount() const;
		HUD* getHUD() const;
	private:
		void update();

		HUD* currentHUD;
		std::vector<std::unique_ptr<Layer>> layers;
	};
}