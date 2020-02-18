#include "Scene.h"
#include <stdexcept>
namespace Tristeon
{
	Layer* Scene::getLayer(unsigned int index) const
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
		return currentHUD;
	}

	void Scene::update()
	{
		//TODO: Implement scene update
	}
}