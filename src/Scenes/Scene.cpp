#include "Scene.h"
#include <stdexcept>

#include <Scenes/HUD.h>
#include <Scenes/Layers/Layer.h>

namespace Tristeon
{
	REGISTER_TYPE_CPP(Scene)

	Scene::Scene()
	{
		camera = std::make_unique<Camera>();
		hud = std::make_unique<HUD>();
	}

	json Scene::serialize()
	{
		json j;
		j["typeID"] = TRISTEON_TYPENAME(Scene);
		j["camera"] = camera->serialize();
		j["hud"] = hud->serialize();

		json serializedLayers = json::array_t();
		for (auto& layer : layers)
			serializedLayers.push_back(layer->serialize());
		j["layers"] = serializedLayers;
		return j;
	}

	void Scene::deserialize(json j)
	{
		camera->deserialize(j["camera"]);
		hud->deserialize(j["hud"]);

		layers.clear(); //TODO: Could detect and reuse existing layers as opposed to clearing every time
		for (auto serializedLayer : j["layers"])
		{
			Unique<Serializable> serializable = TypeRegister::createInstance(serializedLayer["typeID"]);
			serializable->deserialize(serializedLayer);
			layers.push_back(Unique<Layer>((Layer*)serializable.release()));
		}
	}

	Layer* Scene::getLayer(unsigned int const& index) const
	{
		if (index < 0 || index > layers.size() - 1)
			throw std::invalid_argument("Index must be more than 0 and less than the amount of layers");

		return layers[index].get();
	}

	Layer* Scene::findLayer(std::string const& name) const
	{
		for (const auto& layer : layers)
		{
			if (layer->name == name)
				return layer.get();
		}
		return nullptr;
	}

	unsigned int Scene::getLayerCount() const
	{
		return layers.size();
	}

	void Scene::destroyLayer(Layer* layer)
	{
		for (size_t i = 0; i < layers.size(); i++)
		{
			if (layers[i].get() == layer)
			{
				layers[i].reset();
				layers.removeAt(i);
				break;
			}
		}
	}

	HUD* Scene::getHUD() const
	{
		return hud.get();
	}

	Camera* Scene::getCamera() const
	{
		return camera.get();
	}
}
