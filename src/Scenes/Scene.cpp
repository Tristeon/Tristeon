#include "Scene.h"
#include <stdexcept>
namespace Tristeon
{
	Layer* Scene::getLayer(unsigned int const& index) const
	{
		if (index < 0 || index > layers.size() - 1)
			throw std::invalid_argument("Index must be more than 0 and less than the amount of layers");

		return layers[index].get();
	}

	unsigned int Scene::getLayerCount() const
	{
		return layers.size();
	}

	HUD* Scene::getHUD() const
	{
		return hud;
	}

	Camera* Scene::getCamera() const
	{
		return camera.get();
	}

	void Scene::update()
	{
		//TODO: Implement scene update
	}
}